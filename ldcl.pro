TARGET = ldcl
TEMPLATE = app

CONFIG(debug, debug|release){
	DESTDIR = ../bin/debug
}
else{
	DESTDIR = ../bin/release
}

SOURCES += \
        main.cpp \
        ldcl.cpp  \
        qmdiareabackground.cpp \
        widgetcl.cpp \
        widgetcxe.cpp\
        combodelegate.cpp \
        ./serial/qcommserial.cpp \
        ./serial/qextserialbase.cpp \
        ./serial/win_qextserialport.cpp \
        ./serial/qserialthread.cpp \
        itemdm.cpp \
        graphicviewdm.cpp \
        dlgresult.cpp \
        clscx.cpp \
        func.cpp

#        QWidgetManage.cpp \
#        sysconfig.cpp \
#        qdlglogin.cpp

HEADERS += \
        ldcl.h \
        qmdiareabackground.h \
        widgetcl.h \
        widgetcxe.h\
        combodelegate.h \
        ./serial/qcommserial.h \
        ./serial/qextserialbase.h \
        ./serial/win_qextserialport.h \
        ./serial/qserialthread.h \
        itemdm.h \
        graphicviewdm.h \
        dlgresult.h \
        clscx.h \
        func.h

#         QWidgetManage.h \
#         sysconfig.h \
#         QWidgetInOut.h \
#         qdlglogin.h

RESOURCES  =  ldcl.qrc

QT += core gui
QT += sql

LIBS += -lqaxserver \
        -lqaxcontainer

# define for clear error  C4996
DEFINES += _CRT_SECURE_NO_DEPRECATE
