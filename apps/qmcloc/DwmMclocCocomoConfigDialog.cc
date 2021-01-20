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
//!  \file DwmMclocCocomoConfigDialog.cc
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#include "DwmMclocCocomoConfigDialog.hh"

namespace Dwm {

  namespace Mcloc {

    using namespace std;
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    static QString RToComboText(Cocomo1::Intermediate::Rating r)
    {
      QString  rc;
      static const map<Cocomo1::Intermediate::Rating,string>  ratings = {
        { Cocomo1::Intermediate::e_veryLow,   "very low"   },
        { Cocomo1::Intermediate::e_low,       "low"        },
        { Cocomo1::Intermediate::e_nominal,   "nominal"    },
        { Cocomo1::Intermediate::e_high,      "high"       },
        { Cocomo1::Intermediate::e_veryHigh,  "very high"  },
        { Cocomo1::Intermediate::e_extraHigh, "extra high" }
      };
      auto  it = ratings.find(r);
      if (it != ratings.end()) {
        rc = it->second.c_str();
      }
      return rc;
    }
                                     
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    CocomoConfigDialog::
    CocomoConfigDialog(Cocomo1::Intermediate::Config *cocomoCfg,
                       QWidget *parent)
        : QDialog(parent), _cocomoCfg(cocomoCfg)
    {
      _ui.setupUi(this);
      InitFromConfig();
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CocomoConfigDialog::InitFromConfig()
    {
      static const map<Cocomo1::Intermediate::ProjectClassEnum,string> cs = {
        { Cocomo1::Intermediate::e_organic,      "organic"       },
        { Cocomo1::Intermediate::e_semiDetached, "semi-detached" },
        { Cocomo1::Intermediate::e_embedded,     "embedded"      }
      };
      auto  it = cs.find(_cocomoCfg->ProjectClass());
      if (it != cs.end()) {
        _ui.projectClassComboBox->setCurrentText(it->second.c_str());
      }
      
      InitProductGroupFromConfig();
      InitHardwareGroupFromConfig();
      InitPersonnelGroupFromConfig();
      InitProjectGroupFromConfig();
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CocomoConfigDialog::InitProductGroupFromConfig()
    {
      auto & p = _cocomoCfg->Product();
      _ui.relyComboBox->setCurrentText(RToComboText(p.RequiredReliability()));
      _ui.dataComboBox->setCurrentText(RToComboText(p.SizeOfApplicationDatabase()));
      _ui.cplxComboBox->setCurrentText(RToComboText(p.ComplexityOfProduct()));
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CocomoConfigDialog::InitHardwareGroupFromConfig()
    {
      auto & p = _cocomoCfg->Hardware();
      _ui.timeComboBox->setCurrentText(RToComboText(p.RuntimePerformanceConstraints()));
      _ui.storComboBox->setCurrentText(RToComboText(p.MemoryContraints()));
      _ui.virtComboBox->setCurrentText(RToComboText(p.VolatilityOfVMEnvironment()));
      _ui.turnComboBox->setCurrentText(RToComboText(p.RequiredTurnaboutTime()));
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CocomoConfigDialog::InitPersonnelGroupFromConfig()
    {
      auto & p = _cocomoCfg->Personnel();
      _ui.acapComboBox->setCurrentText(RToComboText(p.AnalystCapability()));
      _ui.aexpComboBox->setCurrentText(RToComboText(p.ApplicationsExperience()));
      _ui.pcapComboBox->setCurrentText(RToComboText(p.SoftwareEngineerCapability()));
      _ui.vexpComboBox->setCurrentText(RToComboText(p.VirtualMachineExperience()));
      _ui.lexpComboBox->setCurrentText(RToComboText(p.ProgrammingLanguageExperience()));
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CocomoConfigDialog::InitProjectGroupFromConfig()
    {
      auto & p = _cocomoCfg->Project();
      _ui.modpComboBox->setCurrentText(RToComboText(p.ApplicationOfSWEngineeringMethods()));
      _ui.toolComboBox->setCurrentText(RToComboText(p.UseOfSoftwareTools()));
      _ui.scedComboBox->setCurrentText(RToComboText(p.RequiredDevelopmentSchedule()));
      return;
    }
    
    
  }  // namespace Mcloc

}  // namespace Dwm
