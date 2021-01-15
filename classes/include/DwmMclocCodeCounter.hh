//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocCodeCounter.hh 11828 $
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
//!  \file DwmMclocCodeCounter.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::CodeCounter class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCODECOUNTER_HH_
#define _DWMMCLOCCODECOUNTER_HH_

#include <atomic>
#include <iomanip>
#include <iostream>
#include <memory>

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Encapsulate atomic counters for lines of code and comments.
    //------------------------------------------------------------------------
    class CodeCounter
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      CodeCounter() : _codeLines(0), _commentLines(0)  { }
      
      //----------------------------------------------------------------------
      //!  Copy constructor.
      //----------------------------------------------------------------------
      CodeCounter(const CodeCounter & cc)
      {
        _codeLines = cc._codeLines.load();
        _commentLines = cc._commentLines.load();
      }

      //----------------------------------------------------------------------
      //!  Copy assignment operator.
      //----------------------------------------------------------------------
      CodeCounter & operator = (const CodeCounter & cc)
      {
        if (this != &cc) {
        _codeLines = cc._codeLines.load();
        _commentLines = cc._commentLines.load();
        }
        return *this;
      }
      
      //----------------------------------------------------------------------
      //!  Increment the lines of code by @c codeLines and the lines of
      //!  comments by @c commentLines.
      //----------------------------------------------------------------------
      void Increment(size_t codeLines, size_t commentLines = 0)
      {
        _codeLines += codeLines;
        _commentLines += commentLines;
        return;
      }
      
      //----------------------------------------------------------------------
      //!  Increment the lines of code by 1.
      //----------------------------------------------------------------------
      inline void IncrementCodeLines()
      {
        ++_codeLines;
      }
      
      //----------------------------------------------------------------------
      //!  Increment the lines of comments by 1.
      //----------------------------------------------------------------------
      inline void IncrementCommentLines()
      {
        ++_commentLines;
      }
      
      //----------------------------------------------------------------------
      //!  Add @c cc to the counters.
      //----------------------------------------------------------------------
      CodeCounter & operator += (const CodeCounter & cc)
      {
        _codeLines += cc._codeLines;
        _commentLines += cc._commentLines;
        return *this;
      }

      //----------------------------------------------------------------------
      //!  operator < compares only code lines, not comment lines.
      //----------------------------------------------------------------------
      inline bool operator < (const CodeCounter & cc) const
      {
        return (_codeLines < cc._codeLines);
      }

      //----------------------------------------------------------------------
      //!  operator > compares only code lines, not comment lines.
      //----------------------------------------------------------------------
      inline bool operator > (const CodeCounter & cc) const
      {
        return (_codeLines > cc._codeLines);
      }
      
      //----------------------------------------------------------------------
      //!  Returns the lines of code.
      //----------------------------------------------------------------------
      size_t CodeLines() const  { return _codeLines; }
      
      //----------------------------------------------------------------------
      //!  Returns the lines of comments.
      //----------------------------------------------------------------------
      size_t CommentLines() const  { return _commentLines; }
      
      //----------------------------------------------------------------------
      //!  Set the lines of code and lines of comments to 0.
      //----------------------------------------------------------------------
      void Reset()  { _codeLines = 0;  _commentLines = 0; }
      
      //----------------------------------------------------------------------
      //!  Print to an ostream.
      //----------------------------------------------------------------------
      friend std::ostream & operator << (std::ostream & os,
                                         const CodeCounter & cc)
      {
        os << std::setw(11) << cc._codeLines << ' '
           << std::setw(11) << cc._commentLines;
        return os;
      }
      
    private:
      std::atomic<size_t>   _codeLines;
      std::atomic<size_t>   _commentLines;
    };
    
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCODECOUNTER_HH_
