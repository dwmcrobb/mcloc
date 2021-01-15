//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocTerminalTricks.cc 11829 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2005
//  All rights reserved.
//
//  Permission to use, copy, modify and distribute any part of this
//  software for educational, research and non-profit purposes,
//  without fee, and without a written agreement is hereby granted,
//  provided that the above copyright notice, this paragraph and the
//  following paragraphs appear in all copies.
//
//  Those desiring to incorporate this into commercial products or use
//  for commercial purposes should contact:
//
//    Daniel W. McRobb
//    dwm@mcplex.net
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
//!  \file DwmMclocTerminalTricks.cc
//!  \brief Dwm::Mcloc::TerminalTricks implementation
//---------------------------------------------------------------------------

extern "C" {
  #include <curses.h>
  #include <sys/ioctl.h>
  #include <term.h>
  #include <termcap.h>
}

#include <cstring>
#include <iostream>

#include "DwmMclocTerminalTricks.hh"

using namespace std;

static const string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocTerminalTricks.cc 11829 $");

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static int outc(int c)
{
  cout << (char)c;
  return(c);
}

namespace Dwm {
  
  namespace Mcloc {
    
    //--------------------------------------------------------------------
    //!  
    //--------------------------------------------------------------------
    TerminalTricks::TerminalTricks()
        : _on(true), _rows(0), _columns(0)
    {
      char   termBuf[4096];
      char  *termCapability = (char *)malloc(4096);
      char  *termCapPtr = termCapability;
      
      int termEntry = tgetent(termBuf, getenv("TERM"));

      char  us[3] = "us";
      char  *idstr = tgetstr(us, &termCapability);
      if (idstr) { _startUnderscore = idstr; }
      memset(termCapPtr, 0, 4096);
      termCapability = termCapPtr;
      char  ue[3] = "ue";
      idstr = tgetstr(ue, &termCapability);
      if (idstr) { _endUnderscore = idstr; }
      memset(termCapPtr, 0, 4096);
      termCapability = termCapPtr;
      char  md[3] = "md";
      idstr = tgetstr(md, &termCapability);
      if (idstr) { _startBold = idstr; }
      memset(termCapPtr, 0, 4096);
      termCapability = termCapPtr;
      char  me[3] = "me";
      idstr = tgetstr(me, &termCapability);
      if (idstr) { _endBold = idstr; }
      struct winsize  ws;
      if (ioctl(0, TIOCGWINSZ, &ws) != -1) {
        _columns =  ws.ws_col;
        _rows = ws.ws_row;
      }
      
      free(termCapPtr);
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void TerminalTricks::On(bool on)
    {
      _on = on;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    int TerminalTricks::Columns() const
    {
      return _columns;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    int TerminalTricks::Rows() const
    {
      return _rows;
    }
    
  }  // namespace Mcloc

}  // namespace Dwm
