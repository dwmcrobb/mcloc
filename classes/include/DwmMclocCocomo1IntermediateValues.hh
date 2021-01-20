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
//!  \file DwmMclocCocomo1IntermediateValues.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCOCOMO1INTERMEDIATEVALUES_HH_
#define _DWMMCLOCCOCOMO1INTERMEDIATEVALUES_HH_

#include <map>

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        typedef enum {
          e_veryLow,
          e_low,
          e_nominal,
          e_high,
          e_veryHigh,
          e_extraHigh
        } Rating;
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class ProductValues
        {
        public:
          static double RequiredReliability(Rating r);
          static double ApplicationDBSize(Rating r);
          static double ProductComplexity(Rating r);

        private:
          static std::map<Rating,double>  requiredSoftwareReliability;
          static std::map<Rating,double>  sizeOfApplicationDatabase;
          static std::map<Rating,double>  complexityOfTheProduct;
        };
      
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class HardwareValues
        {
        public:
          static double RuntimePerformanceConstraints(Rating r);
          static double MemoryConstraints(Rating r);
          static double VolatilityOfVMEnvironment(Rating r);
          static double RequiredTurnaboutTime(Rating r);
          
        private:
          static std::map<Rating,double>  runtimePerformanceConstraints;
          static std::map<Rating,double>  memoryConstraints;
          static std::map<Rating,double>  volatilityOfVMEnvironment;
          static std::map<Rating,double>  requiredTurnaboutTime;
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class PersonnelValues
        {
        public:
          static double AnalystCapability(Rating r);
          static double ApplicationsExperience(Rating r);
          static double SoftwareEngineerCapability(Rating r);
          static double VirtualMachineExperience(Rating r);
          static double ProgrammingLanguageExperience(Rating r);
          
        private:
          static std::map<Rating,double>  analystCapability;
          static std::map<Rating,double>  applicationsExperience;
          static std::map<Rating,double>  softwareEngineerCapability;
          static std::map<Rating,double>  virtualMachineExperience;
          static std::map<Rating,double>  programmingLanguageExperience;
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class ProjectValues
        {
        public:
          static double ApplicationOfSWEngineeringMethods(Rating r);
          static double UseOfSoftwareTools(Rating r);
          static double RequiredDevelopmentSchedule(Rating r);
          
        private:
          static std::map<Rating,double>  applicationOfSWEngineeringMethods;
          static std::map<Rating,double>  useOfSoftwareTools;
          static std::map<Rating,double>  requiredDevelopmentSchedule;
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        typedef enum {
          e_organic,
          e_semiDetached,
          e_embedded
        } ProjectClassEnum;

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class ProjectClassValues
        {
        public:
          static double CoefficientA(ProjectClassEnum c);
          static double ExponentB(ProjectClassEnum c);
          static double ExponentC(ProjectClassEnum c);

        private:
          static const std::map<ProjectClassEnum,double>  _coefficientA;
          static const std::map<ProjectClassEnum,double>  _exponentB;
          static const std::map<ProjectClassEnum,double>  _exponentC;
        };
        

      }  // namespace Intermediate

    }  // namespace Cocomo1
      
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCOCOMO1INTERMEDIATEVALUES_HH_
