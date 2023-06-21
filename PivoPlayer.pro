QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/music \
               $$PWD/playlists
RESOURCES += resources.qrc

SOURCES += \
    SoundVisualisationWidget.cpp \
    buttons.cpp \
    main.cpp \
    mainwindow.cpp \
    playlist.cpp \

HEADERS += \
    SoundVisualisationWidget.h \
    mainwindow.h \
    playlist.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
