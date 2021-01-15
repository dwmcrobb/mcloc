//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/qmcloc/main.cc 11836 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2021
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
//!  \file main.cc
//!  \author Daniel W. McRobb
//!  \brief qmcloc main()
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#include <QApplication>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QStyleFactory>
#include <QTreeView>
// #include <QCommandLineParser>
// #include <QCommandLineOption>

#include "DwmMclocArguments.hh"
#include "DwmMclocCodeUtils.hh"
#include "DwmMclocConfig.hh"
#include "DwmMclocPackageInfo.hh"
#include "DwmMclocMainWindow.hh"
#ifdef __APPLE__
  #include "DwmMclocMacOSIsDarkMode.hh"
#endif

using namespace std;

bool g_usingDarkPalette = false;

static const string  g_defaultConf("/usr/local/etc/mcloc.cfg");

//----------------------------------------------------------------------------
//!  If LANG is not set in the environment, or is set to "C", change it to
//!  "en_US.UTF-8" so we get commas in large numbers.
//----------------------------------------------------------------------------
static void SetLocale()
{
  static char  *defaultLang = strdup("LANG=en_US.UTF-8");
  char  *lang = getenv("LANG");
  if (nullptr == lang) {
    putenv(defaultLang);
  }
  else if ((1 == strlen(lang)) && (0 == strncmp(lang, "C", 1))) {
    putenv(defaultLang);
  }
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
void SetDarkPalette(QApplication & app)
{
  app.setStyle(QStyleFactory::create("Fusion"));
  
  QPalette newPalette;
  newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
  newPalette.setColor(QPalette::WindowText,      QColor(232, 232, 232));
  newPalette.setColor(QPalette::Base,            QColor( 28,  28,  28));
  newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));
  newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
  newPalette.setColor(QPalette::Text,            QColor(242, 242, 242));
  newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
  newPalette.setColor(QPalette::ButtonText,      QColor(242, 242, 242));
  newPalette.setColor(QPalette::BrightText,      QColor(255, 255, 255));
  newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
  newPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
  
  newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
  newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
  newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30) );
  newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
  newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));
  
  newPalette.setColor(QPalette::Disabled, QPalette::Text,
                      QColor(127, 127, 127));

  app.setPalette(newPalette);
  g_usingDarkPalette = true;
}

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

typedef   Dwm::Mcloc::Arguments<Dwm::Mcloc::Argument<'D',bool>,
                                Dwm::Mcloc::Argument<'E',set<string>>,
                                Dwm::Mcloc::Argument<'j',size_t>,
                                Dwm::Mcloc::Argument<'L',set<string>>,
                                Dwm::Mcloc::Argument<'S',set<string>>,
                                Dwm::Mcloc::Argument<'c',string>,
                                Dwm::Mcloc::Argument<'v',bool>> MyArgType;

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void InitArgs(MyArgType & args, const std::string & configPath)
{
  args.SetHelp<'D'>("use dark mode theme");
  args.Set<'c'>(configPath);
  args.SetValueName<'c'>("config");
  args.SetHelp<'c'>("specify configuration file");
  args.SetValueName<'E'>("extensions");
  args.SetHelp<'E'>("only scan files with extensions in the given"
                    " comma-separated list of extensions");
  args.Set<'j'>(thread::hardware_concurrency());
  if (args.Get<'j'>() > 24) {
    args.Set<'j'>(args.Get<'j'>() / 2);
  }
  args.SetValueName<'j'>("numThreads");
  args.SetHelp<'j'>("specify the number of parsing threads (default "
                    + to_string(args.Get<'j'>()) + ")");
  args.SetValueName<'L'>("languages");
  args.SetHelp<'L'>("only scan files for the languages in the given"
                    " comma-separated list of languages, which may"
                    " include:\n\n" + SupportedLanguages() );
  args.SetValueName<'S'>("scanners");
  args.SetHelp<'S'>("only enable scanners in the given comma-separated"
                    " list of scanners, which may include:\n\n"
                    + SupportedScanners());
  args.SetHelp<'v'>("show version and exit");
  args.LoadFromEnvironment("MCLOC");
}



//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  namespace  fs = std::filesystem;
  
  SetLocale();

  string  configPath = g_defaultConf;
  string  homeConfig(string(getenv("HOME")) + "/.mcloc");
  if (filesystem::exists(homeConfig)
      && filesystem::is_regular_file(homeConfig)) {
    //  We have a personal configuration file in our home directory.
    configPath = homeConfig;
  }

  QApplication app(argc, argv);

  MyArgType  args;
  InitArgs(args, configPath);
  int  argind = args.Parse(argc, argv);
  if (argind < 0) {
    cerr << args.Usage(argv[0], "[path]");
    exit(1);
  }
  if (args.Get<'v'>()) {
    cout << "qmcloc-" << Dwm::Mcloc::PackageInfo::Version() << '\n';
    exit(0);
  }

  if (args.Get<'D'>()) {
    SetDarkPalette(app);
  }
#ifdef __APPLE__
  if (Dwm::Mcloc::MacOS::IsDarkMode()) {
    g_usingDarkPalette = true;
  }
#endif  
  Dwm::Mcloc::Config::Config  config;
  if (config.Parse(args.Get<'c'>())) {
    if (! args.Get<'E'>().empty()) {
      config.KeepOnlyExtensions(args.Get<'E'>());
    }
    if (! args.Get<'L'>().empty()) {
      config.KeepOnlyLanguages(LowerStrings(args.Get<'L'>()));
    }
    Dwm::Mcloc::CodeUtils::Initialize(config);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    const QString rootPath = (argind < argc) ?
      fs::canonical(argv[argind]).string().c_str() : QString();
    Dwm::Mcloc::MainWindow  *mainWindow =
      new Dwm::Mcloc::MainWindow(rootPath);
    mainWindow->SetNumParsingThreads(args.Get<'j'>());
    mainWindow->show();
    return app.exec();
  }
  
}
