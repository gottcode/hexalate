!versionAtLeast(QT_VERSION, 5.12) {
	error("Hexalate requires Qt 5.12 or greater")
}

TEMPLATE = app
QT += widgets
CONFIG += c++17

CONFIG(debug, debug|release) {
	CONFIG += warn_on
	DEFINES += QT_DEPRECATED_WARNINGS
	DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
	DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
}

# Allow in-tree builds
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

# Set program version
VERSION = 1.1.6
DEFINES += VERSIONSTR=\\\"$${VERSION}\\\"

# Set program name
unix: !macx {
	TARGET = hexalate
} else {
	TARGET = Hexalate
}

# Specify program sources
HEADERS = src/board.h \
	src/locale_dialog.h \
	src/piece.h \
	src/puzzle.h \
	src/window.h

SOURCES = src/board.cpp \
	src/locale_dialog.cpp \
	src/main.cpp \
	src/piece.cpp \
	src/puzzle.cpp \
	src/window.cpp

# Generate translations
TRANSLATIONS = $$files(translations/hexalate_*.ts)
qtPrepareTool(LRELEASE, lrelease)
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$LRELEASE -silent ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

# Install program data
macx {
	ICON = icons/hexalate.icns
} else:win32 {
	RC_ICONS = icons/hexalate.ico
	QMAKE_TARGET_DESCRIPTION = "Color matching game"
	QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2021 Graeme Gott"
} else:unix: {
	RESOURCES = icons/icon.qrc

	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	isEmpty(BINDIR) {
		BINDIR = bin
	}

	target.path = $$PREFIX/$$BINDIR/

	icon.files = icons/hicolor/*
	icon.path = $$PREFIX/share/icons/hicolor

	pixmap.files = icons/hexalate.xpm
	pixmap.path = $$PREFIX/share/pixmaps

	desktop.files = icons/hexalate.desktop
	desktop.path = $$PREFIX/share/applications

	appdata.files = icons/hexalate.appdata.xml
	appdata.path = $$PREFIX/share/metainfo/

	qm.files = $$replace(TRANSLATIONS, .ts, .qm)
	qm.path = $$PREFIX/share/hexalate/translations
	qm.CONFIG += no_check_exist

	man.files = doc/hexalate.6
	man.path = $$PREFIX/share/man/man6

	INSTALLS += target icon pixmap desktop appdata qm man
}
