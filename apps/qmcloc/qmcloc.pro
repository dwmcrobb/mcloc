CONFIG         += c++17
QMAKE_CXX      = $$(CXX)
QMAKE_CXXFLAGS = -std=c++17 -g
QMAKE_LINK     = $$(CXX)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0

QT += widgets
requires(qtConfig(treeview))

ICON = resources/icon.icns

FORMS        += qmcloc.ui
HEADERS       = DwmMclocFileSystemModel.hh DwmMclocMainWindow.hh
HEADERS      += DwmMclocTreeHandler.hh
SOURCES       = DwmMclocFileSystemModel.cc DwmMclocMainWindow.cc
SOURCES      += DwmMclocTreeHandler.cc main.cc

macx {
    SOURCES  += DwmMclocMacOSIsDarkMode.mm
}
    
INCLUDEPATH  += ../../classes/include
LIBS         += -L../../classes/lib -lMcloc -ltermcap 
RESOURCES = qmcloc.qrc

