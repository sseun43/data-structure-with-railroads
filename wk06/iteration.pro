TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        iteration1.cpp \
        iteration2.cpp \
        iteration3.cpp \
        iteration4.cpp

HEADERS += \
        CLI11.hpp \
        iteration1.hh \
        iteration2.hh \
        iteration3.hh \
        iteration4.hh
