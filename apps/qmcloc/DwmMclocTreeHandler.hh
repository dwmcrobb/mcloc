//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/qmcloc/DwmMclocTreeHandler.hh 11837 $
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
//!  \file DwmMclocTreeHandler.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::TreeHandler class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCTREEHANDLER_HH_
#define _DWMMCLOCTREEHANDLER_HH_

#include <sstream>

#include "DwmMclocSourceCollection.hh"
#include "ui_qmcloc.h"

namespace Dwm {

  namespace Mcloc {

    class MainWindow;
    
    //------------------------------------------------------------------------
    //!  Used to connect events in our QTreeView of the filesystem to 
    //!  other parts of the user interface.
    //------------------------------------------------------------------------
    class TreeHandler
      : public QObject
    {
      Q_OBJECT

    public:
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      TreeHandler(MainWindow *mymw, QString rootPath,
                  QObject *parent = nullptr);

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void SetNumParsingThreads(size_t numThreads);

    public slots:
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Expanded(const QModelIndex &)
      { return; }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void DirectoryLoaded(const QString &);
  
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void ItemClicked(const QModelIndex &)
      { return; }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void ItemDoubleClicked(const QModelIndex &)
      { return; }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void ItemActivated(const QModelIndex & idx);

    private:
      QString      _rootPath;
      size_t       _numThreads;
      MainWindow  *_mymw;

      void GetCocomoHtml(const Dwm::Mcloc::CodeCounter & total,
                         std::ostringstream & htmlos);
      
      void SetStatusBar(const Dwm::Mcloc::CodeCounter & totals,
                        std::chrono::duration<long,std::micro> parseTime);
    };


  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCTREEHANDLER_HH_
