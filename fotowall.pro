# Check Qt >= 4.4
contains(QT_VERSION, ^4\.[0-3]\..*) {
    message("Cannot build Qt Creator with Qt version $$QT_VERSION .")
    error("Use at least Qt 4.4.")
}
contains(QT_VERSION, ^4\.4\..*): message("Lots of features will be disabled with Qt $$QT_VERSION . Use Qt 4.5 or later.")
contains(QT_VERSION, ^4\.6\..*): message("Qt $$QT_VERSION support is still experimental, but it already improves the experience!")

# Project Options
TEMPLATE = app
TARGET = fotowall
INCLUDEPATH += .
DEPENDPATH += .
MOC_DIR = .build
OBJECTS_DIR = .build
RCC_DIR = .build
UI_DIR = .build
QT = core \
    gui \
    svg \
    network \
    xml

# use OpenGL where available
contains(QT_CONFIG, opengl)|contains(QT_CONFIG, opengles1)|contains(QT_CONFIG, opengles2) {
    QT += opengl
}

# Fotowall input files
HEADERS += 3rdparty/gsuggest.h \
    CPixmap.h \
    CroppingDialog.h \
    CroppingWidget.h \
    Desk.h \
    ExactSizeDialog.h \
    ExportWizard.h \
    FlickrInterface.h \
    GlowEffectDialog.h \
    GlowEffectWidget.h \
    GroupBoxWidget.h \
    MainWindow.h \
    ModeInfo.h \
    PictureEffect.h \
    RenderOpts.h \
    VersionCheckDialog.h \
    VideoProvider.h \
    XmlRead.h \
    XmlSave.h
SOURCES += 3rdparty/gsuggest.cpp \
    CPixmap.cpp \
    CroppingDialog.cpp \
    CroppingWidget.cpp \
    Desk.cpp \
    ExactSizeDialog.cpp \
    ExportWizard.cpp \
    FlickrInterface.cpp \
    GlowEffectDialog.cpp \
    GlowEffectWidget.cpp \
    GroupBoxWidget.cpp \
    MainWindow.cpp \
    ModeInfo.cpp \
    VersionCheckDialog.cpp \
    VideoProvider.cpp \
    XmlRead.cpp \
    XmlSave.cpp \
    main.cpp
FORMS += CroppingDialog.ui \
    ExactSizeDialog.ui \
    ExportWizard.ui \
    GlowEffectDialog.ui \
    MainWindow.ui \
    VersionCheckDialog.ui
RESOURCES += fotowall.qrc
TRANSLATIONS += translations/fotowall_de.ts \
    translations/fotowall_fr.ts \
    translations/fotowall_it.ts \
    translations/fotowall_pl.ts \
    translations/fotowall_pt_BR.ts

# Sub-Components
include(items/items.pri)
include(frames/frames.pri)
include(3rdparty/richtextedit/richtextedit.pri)
include(3rdparty/videocapture/videocapture.pri)
include(3rdparty/posterazor/posterazor.pri)

# deployment on Linux
unix {
    target.path = /usr/bin
    icon.files = fotowall.png
    icon.path = /usr/share/pixmaps
    dfile.files = fotowall.desktop
    dfile.path = /usr/share/applications
    man.files = fotowall.1
    man.path = /usr/share/man/man1
    INSTALLS += target \
        icon \
        dfile \
        man
}

# deployment on Windows
win32 {
    CONFIG += embed_manifest_exe
    RC_FILE = fotowall.rc
}

# deployment on Mac
macx {
    ICON = fotowall.icns
    CONFIG += x86 ppc
    QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
}

# static builds
win32|macx {
    contains(CONFIG, static)|contains(CONFIG, qt_no_framework) {
        DEFINES += STATIC_LINK
        QTPLUGIN += qgif \
            qjpeg \
            qsvg \
            qtiff
    }
}

# Translations (make ts; make qm)
LUPDATE = lupdate -silent -no-obsolete -no-ui-lines
LRELEASE = lrelease -silent -compress -removeidentical
ts.commands = ($$LUPDATE -pro translations.txt && $$LUPDATE 3rdparty/posterazor/posterazor.pri)
qm.commands = ($$LRELEASE translations/*.ts && $$LRELEASE 3rdparty/posterazor/*.ts)
QMAKE_EXTRA_TARGETS += ts qm
!exists(translations/fotowall_it.qm): {message("Compiling translations. To update strings type 'make ts', to recompile 'make qm'") system($$qm.commands)}
