QT += quick core gui qml

CONFIG += c++17

TARGET = sumplete
TEMPLATE = app

HEADERS += \
    tablemodel.h
SOURCES += \
    tablemodel.cpp \
    main.cpp

RESOURCES += \
    qml.qrc

# Дополнительные настройки компилятора
unix:!macx: QMAKE_LFLAGS += -Wl,-rpath,\'\$$ORIGIN\'

# Установка QML_IMPORT_PATH и QML2_IMPORT_PATH при необходимости

DISTFILES += \
    main.qml