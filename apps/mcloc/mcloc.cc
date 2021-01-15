//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/mcloc/mcloc.cc 11834 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 1998, 2000, 2004, 2012, 2019-2021
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//  3. The names of the authors and copyright holders may not be used to
//     endorse or promote products derived from this software without
//     specific prior written permission.
//
//  IN NO EVENT SHALL DANIEL W. MCROBB BE LIABLE TO ANY PARTY FOR
//  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,
//  INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF DANIEL W. MCROBB HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//
//  THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND
//  DANIEL W. MCROBB HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
//  UPDATES, ENHANCEMENTS, OR MODIFICATIONS. DANIEL W. MCROBB MAKES NO
//  REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER
//  IMPLIED OR EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
//  OR THAT THE USE OF THIS SOFTWARE WILL NOT INFRINGE ANY PATENT,
//  TRADEMARK OR OTHER RIGHTS.
//===========================================================================

//---------------------------------------------------------------------------
//!  \file mcloc.cc
//!  \brief count lines of code in file(s) and/or directories
//---------------------------------------------------------------------------

#include <filesystem>
#include <iostream>
extern "C" {
  #include <unistd.h>
}

#include <cstdlib>
#include <regex>

#include "DwmMclocArguments.hh"
#include "DwmMclocConfig.hh"
#include "DwmMclocSourceCollection.hh"
#include "DwmMclocFileScanner.hh"
#include "DwmMclocPackageInfo.hh"
// #include "DwmMclocSvnTag.hh"

using namespace std;

