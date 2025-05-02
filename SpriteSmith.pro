QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Force MOC processing for SpriteFrameWidget and model
INCLUDEPATH += .

# Explicitly list files containing Q_OBJECT macro to ensure MOC processing
HEADERS_MOC = model.h

SOURCES += \
    graphicsview.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    spriteframewidget.cpp

HEADERS += \
    graphicsview.h \
    mainwindow.h \
    model.h \
    spriteframewidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
