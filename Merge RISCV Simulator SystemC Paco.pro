QT       += core gui
QT += svg
QT += xml
QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += c++17

CONFIG -= c++17
CONFIG += gnu++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SVG_Viewer.cpp \
    qt-sc/sc_config.cpp \
    qt-sc/sc_debug_object.cpp \
    qt-sc/sc_qt_adaptor.cpp \
    qt-sc/sc_run.cpp \
    qt-sc/scqt_worker.cpp \
    qt/main.cpp \
    qt/mainwindow.cpp \
    sc/sc_main.cpp

HEADERS += \
    SVG_Viewer.h \
    qt-sc/sc_config.h \
    qt-sc/sc_debug_object.h \
    qt-sc/sc_qt_adaptor.h \
    qt-sc/sc_run.h \
    qt-sc/scqt_worker.h \
    qt/mainwindow.h \
    sc/common/sc_arith.h \
    sc/common/sc_clk.h \
    sc/common/sc_flexint.h \
    sc/common/sc_gates.h \
    sc/common/sc_gates_pv.h \
    sc/common/sc_mux.h \
    sc/common/sc_reg.h \
    sc/common/sc_rst.h \
    sc/common/sc_terminals.h \
    sc/common/sc_tri.h \
    sc/sc_main.h

QMAKE_INCDIR += \
    qt \
    qt-sc \
    sc \
    sc/common


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SYSTEMC_INSTALL_PREFIX=C:/Qt/Tools/SystemC/Install
LIBS += -L$$SYSTEMC_INSTALL_PREFIX/lib/ -lsystemc

INCLUDEPATH += $$SYSTEMC_INSTALL_PREFIX/include
#DEPENDPATH += $$PWD/../../../../../Qt/Tools/SystemC/systemc-2.3.3/src
#PRE_TARGETDEPS += $$PWD/../../../../../Qt/Tools/SystemC/systemc-2.3.3/build/src/libsystemc.a


