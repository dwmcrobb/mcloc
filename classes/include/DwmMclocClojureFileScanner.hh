//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocClojureFileScanner.hh 11827 $
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
//!  \file DwmMclocClojureFileScanner.hh
//!  \brief Dwm::Mcloc::ClojureFileScanner class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCLOJUREFILESCANNER_HH_
#define _DWMMCLOCCLOJUREFILESCANNER_HH_

#include "DwmMclocFileScanner.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Thin wrapper around our Clojure lexical scanner.
    //------------------------------------------------------------------------
    class ClojureFileScanner
      : public FileScanner
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      ClojureFileScanner()
          : FileScanner(), _commentStyle("Clojure")
      {}
      
      //----------------------------------------------------------------------
      //!  Scans the file at the given @c path and fills @c sfi with
      //!  the results.
      //----------------------------------------------------------------------
      void Scan(const std::filesystem::path & fspath,
                SourceFileInfo & sfi) const override;

      //----------------------------------------------------------------------
      //!  Returns a string description of the comment style the scanner
      //!  expects.
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

#endif  // _DWMMCLOCCLOJUREFILESCANNER_HH_
