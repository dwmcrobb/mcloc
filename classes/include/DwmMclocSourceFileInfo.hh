//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocSourceFileInfo.hh 11828 $
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
//!  \file DwmMclocSourceFileInfo.hh
//!  \brief Dwm::Mcloc::SourceFileInfo class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCSOURCEFILEINFO_HH_
#define _DWMMCLOCSOURCEFILEINFO_HH_

#include <filesystem>
#include <set>

#include "DwmMclocCodeCounter.hh"
#include "DwmMclocCodeUtils.hh"

extern int yylex(void *yyscanner);

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Encapsulate some information for a single source file.
    //------------------------------------------------------------------------
    class SourceFileInfo
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor
      //----------------------------------------------------------------------
      SourceFileInfo() = default;
      
      //----------------------------------------------------------------------
      //!  Returns true if the source file is a generated file (output from
      //!  flex or bison).
      //----------------------------------------------------------------------
      inline bool IsGenerated() const  { return _isGenerated; }

      //----------------------------------------------------------------------
      //!  Sets whether or not the source file was generated by flex or bison.
      //!  Called from the FileScanner.
      //----------------------------------------------------------------------
      inline bool IsGenerated(bool isGenerated)
      {
        _isGenerated = isGenerated;
        return _isGenerated;
      }

      //----------------------------------------------------------------------
      //!  If the source file was generated by flex or bison, returns the
      //!  filename of the generator source.
      //----------------------------------------------------------------------
      inline std::filesystem::path GeneratedBy() const
      {
        return _generatedBy;
      }

      //----------------------------------------------------------------------
      //!  Sets the path of the generator source that generated the source
      //!  file.  Called from the FileScanner.
      //----------------------------------------------------------------------
      inline void GeneratedBy(const std::filesystem::path & fspath)
      {
#if 0
        if (CodeUtils::IsGenerator(fspath)) {
          _generatedBy = fspath;
        }
#endif
        _generatedBy = fspath;
        return;
      }

      //----------------------------------------------------------------------
      //!  Returns a const reference to the source file code counter.
      //----------------------------------------------------------------------
      inline const CodeCounter & Counter() const  { return _counter; }

      //----------------------------------------------------------------------
      //!  Returns a mutable reference to the source file code counter.
      //----------------------------------------------------------------------
      inline CodeCounter & Counter()  { return _counter; }

    private:
      bool                   _isGenerated;
      std::filesystem::path  _generatedBy;
      CodeCounter            _counter;
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCSOURCEFILEINFO_HH_
