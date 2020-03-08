QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QT += network
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CAbout.cpp \
    CUpdate.cpp \
    CleanRuleSet.cpp \
    ConfigFile.cpp \
    ErrorMessage.cpp \
    ItemDelegate.cpp \
    OptionEdit.cpp \
    SuffixOrFileNameEdit.cpp \
    main.cpp \
    HomePage.cpp

HEADERS += \
    CAbout.h \
    CUpdate.h \
    CleanRuleSet.h \
    ConfigFile.h \
    ErrorMessage.h \
    HomePage.h \
    ItemDelegate.h \
    OptionEdit.h \
    OptionEdit.h \
    SuffixOrFileNameEdit.h \
    Version.h

FORMS += \
    CAbout.ui \
    CUpdate.ui \
    CleanRuleSet.ui \
    ErrorMessage.ui \
    HomePage.ui \
    OptionEdit.ui \
    SuffixOrFileNameEdit.ui

TRANSLATIONS += \
    ProjectClean_zh_CN.ts \
    ProjectClean_EN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819

RESOURCES += \
    res.qrc

DISTFILES += \
    Update.txt
