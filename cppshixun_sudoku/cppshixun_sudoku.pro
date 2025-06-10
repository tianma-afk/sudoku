QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_with_server.cpp \
    frame_move.cpp \
    game.cpp \
    game_2.cpp \
    gameboard.cpp \
    gameboard_2.cpp \
    main.cpp \
    mainwindow.cpp \
    namedialog.cpp \
    page.cpp \
    page_person.cpp \
    page_welcome.cpp \
    rankitem.cpp \
    sdkboardlist.cpp

HEADERS += \
    chat_with_server.h \
    frame_move.h \
    game.h \
    game_2.h \
    gameboard.h \
    gameboard_2.h \
    mainwindow.h \
    namedialog.h \
    page.h \
    page_person.h \
    page_welcome.h \
    rankitem.h \
    sdkboardlist.h

FORMS += \
    gameboard.ui \
    gameboard_2.ui \
    mainwindow.ui \
    namedialog.ui \
    page_person.ui \
    page_welcome.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS=favicon.ico
