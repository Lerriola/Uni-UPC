TEMPLATE    = app
QT         += opengl
QT		   += gui

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ./Model/

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h
HEADERS += ./Model/model.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp 
SOURCES += ./Model/model.cpp
