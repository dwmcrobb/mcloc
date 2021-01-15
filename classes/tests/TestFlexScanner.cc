//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/tests/TestFlexScanner.cc 11843 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2021
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
//!  \file TestFlexScanner.cc
//!  \author Daniel W. McRobb
//!  \brief Unit tests for flex scanner
//---------------------------------------------------------------------------

#include "DwmMclocSourceCollection.hh"
#include "DwmMclocSingleFileTests.hh"
#include "MclocUnitAssert.hh"

using namespace std;

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  using Mcloc::Assertions;

  static const map<string,pair<uint64_t,uint64_t>>  files = {
    { "inputs/flex_001.lex", { 11, 8 } },
    { "inputs/flex_002.lex", { 15, 12 } },
    { "inputs/flex_002.lex", { 18, 12 } }
  };

  Dwm::Mcloc::SingleFileTests(files);

  if (Assertions::Total().Failed()) {
    Assertions::Print(cerr, true);
    return(1);
  }
  else {
    cout << Assertions::Total() << " passed" << endl;
  }
  return 0;
}
