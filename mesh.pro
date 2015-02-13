VPATH += ../shared
 INCLUDEPATH += ../shared

 HEADERS       = glwidget.h \
                 window.h \
                 mesh.hpp
 SOURCES       = glwidget.cpp \
                 main.cpp \
                 window.cpp \
                 mesh.cpp
 QT           += opengl

QMAKE_CXX=icpc
QMAKE_LINK=icpc
QMAKE_CXXFLAGS="-g -O0 -std=c++11"
 # install
 target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
 sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS hellogl.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

 symbian: warning(This example might not fully work on Symbian platform)
 maemo5: warning(This example does not work on Maemo platform)
 simulator: warning(This example might not fully work on Simulator platform)
