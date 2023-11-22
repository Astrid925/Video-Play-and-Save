win32: {
    FFMPEG_HOME=D:\cxy\QtProject\ffmpeg\ffmpeg-5.1.2-full_build-shared
    #设置 ffmpeg 的头文件
    INCLUDEPATH += $$FFMPEG_HOME/include

    #设置导入库的目录一边程序可以找到导入库
    # -L ：指定导入库的目录
    # -l ：指定要导入的 库名称
    LIBS +=  -L$$FFMPEG_HOME/lib \
             -lavcodec \
             -lavdevice \
             -lavfilter \
             -lavformat \
             -lavutil \
             -lpostproc \
             -lswresample \
             -lswscale
}

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
    myslider.cpp \
    playimage.cpp \
    playthread.cpp \
    readthread.cpp \
    realTimePlay.cpp \
    urlwindow.cpp \
    videoPlayBack.cpp

HEADERS += \
    mainwindow.h \
    myslider.h \
    playimage.h \
    playthread.h \
    readthread.h \
    realTimePlay.h \
    urlwindow.h \
    videoPlayBack.h

FORMS += \
    mainwindow.ui \
    urlwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = playerIcon.ico
