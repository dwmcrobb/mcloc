//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/qmcloc/DwmMclocTreeHandler.cc 11836 $
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
//!  \file DwmMclocTreeHandler.cc
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::TreeHandler class implementation
//---------------------------------------------------------------------------

#include <sstream>
#include <thread>

#include <QFileSystemModel>

#include "DwmMclocEngNotate.hh"
#include "DwmMclocSourceCollection.hh"
#include "DwmMclocTreeHandler.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    TreeHandler::TreeHandler(Ui_MainWindow *mymw, QString rootPath,
                             QObject *parent)
        : QObject(parent), _rootPath(rootPath), _mymw(mymw)
    {
      _numThreads = std::thread::hardware_concurrency();
      if (_numThreads > 24) {
        _numThreads = 24;
      }
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void TreeHandler::SetNumParsingThreads(size_t numThreads)
    {
      _numThreads = numThreads;
      if (_numThreads > 24) {
        _numThreads = 24;
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void TreeHandler::DirectoryLoaded(const QString &)
    {
      _mymw->treeView->resizeColumnToContents(0);
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    static void GetLanguageHtml(Dwm::Mcloc::SourceCollection & sc,
                                const Dwm::Mcloc::CodeCounter & total,
                                std::ostringstream & htmlos)
    {
      Dwm::Mcloc::SourceCollection::StringCounterPairVector  lcv;
      sc.GetTotalsByLanguage(lcv, Dwm::Mcloc::sortBySizeReversed);
      if (lcv.empty()) {
        return;
      }

      htmlos << "<table width=100% cellspacing=0>"
             << "<tr><td align=center colspan=3><b>By Language</b></td></tr>"
             << "<tr>"
             << "<th align=left style=\"border-bottom: 1px solid #FF7F2A;\"><b>Language</b></th>"
             << "<th align=right style=\"border-bottom: 1px solid #FF7F2A;\"><b>Code</b></th>"
             << "<th align=right style=\"border-bottom: 1px solid #FF7F2A;\"><b>Comments</b></th>"
             << "</tr>";
      auto  lcit = lcv.begin();
      for ( ; lcit != (lcv.end() - 1); ++lcit) {
        htmlos << "<tr>"
               << "<td align=left>" << lcit->first << "</td>"
               << "<td align=right>" << lcit->second.CodeLines() << "</td>"
               << "<td align=right>" << lcit->second.CommentLines() << "</td>"
               << "</tr>";
      }
      htmlos << "<tr>"
             << "<td align=left style=\"border-bottom: 1px solid #FF7F2A;\">" << lcit->first << "</td>"
             << "<td align=right style=\"border-bottom: 1px solid #FF7F2A;\">" << lcit->second.CodeLines() << "</td>"
             << "<td align=right style=\"border-bottom: 1px solid #FF7F2A;\">" << lcit->second.CommentLines() << "</td>"
             << "</tr>";
      
      htmlos << "<tr>"
        // << "<tr style=\"background: #ffeedd;\">"
             << "<td align=left>" << "<b><i>TOTAL</i></b>" << "</td>"
             << "<td align=right><b>"
             << total.CodeLines() << "</b></td>"
             << "<td align=right><b>"
             << total.CommentLines() << "</b></td>"
             << "</tr>"
             << "</table>";
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    static void GetExtensionHtml(Dwm::Mcloc::SourceCollection & sc,
                                 const Dwm::Mcloc::CodeCounter & total,
                                 std::ostringstream & htmlos)
    {
      Dwm::Mcloc::SourceCollection::StringCounterPairVector  ecv;
      sc.GetTotalsByExtension(ecv, Dwm::Mcloc::sortBySizeReversed);
      if (ecv.empty()) {
        return;
      }
      
      htmlos << "<table width=100% cellspacing=0>"
             << "<tr><td align=center colspan=3><b>By Extension</b></td></tr>"
             << "<tr>"
             << "<th align=left style=\"border-bottom: 1px solid #FF7F2A;\"><b>Extension</b></th>"
             << "<th align=right style=\"border-bottom: 1px solid #FF7F2A;\"><b>Code</b></th>"
             << "<th align=right style=\"border-bottom: 1px solid #FF7F2A;\"><b>Comments</b></th>"
             << "</tr>";
      auto  ecit = ecv.begin();
      for ( ; ecit != (ecv.end() - 1); ++ecit) {
        htmlos << "<tr>"
               << "<td>" << ecit->first << "</td>"
               << "<td align=right>" << ecit->second.CodeLines() << "</td>"
               << "<td align=right>" << ecit->second.CommentLines() << "</td>"
               << "</tr>";
      }
      htmlos << "<tr>"
             << "<td align=left style=\"border-bottom: 1px solid #FF7F2A;\">"
             << ecit->first << "</td>"
             << "<td align=right style=\"border-bottom: 1px solid #FF7F2A;\">"
             << ecit->second.CodeLines() << "</td>"
             << "<td align=right style=\"border-bottom: 1px solid #FF7F2A;\">"
             << ecit->second.CommentLines() << "</td>"
             << "</tr>";
      htmlos << "<tr>"
             << "<td align=left>" << "<b><i>TOTAL</i></b>" << "</td>"
             << "<td align=right><b>"
             << total.CodeLines() << "</b></td>"
             << "<td align=right><b>"
             << total.CommentLines() << "</b></td>"
             << "</tr>"
             << "</table>";
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    TreeHandler::SetStatusBar(const CodeCounter & totals,
                              std::chrono::duration<long,std::micro> parseTime)
    {
      if (parseTime.count() > 0) {
        uint64_t  linesProcessed =
          totals.CodeLines() + totals.CommentLines();
        uint64_t  linesPerSec =
          linesProcessed / (parseTime.count() / 1000000.0);
        std::ostringstream  os;
        os.imbue(std::locale(""));
        os << "Processed " << linesProcessed << " lines in "
           << Dwm::Mcloc::EngNotate((parseTime.count() / 1000000.0), 4, 4)
           << "s ("
           << Dwm::Mcloc::EngNotate(linesPerSec, 4, 4)
           << " lines/sec)";
        _mymw->statusbar->showMessage(os.str().c_str(), 5000);
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void TreeHandler::ItemActivated(const QModelIndex & idx)
    {
      QFileSystemModel  *mdl = (QFileSystemModel *)idx.model();
      Dwm::Mcloc::SourceCollection  sc;
      bool  shouldParse = false;
      if (mdl->isDir(idx)) {
        sc.AddDirectory(mdl->filePath(idx).toStdString());
        shouldParse = true;
      }
      else {
        if (CodeUtils::PathToLanguage(mdl->filePath(idx).toStdString())) {
          sc.AddFile(mdl->filePath(idx).toStdString());
          shouldParse = true;
        }
      }
      if (shouldParse) {
        sc.Parse(_numThreads);
        Dwm::Mcloc::CodeCounter  totalCounter;
        sc.GetTotal(totalCounter);
        std::ostringstream  htmlos;
        htmlos.imbue(std::locale(""));
        GetLanguageHtml(sc, totalCounter, htmlos);
        htmlos << "<p>&nbsp;</p>";
        GetExtensionHtml(sc, totalCounter, htmlos);
        _mymw->textEdit->setHtml(htmlos.str().c_str());
        SetStatusBar(totalCounter, sc.ParseTime());
      }
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
