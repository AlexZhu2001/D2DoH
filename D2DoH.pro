QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dnsserver.cpp \
    dohclient.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Logger.h \
    common.h \
    dnsserver.h \
    dohclient.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

RC_ICONS = doh.ico

TRANSLATIONS += .\tr_language_English.ts \
                .\tr_language_Chinese.ts
