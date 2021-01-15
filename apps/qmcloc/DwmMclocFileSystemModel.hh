//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/qmcloc/DwmMclocFileSystemModel.hh 11837 $
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
//!  \file DwmMclocFileSystemModel.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::FileSystemModel class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCFILESYSTEMMODEL_HH_
#define _DWMMCLOCFILESYSTEMMODEL_HH_

#include <QFileSystemModel>

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  This class exists simply to allow me to filter filesystem entries
    //!  correctly.  QFileSystemModel has almost all of the functionality I
    //!  need but its means of filtering is very limited.  Easy to fix:
    //!  derive from QFileSystemModel and override flags().
    //------------------------------------------------------------------------
    class FileSystemModel
      : public QFileSystemModel
    {
    public:
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      FileSystemModel(QObject *parent = nullptr);
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      Qt::ItemFlags flags(const QModelIndex & idx) const override;
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCFILESYSTEMMODEL_HH_
