//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/apps/qmcloc/DwmMclocMainWindow.cc 11836 $
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
//!  \file DwmMclocMainWindow.cc
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#include <QFileDialog>
#include <QLabel>

#include "DwmMclocMainWindow.hh"
#include "DwmMclocCocomoConfigDialog.hh"

extern bool  g_usingDarkPalette;

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    MainWindow::MainWindow(QString path, QWidget *parent)
        : QMainWindow(parent), _treeHandler(this, path), _recents(),
          _cocomoCfg()
    {
      _ui.setupUi(this);

      if (! path.isEmpty()) {
        setWindowTitle(QApplication::translate("MainWindow",
                                               path.toStdString().c_str(),
                                               nullptr));
      }
      else {
        setWindowTitle(QApplication::translate("MainWindow", "qmcloc",
                                               nullptr));
      }
      
      _ui.splitter->setCollapsible(0, false);
      _ui.splitter->setCollapsible(1, false);
      
      _model.setRootPath("");
      _ui.treeView->setModel(&_model);
      
      _ui.treeView->hideColumn(1);
      _ui.treeView->setColumnWidth(1, 0);
      _ui.treeView->hideColumn(2);
      _ui.treeView->setColumnWidth(2, 0);
      _ui.treeView->setExpandsOnDoubleClick(false);

      if (! path.isEmpty()) {
        const QModelIndex rootIndex = _model.index(QDir::cleanPath(path));
        if (rootIndex.isValid()) {
          _ui.treeView->setRootIndex(rootIndex);
          _ui.treeView->setCurrentIndex(rootIndex);
          _recents.Add(path.toStdString());
        }
      }

      _recentsMenu = new QMenu("&Recent...");
      // _recentsMenu->?->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
      
      UpdateRecentsMenu();
      _ui.menuFile->addMenu(_recentsMenu);
      connect(_recentsMenu, &QMenu::triggered,
              this, &MainWindow::onRecentTriggered);
      
      _ui.treeView->setAnimated(false);
      _ui.treeView->setIndentation(20);
      _ui.treeView->setSortingEnabled(true);
      _ui.treeView->setColumnWidth(0, _ui.treeView->width() / 3);
      _ui.treeView->setWindowTitle(QObject::tr("Dir View"));
      _ui.treeView->sortByColumn(0, Qt::AscendingOrder);

#ifdef __APPLE__
      _ui.menubar->setNativeMenuBar(true);
#else
      _ui.menubar->setNativeMenuBar(false);
      AddLogo();
#endif
      MakeConnections();
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::UpdateRecentsMenu()
    {
      _recentsMenu->clear();
      for (const auto & recent : _recents.Paths()) {
        _recentsMenu->addAction(recent.second.c_str());
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::SetNumParsingThreads(size_t numThreads)
    {
      _treeHandler.SetNumParsingThreads(numThreads);
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::AddLogo()
    {
      QString  logoFile;
      if (g_usingDarkPalette) {
        logoFile = ":/resources/images/mcloc_orange_base.svg";
      }
      else {
        logoFile = ":/resources/images/mcloc_blue_base.svg";
      }
      QImage  logoImage(logoFile);
      QPixmap logopxm;
      logopxm.convertFromImage(logoImage.scaled(_ui.menubar->width(),
                                                _ui.menubar->height(),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
      QLabel  *tmpLabel = new QLabel();
      tmpLabel->setPixmap(logopxm);
      _ui.menubar->setCornerWidget(tmpLabel);
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::MakeConnections()
    {
      connect(_ui.treeView, &QTreeView::clicked,
              &_treeHandler, &TreeHandler::ItemClicked);
      connect(_ui.treeView, &QTreeView::doubleClicked,
              &_treeHandler, &TreeHandler::ItemDoubleClicked);
      connect(_ui.treeView, &QTreeView::activated,
              &_treeHandler, &TreeHandler::ItemActivated);
      connect(_ui.treeView, &QTreeView::expanded,
              &_treeHandler, &TreeHandler::Expanded);
      connect(&_model, &QFileSystemModel::directoryLoaded,
              &_treeHandler, &TreeHandler::DirectoryLoaded);
      connect(_ui.actionQuit, &QAction::triggered,
              &QApplication::closeAllWindows);
      connect(_ui.actionOpen, &QAction::triggered,
              this, &MainWindow::onActionOpenTriggered);
      connect(_ui.actionConfig, &QAction::triggered,
              this, &MainWindow::cocomoConfigTriggered);
      connect(_ui.actionLoad_Config, &QAction::triggered,
              this, &MainWindow::cocomoLoadConfigTriggered);
      
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::onActionOpenTriggered()
    {
      QFileDialog dialog(this);
      dialog.setFileMode(QFileDialog::Directory);
      QStringList fileNames;
      if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        _model.setRootPath(fileNames[0]);
        const QModelIndex rootIndex =
          _model.index(QDir::cleanPath(fileNames[0]));
        _ui.treeView->setRootIndex(rootIndex);
        _recents.Add(fileNames[0].toStdString());
        UpdateRecentsMenu();
        setWindowTitle(QApplication::translate("MainWindow",
                                               fileNames[0].toStdString().c_str(),
                                               nullptr));
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::onRecentTriggered(QAction *action)
    {
      const QModelIndex rootIndex =
        _model.index(QDir::cleanPath(action->text()));
      _ui.treeView->setRootIndex(rootIndex);
      setWindowTitle(QApplication::translate("MainWindow",
                                             action->text().toStdString().c_str(),
                                             nullptr));
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::cocomoConfigTriggered()
    {
      Cocomo1::Intermediate::Config  cocomoConfig(this->_cocomoCfg);
      CocomoConfigDialog  dialog(&cocomoConfig, this);
      if (dialog.exec()) {
        this->_cocomoCfg = cocomoConfig;
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::cocomoLoadConfigTriggered()
    {
      QFileDialog  dialog(this);
      dialog.setAcceptMode(QFileDialog::AcceptOpen);
      QStringList fileNames;
      if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        _cocomoCfg.Parse(fileNames[0].toStdString());
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void MainWindow::keyPressEvent(QKeyEvent *event)
    {
      if ((event->key() == Qt::Key_F)
          && (event->modifiers() & Qt::ControlModifier)) {
        _ui.menubar->setActiveAction(_ui.menuFile->menuAction());
      }
      else {
        QMainWindow::keyPressEvent(event);
      }
      return;
    }
    
    
  }  // namespace Mcloc

}  // namespace Dwm
