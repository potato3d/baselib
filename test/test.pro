include(../_begin_default.pri)

TEMPLATE = app
CONFIG += console

LIBS += $${ROOT_DIR}/libraries/libbase_$${TARGET_SUFFIX}$${TARGET_DEBUG}.a

DESTDIR = $${ROOT_DIR}/binaries

SOURCES +=  main.cpp

include(../_end_default.pri)
