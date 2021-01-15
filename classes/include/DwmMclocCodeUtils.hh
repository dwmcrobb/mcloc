//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocCodeUtils.hh 11828 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2019, 2020
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
//!  \file DwmCodeUtils.hh
//!  \brief Miscellaneous functions
//---------------------------------------------------------------------------

#ifndef _DWMCODEUTILS_HH_
#define _DWMCODEUTILS_HH_

#include <filesystem>
#include <set>
#include <string>

#include "DwmMclocConfig.hh"

namespace Dwm {

  namespace Mcloc {

    class Language;
      
    namespace CodeUtils {

      //----------------------------------------------------------------------
      //!  Initialize.  This must be called before any other function here.
      //----------------------------------------------------------------------
      void Initialize(const Config::Config & config);

      //----------------------------------------------------------------------
      //!  Returns the scanner names.
      //----------------------------------------------------------------------
      std::set<std::string> ScannerNames();
      
      //----------------------------------------------------------------------
      //!  Returns true if the file at @c fspath appears to be a source code
      //!  file.  Note this is done purely by file extension; we don't peek
      //!  inside the file.
      //----------------------------------------------------------------------
      bool IsSourceFile(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool IsGenerator(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Returns a string representation of the type of source file
      //!  represented by the filename extension @c ext.  If the extension
      //!  is unknown, returns "UNKNOWN".
      //----------------------------------------------------------------------
      std::string ExtensionToType(const std::string & ext);

      //----------------------------------------------------------------------
      //!  Returns a string representation of the language of source files
      //!  represented by the filename extension @c ext.  If the extension
      //!  is unknown, returns "UNKNOWN".
      //----------------------------------------------------------------------
      std::string ExtensionLanguage(const std::string & ext);

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::string ExtensionScannerStyle(const std::string & ext);

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::string PathToExpression(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Returns a string representation of the type of source file at
      //!  @c fspath: "C", "C++", "flex", "bison", et. al.  Note this is
      //!  done purely by file extension; we don't peek inside the file.
      //----------------------------------------------------------------------
      std::string PathToType(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Returns a pointer to the Language for the given filename extension
      //!  @c ext.  Returns @c nullptr if we do not have a language for
      //!  @c ext.
      //----------------------------------------------------------------------
      const Language *ExtensionToLanguage(const std::string & ext);

      //----------------------------------------------------------------------
      //!  Returns a pointer to the Language for the given filesystem path
      //!  @c fspath.  Returns @c nullptr if we do not have a Language for
      //!  the given @c fspath.
      //!  Note this will try to match by extension first, and match by
      //!  regular expression only if we don't find a match by extension.
      //----------------------------------------------------------------------
      const Language *PathToLanguage(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Returns a lowercase version of @c s.
      //----------------------------------------------------------------------
      std::string tolower(std::string s);

    }  // namespace CodeUtils
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMCODEUTILS_HH_
