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
//!  \file DwmMclocCocomoConfigDialog.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCOCOMOCONFIGDIALOG_HH_
#define _DWMMCLOCCOCOMOCONFIGDIALOG_HH_

#include <utility>
#include <vector>
#include <QDialog>

#include "ui_cocomoCfgDialog.h"
#include "DwmMclocCocomo1IntermediateConfig.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    class CocomoConfigDialog
      : public QDialog
    {
      Q_OBJECT
      
    public:
      CocomoConfigDialog(Cocomo1::Intermediate::Config *cocomoCfg,
                         QWidget *parent = nullptr);

    private:
      Ui_cocomoConfigDialog           _ui;
      Cocomo1::Intermediate::Config  *_cocomoCfg;

      Cocomo1::Intermediate::Rating ComboToRating(const QString & txt);

      Cocomo1::Intermediate::ProjectClassEnum
      ComboToProjectClass(const QString & txt);
      
      template <class T, typename FnType>
      void ConnectGroupCombos(T & cfg,
                              std::vector<std::pair<QComboBox *,FnType>> fns)
      {
        for (auto & it : fns) {
          connect(it.first, &QComboBox::currentTextChanged,
                  [=,&cfg] (const QString & txt)
                  { (cfg.*(it.second))(ComboToRating(txt)); });
        }
        return;
      }

      void InitFromConfig();
      void InitProductGroupFromConfig();
      void InitHardwareGroupFromConfig();
      void InitPersonnelGroupFromConfig();
      void InitProjectGroupFromConfig();

      void ConnectGroupCombos();
      void ConnectProductGroupCombos();
      void ConnectHardwareGroupCombos();
      void ConnectPersonnelGroupCombos();
      void ConnectProjectGroupCombos();
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCOCOMOCONFIGDIALOG_HH_
