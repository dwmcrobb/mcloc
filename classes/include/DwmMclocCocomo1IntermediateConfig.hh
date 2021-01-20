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
//!  \file DwmMclocCocomo1IntermediateConfig.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::Cocomo1::Intermedicate::Config class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCOCOMO1INTERMEDIATECONFIG_HH_
#define _DWMMCLOCCOCOMO1INTERMEDIATECONFIG_HH_

#include <iostream>
#include <string>

#include "DwmMclocCocomo1IntermediateValues.hh"

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class ProductConfig
        {
        public:
          ProductConfig();
          Rating RequiredReliability() const;
          Rating RequiredReliability(Rating r);
          Rating SizeOfApplicationDatabase() const;
          Rating SizeOfApplicationDatabase(Rating r);
          Rating ComplexityOfProduct() const;
          Rating ComplexityOfProduct(Rating r);
          double EffortMultiplier() const;

          friend std::ostream &
          operator << (std::ostream & os, const ProductConfig & pc);
          
        private:
          Rating  _requiredReliability;
          Rating  _sizeOfApplicationDatabase;
          Rating  _complexityOfProduct;
        };
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class HardwareConfig
        {
        public:
          HardwareConfig();
          Rating RuntimePerformanceConstraints() const;
          Rating RuntimePerformanceConstraints(Rating r);
          Rating MemoryContraints() const;
          Rating MemoryContraints(Rating r);
          Rating VolatilityOfVMEnvironment() const;
          Rating VolatilityOfVMEnvironment(Rating r);
          Rating RequiredTurnaboutTime() const;
          Rating RequiredTurnaboutTime(Rating r);
          double EffortMultiplier() const;

          friend std::ostream &
          operator << (std::ostream & os, const HardwareConfig & hc);
          
        private:
          Rating  _runtimePerformanceConstraints;
          Rating  _memoryContraints;
          Rating  _volatilityOfVMEnvironment;
          Rating  _requiredTurnaboutTime;
        };
          
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class PersonnelConfig
        {
        public:
          PersonnelConfig();
          Rating AnalystCapability() const;
          Rating AnalystCapability(Rating r);
          Rating ApplicationsExperience() const;
          Rating ApplicationsExperience(Rating r);
          Rating SoftwareEngineerCapability() const;
          Rating SoftwareEngineerCapability(Rating r);
          Rating VirtualMachineExperience() const;
          Rating VirtualMachineExperience(Rating r);
          Rating ProgrammingLanguageExperience() const;
          Rating ProgrammingLanguageExperience(Rating r);
          double EffortMultiplier() const;

          friend std::ostream &
          operator << (std::ostream & os, const PersonnelConfig & pc);
          
        private:
          Rating  _analystCapability;
          Rating  _applicationsExperience;
          Rating  _softwareEngineerCapability;
          Rating  _virtualMachineExperience;
          Rating  _programmingLanguageExperience;
        };
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class ProjectConfig
        {
        public:
          ProjectConfig();
          Rating ApplicationOfSWEngineeringMethods() const;
          Rating ApplicationOfSWEngineeringMethods(Rating r);
          Rating UseOfSoftwareTools() const;
          Rating UseOfSoftwareTools(Rating r);
          Rating RequiredDevelopmentSchedule() const;
          Rating RequiredDevelopmentSchedule(Rating r);
          double EffortMultiplier() const;

          friend std::ostream &
          operator << (std::ostream & os, const ProjectConfig & pc);
          
        private:
          Rating  _applicationOfSWEngineeringMethods;
          Rating  _useOfSoftwareTools;
          Rating  _requiredDevelopmentSchedule;
        };

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        class Config
        {
        public:
          Config();
          bool Parse(const std::string & path);
          bool Parse(std::istream & is);
          ProjectClassEnum ProjectClass() const;
          ProjectClassEnum ProjectClass(ProjectClassEnum projectClass);
          const ProductConfig & Product() const;
          ProductConfig & Product();
          const ProductConfig & Product(const ProductConfig & product);
          const HardwareConfig & Hardware() const;
          HardwareConfig & Hardware();
          const HardwareConfig & Hardware(const HardwareConfig & hardware);
          const PersonnelConfig & Personnel() const;
          PersonnelConfig & Personnel();
          const PersonnelConfig & Personnel(const PersonnelConfig & personnel);
          const ProjectConfig & Project() const;
          ProjectConfig & Project();
          const ProjectConfig & Project(const ProjectConfig & project);

          friend std::ostream &
          operator << (std::ostream & os, const Config & cfg);
          
          double EAF() const;
          double CoefficientA() const;
          double ExponentB() const;
          double ExponentC() const;
          
        private:
          ProjectClassEnum  _projectClass;
          ProductConfig     _product;
          HardwareConfig    _hardware;
          PersonnelConfig   _personnel;
          ProjectConfig     _project;
        };
        
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCOCOMO1INTERMEDIATECONFIG_HH_
