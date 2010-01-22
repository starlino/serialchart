# -------------------------------------------------
# Project created by QtCreator 2010-01-13T14:10:41
# -------------------------------------------------
TARGET = SerialChart
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    qextserialport/qextserialport.cpp \
    qextserialport/qextserialbase.cpp \
    qextserialport/win_qextserialport.cpp \
    column.cpp \
    process.cpp \
    configuration.cpp \
    chart.cpp
HEADERS += mainwindow.h \
    qextserialport/win_qextserialport.h \
    qextserialport/qextserialport.h \
    qextserialport/qextserialbase.h \
    common.h \
    column.h \
    process.h \
    configuration.h \
    chart.h
FORMS += mainwindow.ui
RESOURCES += resources.qrc
