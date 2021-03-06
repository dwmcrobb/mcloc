//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocHtmlFileScanner.hh 11838 $
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
//!  \file DwmMclocHtmlFileScanner.hh
//!  \brief Dwm::Mcloc::HtmlFileScanner class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCHTMLFILESCANNER_HH_
#define _DWMMCLOCHTMLFILESCANNER_HH_

#include "DwmMclocFileScanner.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Thin wrapper around our C/C++ lexical scanner.  This is used for
    //!  more than just C and C++, since several other language sources use
    //!  the same comment syntax.
    //------------------------------------------------------------------------
    class HtmlFileScanner
      : public FileScanner
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      HtmlFileScanner()
          : FileScanner(), _commentStyle("HTML")
      {}
      
      //----------------------------------------------------------------------
      //!  Scans the file at the given @c path and fills @c sfi with
      //!  the results.
      //----------------------------------------------------------------------
      void Scan(const std::filesystem::path & fspath,
                SourceFileInfo & sfi) const override;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      const std::string & CommentStyle() const override
      {
        return _commentStyle;
      }

    private:
      const std::string  _commentStyle;
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCHTMLFILESCANNER_HH_