static const string  g_defaultConf("/usr/local/etc/mcloc.cfg");

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static set<string> & LowerStrings(set<string> & ss)
{
  for (const auto & str : ss) {
    ss.erase(str);
    ss.insert(Dwm::Mcloc::CodeUtils::tolower(str));
  }
  return ss;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static string SupportedLanguages()
{
  using namespace Dwm::Mcloc;

  string          rc;
  vector<string>  languages;
  Config::Config  config;
  if (config.Parse(g_defaultConf)) {
    string  sep;
    for (const auto & scanner : config.Scanners()) {
      for (const auto & language : scanner.Languages()) {
        languages.push_back(language.Name());
      }
    }
    sort(languages.begin(), languages.end(),
         [] (const string & s1, const string & s2)
         {
           return (Dwm::Mcloc::CodeUtils::tolower(s1) < Dwm::Mcloc::CodeUtils::tolower(s2));
         });
    for (const auto & lang : languages) {
      rc += sep + lang;
      sep = ", ";
    }
  }
  return rc;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static string SupportedScanners()
{
  string    scanners;
  Dwm::Mcloc::Config::Config  config;
  if (config.Parse(g_defaultConf)) {
    string  sep;
    for (const auto & scanner : config.Scanners()) {
      scanners += sep + scanner.Name();
      sep = ", ";
    }
  }
  return scanners;
  
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
typedef   Dwm::Mcloc::Arguments<Dwm::Mcloc::Argument<'C',bool>,
                                Dwm::Mcloc::Argument<'E',set<string>>,
                                Dwm::Mcloc::Argument<'L',set<string>>,
                                Dwm::Mcloc::Argument<'S',set<string>>,
                                Dwm::Mcloc::Argument<'c',string>,
                                Dwm::Mcloc::Argument<'d',bool>,
                                Dwm::Mcloc::Argument<'e',bool>,
                                Dwm::Mcloc::Argument<'f',bool>,
                                Dwm::Mcloc::Argument<'l',bool>,
                                Dwm::Mcloc::Argument<'s',bool>,
                                Dwm::Mcloc::Argument<'g',bool>,
                                Dwm::Mcloc::Argument<'j',int>,
                                Dwm::Mcloc::Argument<'o',bool>,
                                Dwm::Mcloc::Argument<'q',bool>,
                                Dwm::Mcloc::Argument<'a',bool>,
                                Dwm::Mcloc::Argument<'r',bool>,
                                Dwm::Mcloc::Argument<'v',bool>> MyArgType;

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void InitArgs(MyArgType & args, const std::string & configPath)
{
  args.SetHelp<'C'>("print the configuration");
  args.SetValueName<'E'>("extensions");
  args.SetHelp<'E'>("only scan files with extensions in the given"
                    " comma-separated list of extensions");
  args.SetValueName<'L'>("languages");
  args.SetHelp<'L'>("only scan files for the languages in the given"
                    " comma-separated list of languages, which may"
                    " include:\n\n" + SupportedLanguages() );
  args.SetValueName<'S'>("scanners");
  args.SetHelp<'S'>("only enable scanners in the given comma-separated"
                    " list of scanners, which may include:\n\n"
                    + SupportedScanners());
  args.Set<'c'>(configPath);
  args.SetValueName<'c'>("config");
  args.SetHelp<'c'>("specify configuration file");
  args.SetHelp<'d'>("print directory counters");
  args.SetHelp<'e'>("print counters by file extension");
  args.SetHelp<'f'>("print counters by file (default)");
  args.SetHelp<'l'>("print counters by language");
  args.SetHelp<'s'>("print counters by scanner type");
  args.SetHelp<'g'>("include generated files");
  args.Set<'j'>(thread::hardware_concurrency());
  if (args.Get<'j'>() > 24) {
    args.Set<'j'>(args.Get<'j'>() / 2);
  }
  args.SetValueName<'j'>("numThreads");
  args.SetHelp<'j'>("specify the number of parsing threads (default "
                    + to_string(args.Get<'j'>()) + ")");
  args.SetHelp<'o'>("do not recurse into subdirectories");
  args.SetHelp<'q'>("only show total");
  args.SetHelp<'a'>("sort output by size, ascending");
  args.SetHelp<'r'>("sort output by size, descending");
  args.SetHelp<'v'>("show version and exit");
  
  args.SetConflicts({ {'d','e','f','l','s'}, {'a','r'} });

  args.LoadFromEnvironment("MCLOC");
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  string  configPath(g_defaultConf);
  string  homeConfig(string(getenv("HOME")) + "/.mcloc");
  if (filesystem::exists(homeConfig)
      && filesystem::is_regular_file(homeConfig)) {
    //  We have a personal configuration file in our home directory.
    configPath = homeConfig;
  }

  using Dwm::Mcloc::SourceCollection;

  MyArgType  args;
  InitArgs(args, configPath);
  
  int  argind = args.Parse(argc, argv);
  if (argind < 0) {
    cerr << args.Usage(argv[0], "path(s)");
    exit(1);
  }

  if (args.Get<'v'>()) {
    cout << Dwm::Mcloc::PackageInfo::Name() << '-'
         << Dwm::Mcloc::PackageInfo::Version() << '\n';
    exit(0);
  }
  
  int  numThreads = args.Get<'j'>();
  
  Dwm::Mcloc::Config::Config  config;
  if (config.Parse(configPath)) {
    if (! args.Get<'E'>().empty()) {
      config.KeepOnlyExtensions(args.Get<'E'>());
    }
    if (! args.Get<'L'>().empty()) {
      config.KeepOnlyLanguages(LowerStrings(args.Get<'L'>()));
    }
    if (! args.Get<'S'>().empty()) {
      config.KeepOnlyScanners(LowerStrings(args.Get<'S'>()));
    }
    
    if (args.Get<'C'>()) {
      cout << configPath;
      if (! args.Get<'E'>().empty()) { cout << ", modified by -E"; }
      if (! args.Get<'L'>().empty()) { cout << ", modified by -L"; }
      if (! args.Get<'S'>().empty()) { cout << ", modified by -S"; }
      cout << ":\n\n" << config;
      return 0;
    }
    
    if (argind >= argc) {
      cerr << args.Usage(argv[0], "path(s)");
      exit(1);
    }

    Dwm::Mcloc::CodeUtils::Initialize(config);

    Dwm::Mcloc::HowToSort  howToSort = Dwm::Mcloc::sortByName;
    if (args.Get<'a'>())      { howToSort = Dwm::Mcloc::sortBySize; }
    else if (args.Get<'r'>()) { howToSort = Dwm::Mcloc::sortBySizeReversed; }

    SourceCollection::WhatToPrint  printBy = SourceCollection::byFile;
    if (args.Get<'l'>()) { printBy = SourceCollection::byLanguage; }
    else if (args.Get<'d'>()) { printBy = SourceCollection::byDirectory; }
    else if (args.Get<'e'>()) { printBy = SourceCollection::byExtension; }
    else if (args.Get<'f'>()) { printBy = SourceCollection::byFile; }
    else if (args.Get<'s'>()) { printBy = SourceCollection::byScanner; }

    SourceCollection  sc;
    for (int i = argind; i < argc; ++i) {
      if (filesystem::exists(argv[i])) {
        if (filesystem::is_directory(argv[i])) {
          sc.AddDirectory(argv[i], (! args.Get<'o'>()));
        }
        else if (filesystem::is_regular_file(argv[i])) {
          sc.AddFile(argv[i]);
        }
      }
    }
    sc.Parse(numThreads);
    cout.imbue(std::locale(""));
    
    sc.Print(cout, printBy, args.Get<'g'>(), howToSort, args.Get<'q'>());
    
    return 0;
  }
  else {
    cerr << "Failed to parse configuration in '" << configPath << "'\n";
    args.Usage(argv[0], "path(s)");
  }
  return 1;
}
