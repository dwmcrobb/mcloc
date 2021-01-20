//===========================================================================
// @(#) $DwmPath$
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
//!  \file TestCocomoConfig.cc
//!  \author Daniel W. McRobb
//!  \brief Unit tests for Dwm::Mcloc::Cocomo1::Intermediate::Config
//---------------------------------------------------------------------------

#include <iostream>
#include <sstream>

#include "MclocUnitAssert.hh"
#include "DwmMclocCocomo1IntermediateConfig.hh"

using namespace std;

//----------------------------------------------------------------------------
//!  Test config with all nominal values.
//----------------------------------------------------------------------------
void Test1()
{
  Intermediate::Config  config;
  UnitAssert(config.Parse("inputs/cocomo_test1.cfg"));
  UnitAssert(config.EAF() == 1.0);
  UnitAssert(config.ProjectClass() == Intermediate::e_organic);
  return;
}

//----------------------------------------------------------------------------
//!  Test config with non-nominal personnel values.
//----------------------------------------------------------------------------
void Test2()
{
  using namespace Dwm::Mcloc::Cocomo1;
  Intermediate::Config  config;
  UnitAssert(config.Parse("inputs/cocomo_test2.cfg"));
  UnitAssert(config.Product().EffortMultiplier() == 1);
  UnitAssert(config.Hardware().EffortMultiplier() == 1);
  UnitAssert(abs(config.Personnel().EffortMultiplier() - 0.348447) < .000001);
  UnitAssert(config.Project().EffortMultiplier() == 1);
  UnitAssert(abs(config.EAF() - 0.348447) < .000001);
  UnitAssert(config.CoefficientA() == 2.4);
  UnitAssert(config.ExponentB() == 1.05);
  UnitAssert(config.ExponentC() == 0.38);

  std::stringstream  ss;
  ss << config;
  ss.seekg(0);
  Intermediate::Config  config2;
  UnitAssert(config2.Parse(ss));
  UnitAssert(config2.EAF() == config.EAF());
}

//----------------------------------------------------------------------------
//!  Test config with non-nominal product values.
//----------------------------------------------------------------------------
void Test3()
{
  using namespace Dwm::Mcloc::Cocomo1;
  Intermediate::Config  config;
  UnitAssert(config.Parse("inputs/cocomo_test3.cfg"));
  UnitAssert(abs(config.Product().EffortMultiplier() - 2.67960) < 0.000001);
  UnitAssert(config.Hardware().EffortMultiplier() == 1);
  UnitAssert(config.Personnel().EffortMultiplier() == 1);
  UnitAssert(config.Project().EffortMultiplier() == 1);
  UnitAssert(abs(config.EAF() - 2.67960) < .000001);
  UnitAssert(config.CoefficientA() == 3.0);
  UnitAssert(config.ExponentB() == 1.12);
  UnitAssert(config.ExponentC() == 0.35);

  std::stringstream  ss;
  ss << config;
  ss.seekg(0);
  Intermediate::Config  config2;
  UnitAssert(config2.Parse(ss));
  UnitAssert(config2.EAF() == config.EAF());
}

//----------------------------------------------------------------------------
//!  Test config with non-nominal hardware values.
//----------------------------------------------------------------------------
void Test4()
{
  using namespace Dwm::Mcloc::Cocomo1;
  Intermediate::Config  config;
  UnitAssert(config.Parse("inputs/cocomo_test4.cfg"));
  UnitAssert(config.Product().EffortMultiplier() == 1);
  UnitAssert(abs(config.Hardware().EffortMultiplier() - 3.871452) < 0.0000001);
  UnitAssert(config.Personnel().EffortMultiplier() == 1);
  UnitAssert(config.Project().EffortMultiplier() == 1);
  UnitAssert(abs(config.EAF() - 3.871452) < .0000001);
  UnitAssert(config.CoefficientA() == 3.6);
  UnitAssert(config.ExponentB() == 1.20);
  UnitAssert(config.ExponentC() == 0.32);

  std::stringstream  ss;
  ss << config;
  ss.seekg(0);
  Intermediate::Config  config2;
  UnitAssert(config2.Parse(ss));
  UnitAssert(config2.EAF() == config.EAF());
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  using Mcloc::Assertions;
  using namespace Dwm::Mcloc::Cocomo1;

  Test1();
  Test2();
  Test3();
  Test4();
  
  if (Assertions::Total().Failed()) {
    Assertions::Print(cerr, true);
    return(1);
  }
  else {
    cout << Assertions::Total() << " passed" << endl;
  }
  return 0;
}
