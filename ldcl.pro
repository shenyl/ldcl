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
        floatdelegate.cpp \
        ./serial/qcommserial.cpp \
        ./serial/qextserialbase.cpp \
        ./serial/win_qextserialport.cpp \
        ./serial/qserialthread.cpp \
        itemdm.cpp \
        itemv.cpp \
        graphicviewdm.cpp \
        graphicviewv.cpp \
        dlgresult.cpp \
        dlgresultcl.cpp \
        dlgresultq.cpp \
        clscx.cpp \
        func.cpp \
        getdm.cpp \
        qdlglogin.cpp \
        browsehistory.cpp  \
        sqlquerymodelresult.cpp \
        widgetv.cpp \
        sendbuf.cpp \
	ramdrive.cpp

HEADERS += \
        ldcl.h \
        qmdiareabackground.h \
        widgetcl.h \
        widgetcxe.h\
        combodelegate.h \
        floatdelegate.h \
        ./serial/qcommserial.h \
        ./serial/qextserialbase.h \
        ./serial/win_qextserialport.h \
        ./serial/qserialthread.h \
        itemdm.h \
        itemv.h \
        graphicviewdm.h \
        graphicviewv.h \
        dlgresult.h \
        dlgresultcl.h \
        dlgresultq.h \
        clscx.h \
        func.h \
        getdm.h \
        qdlglogin.h \
        browsehistory.h  \
        sqlquerymodelresult.h \
        widgetv.h \
        sendbuf.h \
	ramdrive.h 

RESOURCES  =  ldcl.qrc

QT += core gui
QT += sql

LIBS += -lqaxserver \
        -lqaxcontainer

# define for clear error  C4996
DEFINES += _CRT_SECURE_NO_DEPRECATE
