QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    modelview.cpp

HEADERS += \
    mainwindow.h \
    modelview.h

FORMS += \
    mainwindow.ui

unix: !macx {
    INCLUDEPATH +=  /usr/include
    LIBS += /usr/lib/libassimp.so
}

macx {
    INCLUDEPATH +=  /usr/local/include
    LIBS += /usr/local/lib/libassimp.dylib
}

win32 {
    INCLUDEPATH += "C:/Assimp3/include"
    LIBS += -L"C:/Assimp3/lib/Release" -lassimp
}



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../JEngine/release/ -lJEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../JEngine/debug/ -lJEngine
else:unix: LIBS += -L$$OUT_PWD/../JEngine/ -lJEngine

INCLUDEPATH += $$PWD/../JEngine
DEPENDPATH += $$PWD/../JEngine

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../JEngine/release/libJEngine.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../JEngine/debug/libJEngine.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../JEngine/release/JEngine.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../JEngine/debug/JEngine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../JEngine/libJEngine.a

DISTFILES +=

RESOURCES += \
    Shaders.qrc


