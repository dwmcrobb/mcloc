//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocCodeUtils.cc 11829 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2019
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
//!  \file DwmCodeUtils.cc
//!  \brief Dwm::Mcloc::CodeUtils function implementations
//---------------------------------------------------------------------------

#include <map>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "DwmMclocCodeUtils.hh"
#include "DwmMclocConfig.hh"
#include "DwmMclocLanguage.hh"
#include "DwmMclocFileScanner.hh"
#include "DwmMclocAdaFileScanner.hh"
#include "DwmMclocClojureFileScanner.hh"
#include "DwmMclocCmakeFileScanner.hh"
#include "DwmMclocCppFileScanner.hh"
#include "DwmMclocCssFileScanner.hh"
#include "DwmMclocDFileScanner.hh"
#include "DwmMclocErlangFileScanner.hh"
#include "DwmMclocFlexFileScanner.hh"
#include "DwmMclocFortranFileScanner.hh"
#include "DwmMclocHaskellFileScanner.hh"
#include "DwmMclocHtmlFileScanner.hh"
#include "DwmMclocJuliaFileScanner.hh"
#include "DwmMclocKotlinFileScanner.hh"
#include "DwmMclocLispFileScanner.hh"
#include "DwmMclocLuaFileScanner.hh"
#include "DwmMclocM4FileScanner.hh"
#include "DwmMclocManpageFileScanner.hh"
#include "DwmMclocNoCommentsFileScanner.hh"
#include "DwmMclocPerlFileScanner.hh"
#include "DwmMclocPrologFileScanner.hh"
#include "DwmMclocPythonFileScanner.hh"
#include "DwmMclocRubyFileScanner.hh"
#include "DwmMclocRustFileScanner.hh"
#include "DwmMclocShellFileScanner.hh"
#include "DwmMclocSqlFileScanner.hh"
#include "DwmMclocTexFileScanner.hh"

using namespace std;

static const string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocCodeUtils.cc 11829 $");

namespace Dwm {

  namespace Mcloc {

    namespace CodeUtils {

