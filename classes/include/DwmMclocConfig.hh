//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocConfig.hh 11828 $
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
//!  \file DwmMclocConfig.hh
//!  \brief Dwm::Mcloc::Config class declarations
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCONFIG_HH_
#define _DWMMCLOCCONFIG_HH_

#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

namespace Dwm {

  namespace Mcloc {

    namespace Config {

      //----------------------------------------------------------------------
      //!  Encapsulate the configuration for a single language.
      //----------------------------------------------------------------------
      class Language
      {
      public:
        //--------------------------------------------------------------------
        //!  Construct from the @c name of the language, the set of file
        //!  @c extensions used for source code files in the language, and
        //!  a set of regular expressions to match filenames in the language.
        //--------------------------------------------------------------------
        Language(const std::string & name,
                 const std::set<std::string> & extensions,
                 const std::map<std::string,std::regex> & expressions,
                 const std::vector<std::string> & comments =
                 std::vector<std::string>());
        
        //--------------------------------------------------------------------
        //!  Only scan the source code files whose extensions are in @c keep.
        //--------------------------------------------------------------------
        void KeepOnlyExtensions(const std::set<std::string> & keep);
        
        //--------------------------------------------------------------------
        //!  Returns the name of the language.
        //--------------------------------------------------------------------
        const std::string & Name() const;

        //--------------------------------------------------------------------
        //!  Sets and returns the name of the language.
        //--------------------------------------------------------------------
        const std::string & Name(const std::string & name);

        //--------------------------------------------------------------------
        //!  Returns a const reference to the filename extensions used for
        //!  source code files in the language.
        //--------------------------------------------------------------------
        const std::set<std::string> & Extensions() const;

        //--------------------------------------------------------------------
        //!  Sets and returns the filename extensions used for source code
        //!  files in the language.
        //--------------------------------------------------------------------
        const std::set<std::string> &
        Extensions(const std::set<std::string> & extensions);
        
        //--------------------------------------------------------------------
        //!  Returns a const reference to the filename regular expressions
        //!  used to match source code files in the language.
        //--------------------------------------------------------------------
        const std::map<std::string,std::regex> & Expressions() const;

        //--------------------------------------------------------------------
        //!  Sets and returns the filename regular expressions used to match
        //!  source code files in the language.
        //--------------------------------------------------------------------
        const std::map<std::string,std::regex> &
        Expressions(const std::map<std::string,std::regex> & expressions);

        const std::vector<std::string> & Comments() const;

        const std::vector<std::string> &
        Comments(const std::vector<std::string> & comments);
        
        //--------------------------------------------------------------------
        //!  operator <
        //--------------------------------------------------------------------
        bool operator < (const Language & language) const;
        
        //--------------------------------------------------------------------
        //!  operator ==
        //--------------------------------------------------------------------
        bool operator == (const Language & language) const;

        //--------------------------------------------------------------------
        //!  Prints the language configuration to an ostream in the form
        //!  used in the configuration file.
        //--------------------------------------------------------------------
        friend std::ostream & operator << (std::ostream & os,
                                           const Language &);
        
      private:
        std::string                       _name;
        std::set<std::string>             _extensions;
        std::map<std::string,std::regex>  _expressions;
        std::vector<std::string>          _comments;
      };

      //----------------------------------------------------------------------
      //!  Encapsulate the configuration for a single scanner.
      //----------------------------------------------------------------------
      class Scanner
      {
      public:
        //--------------------------------------------------------------------
        //!  Construct from the given @c name of the scanner and the set
        //!  of @c languages the scanner supports.
        //--------------------------------------------------------------------
        Scanner(const std::string & name,
                const std::vector<Language> & languages,
                const std::vector<std::string> & comments = std::vector<std::string>());

        //--------------------------------------------------------------------
        //!  Copy constructor.
        //--------------------------------------------------------------------
        Scanner(const Scanner & scanner) = default;
        
        //--------------------------------------------------------------------
        //!  Returns the name of the scanner.
        //--------------------------------------------------------------------
        const std::string & Name() const;

        //--------------------------------------------------------------------
        //!  Only scan files with an extension in the given set of extensions
        //!  @c keep.
        //--------------------------------------------------------------------
        void KeepOnlyExtensions(const std::set<std::string> & keep);
        
