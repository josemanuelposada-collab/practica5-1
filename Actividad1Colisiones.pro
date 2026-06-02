QT += core gui widgets

CONFIG += c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = Actividad1Colisiones

SOURCES += \
    main.cpp \
    particle.cpp \
    obstacle.cpp \
    simulation.cpp \
    trajectorywidget.cpp

HEADERS += \
    particle.h \
    obstacle.h \
    simulation.h \
    trajectorywidget.h \
    vector2d.h

OTHER_FILES += \
    README.md \
    DIAGRAMA_CLASES.md \
    INFORME_ACTIVIDAD1.md
