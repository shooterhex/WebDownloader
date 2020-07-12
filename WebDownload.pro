QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

CONFIG += c++17

#FORMS += \
#    WINDOW/about.ui \
#    WINDOW/firstpage.ui \
#    WINDOW/model1_main.ui \
#    WINDOW/operation.ui \
#    WINDOW/testwindow.ui \
#    WINDOW/model2_main.ui

HEADERS += \
    App/App.h \
    common/cslbase.h \
    common/curl/curl.h \
    model/model.h \
    viewModel/viewModel.h \
    window/mainWindow.h

SOURCES += \
    App/App.cpp \
    App/main.cpp \
    common/clsbase.cpp \
    model/model.cpp \
    viewModel/command/command.cpp \
    viewModel/viewModel.cpp \
    window/mainWindow.cpp

unix {
    LIBS += -lcurl
}
else {
    LIBS+= -L"D:\tool\curl-7.71.1\lib\curl-7.71.1\x86 Release\lib" -llibcurl_a
    FORMS += \
        mainWindow.ui
    #DEFINES+=CURL_STATICLIB
    DEFINES+=WINDOWS 
    DEFINES+=BUILDING_LIBCURL \
        HTTP_ONLY
}