        //--------------------------------------------------------------------
        //!  Only scan files from the given set of @c languages.
        //--------------------------------------------------------------------
        void KeepOnlyLanguages(const std::set<std::string> & languages);
        
        //--------------------------------------------------------------------
        //!  Set the name of the scanner.
        //--------------------------------------------------------------------
        const std::string & Name(const std::string & name);
        
        //--------------------------------------------------------------------
        //!  Returns the set of languages the scanner supports.
        //--------------------------------------------------------------------
        const std::vector<Language> & Languages() const;
        
        //--------------------------------------------------------------------
        //!  Sets and returns the set of languages the scanner supports.
        //--------------------------------------------------------------------
        const std::vector<Language> &
        Languages(const std::vector<Language> & languages);

        //--------------------------------------------------------------------
        //!  Returns the comments for the scanner.
        //--------------------------------------------------------------------
        const std::vector<std::string> & Comments() const;
        
        //--------------------------------------------------------------------
        //!  Sets and returns the comments for the scanner.
        //--------------------------------------------------------------------
        const std::vector<std::string> &
        Comments(const std::vector<std::string> & comments);
        
        //--------------------------------------------------------------------
        //!  operator <
        //--------------------------------------------------------------------
        bool operator < (const Scanner & scanner) const;
        
        //--------------------------------------------------------------------
        //!  operator ==
        //--------------------------------------------------------------------
        bool operator == (const Scanner & scanner) const;

        //--------------------------------------------------------------------
        //!  Prints a Scanner to an ostream in the form used in the
        //!  configuration file.  Returns the ostream.
        //--------------------------------------------------------------------
        friend std::ostream & operator << (std::ostream & os, const Scanner &);
        
      private:
        std::string               _name;
        std::vector<Language>     _languages;
        std::vector<std::string>  _comments;
      };

      //----------------------------------------------------------------------
      //!  Encapsulate the configuration of mcloc.
      //----------------------------------------------------------------------
      class Config
      {
      public:
        //--------------------------------------------------------------------
        //!  Default constructor.
        //--------------------------------------------------------------------
        Config() = default;

        //--------------------------------------------------------------------
        //!  Parse the configuration from the file at the given @c path.
        //!  Returns true on success, false on failure.
        //--------------------------------------------------------------------
        bool Parse(const std::string & path);

        //--------------------------------------------------------------------
        //!  Returns the configured scanners.
        //--------------------------------------------------------------------
        const std::vector<Scanner> & Scanners() const;

        //--------------------------------------------------------------------
        //!  Sets and returns the configured scanners.
        //--------------------------------------------------------------------
        const std::vector<Scanner> &
        Scanners(const std::vector<Scanner> & scanners);

        //--------------------------------------------------------------------
        //!  Returns the comments for the configuration.
        //--------------------------------------------------------------------
        const std::vector<std::string> & Comments() const;

        //--------------------------------------------------------------------
        //!  Sets and returns the comments for the configuration.
        //--------------------------------------------------------------------
        const std::vector<std::string> &
        Comments(const std::vector<std::string> & comments);
        
        //--------------------------------------------------------------------
        //!  Only scan files with an extension in the given set of
        //!  extensions @c keep.
        //--------------------------------------------------------------------
        void KeepOnlyExtensions(const std::set<std::string> & keep);

        //--------------------------------------------------------------------
        //!  Only scan files with an extension in the given set of
        //!  @c languages.
        //--------------------------------------------------------------------
        void KeepOnlyLanguages(const std::set<std::string> & languages);
        
        //--------------------------------------------------------------------
        //!  Only scan files that are handled by one of the given @c scanners.
        //--------------------------------------------------------------------
        void KeepOnlyScanners(const std::set<std::string> & scanners);
        
        //--------------------------------------------------------------------
        //!  Prints a Config to an ostream in the form used in the
        //!  configuration file.  Returns the ostream.
        //--------------------------------------------------------------------
        friend std::ostream & operator << (std::ostream & os, const Config &);
        
      private:
        std::vector<Scanner>      _scanners;
        std::vector<std::string>  _comments;
      };

    }  // namespace Config
      
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCONFIG_HH_
