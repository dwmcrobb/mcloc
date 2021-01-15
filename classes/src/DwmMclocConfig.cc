//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfig.cc 11829 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2020
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
//!  \file DwmMclocConfig.cc
//!  \brief Dwm::Mcloc::Config class implementations
//---------------------------------------------------------------------------

#include <algorithm>

#include "DwmMclocCodeUtils.hh"
#include "DwmMclocConfig.hh"

static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfig.cc 11829 $");

namespace Dwm {

  namespace Mcloc {

    namespace Config {

      using namespace std;
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      Language::Language(const string & name, const set<string> & extensions,
                         const map<string,regex> & expressions,
                         const std::vector<std::string> & comments)
          : _name(name), _extensions(extensions), _expressions(expressions),
            _comments(comments)
      {}
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const string & Language::Name() const
      {
        return _name;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const string & Language::Name(const string & name)
      {
        _name = name;
        return _name;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const set<string> & Language::Extensions() const
      {
        return _extensions;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const set<string> & Language::Extensions(const set<string> & extensions)
      {
        _extensions = extensions;
        return _extensions;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const map<string,regex> & Language::Expressions() const
      {
        return _expressions;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const map<string,regex> &
      Language::Expressions(const map<string,regex> & expressions)
      {
        _expressions = expressions;
        return _expressions;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::vector<std::string> & Language::Comments() const
      {
        return _comments;
      }
	
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::vector<std::string> &
      Language::Comments(const std::vector<std::string> & comments)
      {
        _comments = comments;
        return _comments;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Language::KeepOnlyExtensions(const set<string> & keep)
      {
        std::set<std::string>::iterator  it;
        for (it = _extensions.begin(); it != _extensions.end(); ) {
          if (keep.find(*it) == keep.end()) {
            it = _extensions.erase(it);
          }
          else {
            ++it;
          }
        }
        return;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool Language::operator < (const Language & language) const
      {
        return (_name < language._name);
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool Language::operator == (const Language & language) const
      {
        return (_name == language._name);
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      ostream & operator << (ostream & os, const Language & language)
      {
        for (const auto & comment : language._comments) {
          os << comment;
        }
        os << "      \"" << language._name << '"' << " {";
        if (! language._extensions.empty()) {
          os << " Extensions { ";
          auto  eit = language._extensions.begin();
          os << '"' << *eit << '"';
          ++eit;
          for ( ; eit != language._extensions.end(); ++eit) {
            os << ", \"" << *eit << '"';
          }
          os << " }";
        }
        if (! language._expressions.empty()) {
          os << " Expressions { ";
          auto  eit = language._expressions.begin();
          os << '"' << eit->first << '"';
          ++eit;
          for ( ; eit != language._expressions.end(); ++eit) {
            os << ", \"" << eit->first << '"';
          }
          os << " }";
        }
        os << " }";
        return os;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      Scanner::Scanner(const string & name,
                       const vector<Language> & languages,
                       const vector<string> & comments)
          : _name(name), _languages(languages), _comments(comments)
      {}
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const string & Scanner::Name() const
      {
        return _name;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const string & Scanner::Name(const string & name)
      {
        _name = name;
        return _name;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const vector<Language> & Scanner::Languages() const
      {
        return _languages;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const vector<Language> &
      Scanner::Languages(const vector<Language> & languages)
      {
        _languages = languages;
        return _languages;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::vector<std::string> & Scanner::Comments() const
      {
        return _comments;
      }

      //----------------------------------------------------------------------
      //!
      //----------------------------------------------------------------------
      const vector<string> & Scanner::Comments(const vector<string> & comments)
      {
        _comments = comments;
        return _comments;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Scanner::KeepOnlyExtensions(const std::set<std::string> & keep)
      {
        for (auto it = _languages.begin(); it != _languages.end(); ) {
          it->KeepOnlyExtensions(keep);
          if (it->Extensions().empty()) {
            it = _languages.erase(it);
          }
          else {
            ++it;
          }
        }
        return;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Scanner::KeepOnlyLanguages(const set<string> & keep)
      {
        set<string>  ks;
        for (const auto & k : keep) {
          ks.insert(CodeUtils::tolower(k));
        }
        for (auto it = _languages.begin(); it != _languages.end(); ) {
          if (ks.find(CodeUtils::tolower(it->Name())) == ks.end()) {
            it = _languages.erase(it);
          }
          else {
            ++it;
          }
        }
        return;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool Scanner::operator < (const Scanner & scanner) const
      {
        return (_name < scanner._name);
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool Scanner::operator == (const Scanner & scanner) const
      {
        return (_name == _name);
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      ostream & operator << (ostream & os, const Scanner & scanner)
      {
        for (const auto & comment : scanner._comments) {
          os << comment;
        }
        os << "  \"" << scanner._name << "\" {\n";
        if (! scanner._languages.empty()) {
          os << "    Languages {\n";
          for (const auto & language : scanner._languages) {
            os << language << '\n';
          }
          os << "    }\n";
        }
        os << "  }\n";
        return os;
      }
        
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const vector<Scanner> & Config::Scanners() const
      {
        return _scanners;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const vector<Scanner> &
      Config::Scanners(const vector<Scanner> & scanners)
      {
        _scanners = scanners;
        return _scanners;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::vector<std::string> & Config::Comments() const
      {
        return _comments;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::vector<std::string> &
      Config::Comments(const std::vector<std::string> & comments)
      {
        _comments = comments;
        return _comments;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Config::KeepOnlyExtensions(const std::set<std::string> & keep)
      {
        for (auto it = _scanners.begin(); it != _scanners.end(); ) {
          it->KeepOnlyExtensions(keep);
          if (it->Languages().empty()) {
            it = _scanners.erase(it);
          }
          else {
            ++it;
          }
        }
        return;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Config::KeepOnlyLanguages(const std::set<std::string> & keep)
      {
        for (auto it = _scanners.begin(); it != _scanners.end(); ) {
          it->KeepOnlyLanguages(keep);
          if (it->Languages().empty()) {
            it = _scanners.erase(it);
          }
          else {
            ++it;
          }
        }
        return;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Config::KeepOnlyScanners(const std::set<std::string> & ss)
      {
        auto  it = remove_if(_scanners.begin(), _scanners.end(),
                             [&] (const Scanner & sc)
                             { return (ss.find(CodeUtils::tolower(sc.Name()))
                                       == ss.end()); });
        _scanners.erase(it, _scanners.end());
        return;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      ostream & operator << (ostream & os, const Config & config)
      {
        for (const auto & comment : config._comments) {
          os << comment;
        }
        if (! config._scanners.empty()) {
          os << "Scanners {\n";
          for (const auto & scanner : config._scanners) {
            os << scanner;
          }
          os << "}\n";
        }
        return os;
      }


    }  // namespace Config

  }  // namespace Mcloc

}  // namespace Dwm
