//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/tests/TestLuaScanner.cc 11843 $
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
//!  \file TestLuaScanner.cc
//!  \author Daniel W. McRobb
//!  \brief Unit tests for Lua scanner
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
    { "inputs/lua_001.lua", { 10, 1 } },
    { "inputs/lua_002.lua", {  7, 6 } }
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
