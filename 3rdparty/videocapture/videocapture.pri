VPATH += $$PWD
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

unix {
    DEFINES += HAS_VIDEOCAPTURE

    HEADERS += bayer.h \
        sonix_compress.h \
        VideoDevice_linux.h \
        VideoInput.h

    SOURCES += bayer.cpp \
        sonix_compress.cpp \
        VideoDevice_linux.cpp \
        VideoInput.cpp
} else: {
    message("VideoCapture is only available on Linux for now")
    DEFINES -= HAS_VIDEOCAPTURE
}