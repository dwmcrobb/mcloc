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
//!  \file DwmMclocCocomo1IntermediateValues.cc
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::Cocomo1::Intermediate values implementations
//---------------------------------------------------------------------------

#include <cassert>

#include "DwmMclocCocomo1IntermediateValues.hh"

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        using namespace std;

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        static double
        GetValueByRating(const map<Rating,double> & values, Rating r)
        {
          double  rc = 1.0;
          auto  it = values.find(r);
          if (it != values.end()) {
            rc = it->second;
          }
          return rc;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double> ProductValues::requiredSoftwareReliability = {
          { e_veryLow,    0.75 },
          { e_low,        0.88 },
          { e_nominal,    1.00 },
          { e_high,       1.15 },
          { e_veryHigh,   1.40 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProductValues::RequiredReliability(Rating r)
        { return GetValueByRating(requiredSoftwareReliability, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double> ProductValues::sizeOfApplicationDatabase = {
          { e_low,        0.94 },
          { e_nominal,    1.00 },
          { e_high,       1.08 },
          { e_veryHigh,   1.16 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProductValues::ApplicationDBSize(Rating r)
        { return GetValueByRating(sizeOfApplicationDatabase, r); }
          
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double> ProductValues::complexityOfTheProduct = {
          { e_veryLow,     0.70 },
          { e_low,         0.85 },
          { e_nominal,     1.00 },
          { e_high,        1.15 },
          { e_veryHigh,    1.30 },
          { e_extraHigh,   1.65 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProductValues::ProductComplexity(Rating r)
        { return GetValueByRating(complexityOfTheProduct, r); }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  HardwareValues::runtimePerformanceConstraints = {
          { e_nominal,     1.00 },
          { e_high,        1.11 },
          { e_veryHigh,    1.30 },
          { e_extraHigh,   1.66 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double HardwareValues::RuntimePerformanceConstraints(Rating r)
        { return GetValueByRating(runtimePerformanceConstraints, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  HardwareValues::memoryConstraints = {
          { e_nominal,     1.00 },
          { e_high,        1.06 },
          { e_veryHigh,    1.21 },
          { e_extraHigh,   1.56 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double HardwareValues::MemoryConstraints(Rating r)
        { return GetValueByRating(memoryConstraints, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  HardwareValues::volatilityOfVMEnvironment = {
          { e_low,         0.87 },
          { e_nominal,     1.00 },
          { e_high,        1.15 },
          { e_veryHigh,    1.30 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double HardwareValues::VolatilityOfVMEnvironment(Rating r)
        { return GetValueByRating(volatilityOfVMEnvironment, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  HardwareValues::requiredTurnaboutTime = {
          { e_low,         0.87 },
          { e_nominal,     1.00 },
          { e_high,        1.07 },
          { e_veryHigh,    1.15 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double HardwareValues::RequiredTurnaboutTime(Rating r)
        { return GetValueByRating(requiredTurnaboutTime, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  PersonnelValues::analystCapability = {
          { e_veryLow,     1.46 },
          { e_low,         1.19 },
          { e_nominal,     1.00 },
          { e_high,        0.86 },
          { e_veryHigh,    0.71 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelValues::AnalystCapability(Rating r)
        { return GetValueByRating(analystCapability, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  PersonnelValues::applicationsExperience = {
          { e_veryLow,     1.29 },
          { e_low,         1.13 },
          { e_nominal,     1.00 },
          { e_high,        0.91 },
          { e_veryHigh,    0.82 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelValues::ApplicationsExperience(Rating r)
        { return GetValueByRating(applicationsExperience, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  PersonnelValues::softwareEngineerCapability = {
          { e_veryLow,     1.42 },
          { e_low,         1.17 },
          { e_nominal,     1.00 },
          { e_high,        0.86 },
          { e_veryHigh,    0.70 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelValues::SoftwareEngineerCapability(Rating r)
        { return GetValueByRating(softwareEngineerCapability, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  PersonnelValues::virtualMachineExperience = {
          { e_veryLow,     1.21 },
          { e_low,         1.10 },
          { e_nominal,     1.00 },
          { e_high,        0.90 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelValues::VirtualMachineExperience(Rating r)
        { return GetValueByRating(virtualMachineExperience, r); }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  PersonnelValues::programmingLanguageExperience = {
          { e_veryLow,     1.14 },
          { e_low,         1.07 },
          { e_nominal,     1.00 },
          { e_high,        0.95 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelValues::ProgrammingLanguageExperience(Rating r)
        { return GetValueByRating(programmingLanguageExperience, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>
        ProjectValues::applicationOfSWEngineeringMethods = {
          { e_veryLow,     1.24 },
          { e_low,         1.10 },
          { e_nominal,     1.00 },
          { e_high,        0.91 },
          { e_veryHigh,    0.82 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectValues::ApplicationOfSWEngineeringMethods(Rating r)
        { return GetValueByRating(applicationOfSWEngineeringMethods, r); }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  ProjectValues::useOfSoftwareTools = {
          { e_veryLow,     1.24 },
          { e_low,         1.10 },
          { e_nominal,     1.00 },
          { e_high,        0.91 },
          { e_veryHigh,    0.83 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectValues::UseOfSoftwareTools(Rating r)
        { return GetValueByRating(useOfSoftwareTools, r); }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        map<Rating,double>  ProjectValues::requiredDevelopmentSchedule = {
          { e_veryLow,     1.23 },
          { e_low,         1.08 },
          { e_nominal,     1.00 },
          { e_high,        1.04 },
          { e_veryHigh,    1.10 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectValues::RequiredDevelopmentSchedule(Rating r)
        { return GetValueByRating(requiredDevelopmentSchedule, r); }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const map<ProjectClassEnum,double>
        ProjectClassValues::_coefficientA = {
          { e_organic,      2.4 },
          { e_semiDetached, 3.0 },
          { e_embedded,     3.6 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectClassValues::CoefficientA(ProjectClassEnum c)
        {
          auto  it = _coefficientA.find(c);
          assert(it != _coefficientA.end());
          return it->second;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const std::map<ProjectClassEnum,double>
        ProjectClassValues::_exponentB = {
          { e_organic,      1.05 },
          { e_semiDetached, 1.12 },
          { e_embedded,     1.20 }
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectClassValues::ExponentB(ProjectClassEnum c)
        {
          auto  it = _exponentB.find(c);
          assert(it != _exponentB.end());
          return it->second;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const std::map<ProjectClassEnum,double>
        ProjectClassValues::_exponentC = {
          { e_organic,      0.38 },
          { e_semiDetached, 0.35 },
          { e_embedded,     0.32 }
        };
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectClassValues::ExponentC(ProjectClassEnum c)
        {
          auto  it = _exponentC.find(c);
          assert(it != _exponentC.end());
          return it->second;
        }
        
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm


