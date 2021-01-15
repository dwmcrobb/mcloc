//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocM4FileScanner.hh 11828 $
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
//!  \file DwmMclocM4FileScanner.hh
//!  \brief Dwm::Mcloc::M4FileScanner class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCM4FILESCANNER_HH_
#define _DWMMCLOCM4FILESCANNER_HH_

#include "DwmMclocFileScanner.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Thin wrapper around our m4 lexical scanner.  Note that we only
    //!  count non-echoing comments (dnl ...) as comments.  Echoing comments
    //!  are counted as code.
    //------------------------------------------------------------------------
    class M4FileScanner
      : public FileScanner
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      M4FileScanner()
          : FileScanner(), _commentStyle("m4")
      {}
      
      //----------------------------------------------------------------------
      //!  Scans the file at the given @c path and fills @c sfi with
      //!  the results.
      //----------------------------------------------------------------------
      void Scan(const std::filesystem::path & fspath,
                SourceFileInfo & sfi) const override;

      //----------------------------------------------------------------------
      //!  Returns the comment style name.
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

#endif  // _DWMMCLOCM4FILESCANNER_HH_
