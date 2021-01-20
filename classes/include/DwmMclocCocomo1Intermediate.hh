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
//!  \file DwmMclocCocomo1Intermediate.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::Cocomo1::Intermediate calculation declarations
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCOCOMO1INTERMEDIATE_HH_
#define _DWMMCLOCCOCOMO1INTERMEDIATE_HH_

#include "DwmMclocCocomo1IntermediateConfig.hh"

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double Effort(const Config & config, uint64_t loc);

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double DevelopmentTime(const Config & config, uint64_t loc);

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        inline double Persons(double effort, double devTime)
        { return (effort / devTime); }
          
        
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCOCOMO1INTERMEDIATE_HH_