      static unordered_map<string,const FileScanner *>  g_scanners;
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      static void FillScannerMap()
      {
        static mutex  mtx;
        lock_guard  lck(mtx);
        if (g_scanners.empty()) {
          g_scanners["Ada"]        = new AdaFileScanner();
          g_scanners["Clojure"]    = new ClojureFileScanner();
          g_scanners["Cmake"]      = new CmakeFileScanner();
          g_scanners["Cpp"]        = new CppFileScanner();
          g_scanners["CSS"]        = new CssFileScanner();
          g_scanners["D"]          = new DFileScanner();
          g_scanners["Erlang"]     = new ErlangFileScanner();
          g_scanners["Flex"]       = new FlexFileScanner();
          g_scanners["Fortran"]    = new FortranFileScanner();
          g_scanners["Haskell"]    = new HaskellFileScanner();
          g_scanners["HTML"]       = new HtmlFileScanner();
          g_scanners["Julia"]      = new JuliaFileScanner();
          g_scanners["Kotlin"]     = new KotlinFileScanner();
          g_scanners["Lisp"]       = new LispFileScanner();
          g_scanners["Lua"]        = new LuaFileScanner();
          g_scanners["m4"]         = new M4FileScanner();
          g_scanners["Manpage"]    = new ManpageFileScanner();
          g_scanners["NoComments"] = new NoCommentsFileScanner();
          g_scanners["Perl"]       = new PerlFileScanner();
          g_scanners["Prolog"]     = new PrologFileScanner();
          g_scanners["Python"]     = new PythonFileScanner();
          g_scanners["Ruby"]       = new RubyFileScanner();
          g_scanners["Rust"]       = new RustFileScanner();
          g_scanners["Shell"]      = new ShellFileScanner();
          g_scanners["SQL"]        = new SqlFileScanner();
          g_scanners["TeX"]        = new TexFileScanner();
        }
        return;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      set<string> ScannerNames()
      {
        set<string>  rc;
        FillScannerMap();
        for (auto it = g_scanners.begin(); it != g_scanners.end(); ++it) {
          rc.insert(it->first);
        }
        return rc;
      }
      
      //----------------------------------------------------------------------
      //!  We need a container that lets us quickly map from a file extension
      //!  to a pointer to a Language.
      //----------------------------------------------------------------------
      static unordered_map<string,shared_ptr<const Language>>
      g_extensionToLanguageMap;

      //----------------------------------------------------------------------
      //!  We need a container that lets us map from a filename to
      //!  to a pointer to a Language via regular expression match.
      //----------------------------------------------------------------------
      static vector<tuple<string,regex,shared_ptr<const Language>>>
      g_regexLanguageMap;

      //----------------------------------------------------------------------
      //!  Fills g_extensionToLanguageMap using the contents of g_languages.
      //----------------------------------------------------------------------
      static void
      FillExtensionToTypeAndScannerMap(const Config::Config & config)
      {
        for (const auto & scannerCfg : config.Scanners()) {
          auto  scit = g_scanners.find(scannerCfg.Name());
          if (scit != g_scanners.end()) {
            for (const auto & languageCfg : scannerCfg.Languages()) {
              shared_ptr<Language>  language =
                make_shared<Language>(languageCfg.Name(),
                                      languageCfg.Extensions(), scit->second);
              for (const auto & extension : languageCfg.Extensions()) {
                g_extensionToLanguageMap[extension] = language;
              }
              for (const auto & expression : languageCfg.Expressions()) {
                g_regexLanguageMap.push_back(tuple(expression.first,
                                                   expression.second,
                                                   language));
              }
            }
          }
        }
        return;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Initialize(const Config::Config & config)
      {
        FillScannerMap();
        FillExtensionToTypeAndScannerMap(config);
        return;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool IsSourceFile(const std::filesystem::path & fspath)
      {
        bool  rc = (g_extensionToLanguageMap.find(fspath.extension())
                    != g_extensionToLanguageMap.end());
        if (! rc) {
          auto  it =
            find_if(g_regexLanguageMap.begin(), g_regexLanguageMap.end(),
                    [&] (const tuple<string,regex,shared_ptr<const Language>> & p)
                    { return regex_match(fspath.filename().string(),
                                         get<1>(p)); });
          rc = (it != g_regexLanguageMap.end());
        }
        if (rc) {
          static regex   shlib(".+\\.so(\\.[0-9]+)*",
                               regex::ECMAScript | regex::optimize);
          rc = (! regex_match(fspath.filename().string(), shlib));
        }

        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool IsGenerator(const std::filesystem::path & fspath)
      {
        static const unordered_set<string>  generatorExtensions = {
          ".l", ".lex", ".ll",                        // flex input
          ".y", ".yy"                                 // bison input
        };
        return (generatorExtensions.find(fspath.extension())
                != generatorExtensions.end());
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string ExtensionLanguage(const std::string & ext)
      {
        string  rc = "UNKNOWN";
        auto  language = ExtensionToLanguage(ext);
        if (nullptr != language) {
          rc = language->Name();
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const Language *ExtensionToLanguage(const std::string & ext)
      {
        const Language  *rc = nullptr;
        auto  it = g_extensionToLanguageMap.find(ext);
        if (it != g_extensionToLanguageMap.end()) {
          rc = it->second.get();
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const Language *
      LanguageByExpression(const std::filesystem::path & fspath)
      {
        const Language  *rc = nullptr;
        auto  it =
          find_if(g_regexLanguageMap.begin(), g_regexLanguageMap.end(),
                  [&] (const tuple<string,regex,shared_ptr<const Language>> & p)
                  { return regex_match(fspath.filename().string(),
                                       get<1>(p)); });
        if (it != g_regexLanguageMap.end()) {
          rc = get<2>(*it).get();
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string ExtensionScannerStyle(const std::string & ext)
      {
        string  rc = "UNKNOWN";
        auto  it = g_extensionToLanguageMap.find(ext);
        if (it != g_extensionToLanguageMap.end()) {
          rc = it->second->Scanner()->CommentStyle();
        }
        return rc;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const Language *PathToLanguage(const std::filesystem::path & fspath)
      {
        const Language *rc = ExtensionToLanguage(fspath.extension());
        if (nullptr == rc) {
          rc = LanguageByExpression(fspath);
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string ExtensionToType(const std::string & ext)
      {
        string  rc = "UNKNOWN";
        auto  it = g_extensionToLanguageMap.find(ext);
        if (it != g_extensionToLanguageMap.end()) {
          rc = it->second->Name();
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string PathToExpression(const std::filesystem::path & fspath)
      {
        string  rc = "UNKNOWN";
        auto  it =
          find_if(g_regexLanguageMap.begin(),
                  g_regexLanguageMap.end(),
                  [&] (const tuple<string,regex,
                       shared_ptr<const Language>> & p)
                  { return regex_match(fspath.filename().string(),
                                       get<1>(p)); });
        if (it != g_regexLanguageMap.end()) {
          rc = get<0>(*it);
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string TypeByExpression(const std::filesystem::path & fspath)
      {
        string  rc = "UNKNOWN";
        auto  it =
          find_if(g_regexLanguageMap.begin(),
                  g_regexLanguageMap.end(),
                  [&] (const tuple<string,regex,shared_ptr<const Language>> & p) 
                  { return regex_match(fspath.filename().string(),
                                       get<1>(p)); });
        if (it != g_regexLanguageMap.end()) {
          rc = get<2>(*it)->Name();
        }
        return rc;
      }
                             
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      string PathToType(const std::filesystem::path & fspath)
      {
        string  rc = ExtensionToType(fspath.extension());
        if (rc == "UNKNOWN") {
          rc = TypeByExpression(fspath);
        }
        return rc;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::string tolower(std::string s)
      {
        std::string  rc(s);
        
        std::transform(s.begin(), s.end(), rc.begin(), 
                       [] (unsigned char c)
                       { return std::tolower(c); });
        return rc;
      }
      
    }  // namespace CodeUtils

  }  // namespace Mcloc

}  // namespace Dwm
