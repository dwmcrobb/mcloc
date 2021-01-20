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
//!  \file DwmMclocCocomo1IntermediateConfig.cc
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::Cocomo1::Intermediate::Config class implementation
//---------------------------------------------------------------------------

#include "DwmMclocCocomo1IntermediateConfig.hh"

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        static std::string RatingName(Rating r)
        {
          static const std::map<Rating,std::string>  ratingNames = {
            { e_veryLow,   "verylow"   },
            { e_low,       "low"       },
            { e_nominal,   "nominal"   },
            { e_high,      "high"      },
            { e_veryHigh,  "veryhigh"  },
            { e_extraHigh, "extrahigh" }
          };
          std::string  rc;
          auto  it = ratingNames.find(r);
          if (it != ratingNames.end()) {
            rc = it->second;
          }
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        static std::string ProjectClassName(ProjectClassEnum pcName)
        {
          static const std::map<ProjectClassEnum, std::string>  pcNames = {
            { e_organic,      "organic"       },
            { e_semiDetached, "semi-detached" },
            { e_embedded,     "embedded"      }
          };
          std::string  rc;
          auto  it = pcNames.find(pcName);
          if (it != pcNames.end()) {
            rc = it->second;
          }
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProductConfig::ProductConfig()
            : _requiredReliability(e_nominal),
              _sizeOfApplicationDatabase(e_nominal),
              _complexityOfProduct(e_nominal)
        {}
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::RequiredReliability() const
        {
          return _requiredReliability;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::RequiredReliability(Rating r)
        {
          _requiredReliability = r;
          return _requiredReliability;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::SizeOfApplicationDatabase() const
        {
          return _sizeOfApplicationDatabase;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::SizeOfApplicationDatabase(Rating r)
        {
          _sizeOfApplicationDatabase = r;
          return _sizeOfApplicationDatabase;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::ComplexityOfProduct() const
        {
          return _complexityOfProduct;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProductConfig::ComplexityOfProduct(Rating r)
        {
          _complexityOfProduct = r;
          return _complexityOfProduct;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProductConfig::EffortMultiplier() const
        {
          using PV = ProductValues;
          double  rc = PV::RequiredReliability(_requiredReliability);
          rc *= PV::ApplicationDBSize(_sizeOfApplicationDatabase);
          rc *= PV::ProductComplexity(_complexityOfProduct);
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        std::ostream & operator << (std::ostream & os,
                                    const ProductConfig & pc)
        {
          os << "#-----------------------------------------------------------"
             << "----------------\n"
             << "#!  Product attributes\n"
             << "#-----------------------------------------------------------"
             << "----------------\n"
             << "Product {\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Required software reliability: "
             << RatingName(pc._requiredReliability) << ";\n"
             << "  #  can be low, nominal, high or veryhigh\n"
             << "  Size of application database: "
             << RatingName(pc._sizeOfApplicationDatabase) << ";\n"
             << "  #  can be verylow, low, nominal, high, veryhigh or extrahigh\n"
             << "  Complexity of the product: "
             << RatingName(pc._complexityOfProduct) << ";\n"
             << "}\n";
          return os;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        HardwareConfig::HardwareConfig()
            : _runtimePerformanceConstraints(e_nominal),
              _memoryContraints(e_nominal),
              _volatilityOfVMEnvironment(e_nominal),
              _requiredTurnaboutTime(e_nominal)
        {}
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::RuntimePerformanceConstraints() const
        {
          return _runtimePerformanceConstraints;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::RuntimePerformanceConstraints(Rating r)
        {
          _runtimePerformanceConstraints = r;
          return _runtimePerformanceConstraints;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::MemoryContraints() const
        {
          return _memoryContraints;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::MemoryContraints(Rating r)
        {
          _memoryContraints = r;
          return _memoryContraints;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::VolatilityOfVMEnvironment() const
        {
          return _volatilityOfVMEnvironment;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::VolatilityOfVMEnvironment(Rating r)
        {
          _volatilityOfVMEnvironment = r;
          return _volatilityOfVMEnvironment;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::RequiredTurnaboutTime() const
        {
          return _requiredTurnaboutTime;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating HardwareConfig::RequiredTurnaboutTime(Rating r)
        {
          _requiredTurnaboutTime = r;
          return _requiredTurnaboutTime;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double HardwareConfig::EffortMultiplier() const
        {
          using HV = HardwareValues;
          double  rc = HV::RuntimePerformanceConstraints(_runtimePerformanceConstraints);
          rc *= HV::MemoryConstraints(_memoryContraints);
          rc *= HV::VolatilityOfVMEnvironment(_volatilityOfVMEnvironment);
          rc *= HV::RequiredTurnaboutTime(_requiredTurnaboutTime);
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        std::ostream &
        operator << (std::ostream & os, const HardwareConfig & hc)
        {
          os << "#-----------------------------------------------------------"
             << "----------------\n"
             << "#!  Hardware attributes\n"
             << "#-----------------------------------------------------------"
             << "----------------\n"
             << "Hardware {\n"
             << "  #  can be nominal, high, veryhigh or extrahigh\n"
             << "  Runtime performance constraints: "
             << RatingName(hc._runtimePerformanceConstraints) << ";\n"
             << "  #  can be nominal, high, veryhigh or extrahigh\n"
             << "  Memory constraints: "
             << RatingName(hc._memoryContraints) << ";\n"
             << "  #  can be low, nominal, high or veryhigh\n"
             << "  Volatility of the virtual machine environment: "
             << RatingName(hc._volatilityOfVMEnvironment) << ";\n"
             << "  #  can be low, nominal, high or veryhigh\n"
             << "  Required turnabout time: "
             << RatingName(hc._requiredTurnaboutTime) << ";\n"
             << "}\n";
          return os;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        PersonnelConfig::PersonnelConfig()
            : _analystCapability(e_nominal),
              _applicationsExperience(e_nominal),
              _softwareEngineerCapability(e_nominal),
              _virtualMachineExperience(e_nominal),
              _programmingLanguageExperience(e_nominal)
        {}
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::AnalystCapability() const
        {
          return _analystCapability;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::AnalystCapability(Rating r)
        {
          _analystCapability = r;
          return _analystCapability;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::ApplicationsExperience() const
        {
          return _applicationsExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::ApplicationsExperience(Rating r)
        {
          _applicationsExperience = r;
          return _applicationsExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::SoftwareEngineerCapability() const
        {
          return _softwareEngineerCapability;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::SoftwareEngineerCapability(Rating r)
        {
          _softwareEngineerCapability = r;
          return _softwareEngineerCapability;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::VirtualMachineExperience() const
        {
          return _virtualMachineExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::VirtualMachineExperience(Rating r)
        {
          _virtualMachineExperience = r;
          return _virtualMachineExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::ProgrammingLanguageExperience() const
        {
          return _programmingLanguageExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating PersonnelConfig::ProgrammingLanguageExperience(Rating r)
        {
          _programmingLanguageExperience = r;
          return _programmingLanguageExperience;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double PersonnelConfig::EffortMultiplier() const
        {
          using PV = PersonnelValues;
          double  rc = PV::AnalystCapability(_analystCapability);
          rc *= PV::ApplicationsExperience(_applicationsExperience);
          rc *= PV::SoftwareEngineerCapability(_softwareEngineerCapability);
          rc *= PV::VirtualMachineExperience(_virtualMachineExperience);
          rc *= PV::ProgrammingLanguageExperience(_programmingLanguageExperience);
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        std::ostream &
        operator << (std::ostream & os, const PersonnelConfig & pc)
        {
          os << "#-----------------------------------------------------------"
             << "----------------\n"
             << "#!  Personnel attributes\n"
             << "#-----------------------------------------------------------"
             << "----------------\n"
             << "Personnel {\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Analyst capability: "
             << RatingName(pc._analystCapability) << ";\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Applications experience: "
             << RatingName(pc._applicationsExperience) << ";\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Software engineer capability: \n"
             << RatingName(pc._softwareEngineerCapability) << ";\n"
             << "  #  can be verylow, low, nominal or high\n"
             << "  Virtual machine experience: "
             << RatingName(pc._virtualMachineExperience) << ";\n"
             << "  #  can be verylow, low, nominal or high\n"
             << "  Programming language experience: "
             << RatingName(pc._programmingLanguageExperience) << ";\n"
             << "}\n";
          return os;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProjectConfig::ProjectConfig()
            : _applicationOfSWEngineeringMethods(e_nominal),
              _useOfSoftwareTools(e_nominal),
              _requiredDevelopmentSchedule(e_nominal)
        {}
          
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::ApplicationOfSWEngineeringMethods() const
        {
          return _applicationOfSWEngineeringMethods;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::ApplicationOfSWEngineeringMethods(Rating r)
        {
          _applicationOfSWEngineeringMethods = r;
          return _applicationOfSWEngineeringMethods;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::UseOfSoftwareTools() const
        {
          return _useOfSoftwareTools;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::UseOfSoftwareTools(Rating r)
        {
          _useOfSoftwareTools = r;
          return _useOfSoftwareTools;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::RequiredDevelopmentSchedule() const
        {
          return _requiredDevelopmentSchedule;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Rating ProjectConfig::RequiredDevelopmentSchedule(Rating r)
        {
          _requiredDevelopmentSchedule = r;
          return _requiredDevelopmentSchedule;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double ProjectConfig::EffortMultiplier() const
        {
          using PV = ProjectValues;
          double  rc = PV::ApplicationOfSWEngineeringMethods(_applicationOfSWEngineeringMethods);
          rc *= PV::UseOfSoftwareTools(_useOfSoftwareTools);
          rc *= PV::RequiredDevelopmentSchedule(_requiredDevelopmentSchedule);
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        std::ostream &
        operator << (std::ostream & os, const ProjectConfig & pc)
        {
          os << "#-----------------------------------------------------------"
             << "----------------\n"
             << "#!  Project attributes\n"
             << "#-----------------------------------------------------------"
             << "----------------\n"
             << "Project {\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Application of software engineering methods: "
             << RatingName(pc._applicationOfSWEngineeringMethods) << ";\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Use of software tools: "
             << RatingName(pc._useOfSoftwareTools) << ";\n"
             << "  #  can be verylow, low, nominal, high or veryhigh\n"
             << "  Required development schedule: "
             << RatingName(pc._requiredDevelopmentSchedule) << ";\n"
             << "}\n";
          return os;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        Config::Config()
            : _projectClass(e_organic), _product(), _hardware(),
              _personnel(), _project()
        {}

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProjectClassEnum Config::ProjectClass() const
        {
          return _projectClass;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProjectClassEnum Config::ProjectClass(ProjectClassEnum projectClass)
        {
          _projectClass = projectClass;
          return _projectClass;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const ProductConfig & Config::Product() const
        {
          return _product;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProductConfig & Config::Product()
        {
          return _product;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const ProductConfig & Config::Product(const ProductConfig & product)
        {
          _product = product;
          return _product;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const HardwareConfig & Config::Hardware() const
        {
          return _hardware;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        HardwareConfig & Config::Hardware()
        {
          return _hardware;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const HardwareConfig &
        Config::Hardware(const HardwareConfig & hardware)
        {
          _hardware = hardware;
          return _hardware;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const PersonnelConfig & Config::Personnel() const
        {
          return _personnel;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        PersonnelConfig & Config::Personnel()
        {
          return _personnel;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const PersonnelConfig &
        Config::Personnel(const PersonnelConfig & personnel)
        {
          _personnel = personnel;
          return _personnel;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const ProjectConfig & Config::Project() const
        {
          return _project;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        ProjectConfig & Config::Project()
        {
          return _project;
        }
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        const ProjectConfig & Config::Project(const ProjectConfig & project)
        {
          _project = project;
          return _project;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        std::ostream &
        operator << (std::ostream & os, const Config & cfg)
        {
          os << "#-----------------------------------------------------------"
             << "----------------\n"
             << "#!  Can be 'organic', 'semi-detached' or 'embeddded'\n"
             << "#-----------------------------------------------------------"
             << "----------------\n"
             << "Project class: "
             << ProjectClassName(cfg._projectClass) << ";\n\n"
             << cfg._product << '\n'
             << cfg._hardware << '\n'
             << cfg._personnel << '\n'
             << cfg._project << '\n';
          return os;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double Config::EAF() const
        {
          double  rc = _product.EffortMultiplier();
          rc *= _hardware.EffortMultiplier();
          rc *= _personnel.EffortMultiplier();
          rc *= _project.EffortMultiplier();
          return rc;
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double Config::CoefficientA() const
        {
          return ProjectClassValues::CoefficientA(_projectClass);
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double Config::ExponentB() const
        {
          return ProjectClassValues::ExponentB(_projectClass);
        }

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        double Config::ExponentC() const
        {
          return ProjectClassValues::ExponentC(_projectClass);
        }
        
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm
