%{
  //===========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocRecentsParser.y 11842 $
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
  //!  \file DwmMclocRecentsParser.y
  //!  \author Daniel W. McRobb
  //!  \brief Recent path parser (for qmcloc)
  //---------------------------------------------------------------------------

  #include <algorithm>
  #include <cstdio>
  #include <filesystem>
  #include <fstream>
  #include <string>
  #include <utility>

  extern "C" {
    extern void mclocrecentserror(const char *arg, ...);
    extern FILE *mclocrecentsin;
  }

  #include "DwmMclocRecents.hh"

  using namespace std;

  string                g_recentsPath;
  Dwm::Mcloc::Recents  *g_recents = nullptr;
%}
    
%define api.prefix {mclocrecents}

%union {
    time_t                 timeVal;
    const string          *stringVal;
    pair<time_t,string>   *recentVal;
}

%code provides
{
  // Tell Flex the expected prototype of yylex.
  #define YY_DECL                             \
    int mclocrecentslex ()

  // Declare the scanner.
  YY_DECL;
}

%token<timeVal>    TIME
%token<stringVal>  PATH
%type<recentVal>   Recent

%%

Recents: Recent
{
  if (g_recents) {
    g_recents->Add(*$1);
  }
  delete $1;
}
| Recents Recent
{
  if (g_recents) {
    g_recents->Add(*$2);
  }
  delete $2;
};

Recent: TIME PATH
{
    $$ = new std::pair<time_t,std::string>($1,*$2);
    delete $2;
};

%%

namespace Dwm {

  namespace Mcloc {

    namespace  fs = std::filesystem;
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    Recents::Recents()
    {
      if (EnsureRecentsDirExists()) {
        string  recentsFile(RecentsDir());
        recentsFile += "/recents";
        if (fs::is_regular_file(recentsFile)) {
          Parse(recentsFile);
        }
      }
    }

    //-----------------------------------------------------------------------
    //!  
    //-----------------------------------------------------------------------
    void Recents::Add(const pair<time_t,string> & timeAndPath)
    {
      _paths.push_back(timeAndPath);
      return;
    }
    
    //-----------------------------------------------------------------------
    //!  
    //-----------------------------------------------------------------------
    void Recents::Add(const std::string & path)
    {
      time_t  now = time((time_t *)0);
      auto  it = find_if(_paths.begin(), _paths.end(),
                         [&path] (const auto & p) 
                         { return (p.second == path); });
      if (it != _paths.end()) {
          _paths.erase(it);
      }
      _paths.push_front({now, path});
      Cull();
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void Recents::Cull(size_t numToKeep)
    {
      if (_paths.size() > numToKeep) {
        _paths.erase(_paths.begin() + (_paths.size() - numToKeep),
                     _paths.end());
      }
      Save();
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool Recents::EnsureRecentsDirExists()
    {
      bool         rc = false;
      std::string  saveDir = RecentsDir();
      if (fs::exists(saveDir)) {
        if (fs::is_directory(saveDir)) {
          rc = true;
        }
      }
      else {
        std::error_code  ec;
        if (fs::create_directory(saveDir, ec)) {
          rc = true;
        }
      }
      return rc;
    }
        
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    std::string Recents::RecentsDir()
    {
      std::string  rc;
      char  *homedir = getenv("HOME");
      if (homedir) {
        rc += homedir;
        rc += "/.qmcloc";
      }
      return rc;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void Recents::Save()
    {
      if (EnsureRecentsDirExists()) {
        string  recentsFile(RecentsDir());
        recentsFile += "/recents";
        std::ofstream  os(recentsFile);
        if (os) {
          for (const auto & p : _paths) {
            os << p.first << ' ' << '"' << p.second << '"' << '\n';
          }
        }
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool Recents::Parse(const string & recentsFile)
    {
      bool  rc = false;
      mclocrecentsin = fopen(recentsFile.c_str(), "r");
      if (mclocrecentsin) {
        g_recentsPath = recentsFile;
        g_recents = this;
        rc = (0 == mclocrecentsparse());
        fclose(mclocrecentsin);
        g_recents = nullptr;
      }
      return rc;
    }
    
  }  // namespace Mcloc

}  // namespace Dwm
