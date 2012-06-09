TEMPLATE = app
CONFIG += warn_on
macx {
	CONFIG += x86_64
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

VERSION = $$system(git rev-parse --short HEAD)
isEmpty(VERSION) {
	VERSION = 0
}
DEFINES += VERSIONSTR=\\\"git.$${VERSION}\\\"

unix: !macx {
	TARGET = hexalate
} else {
	TARGET = Hexalate
}

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

TRANSLATIONS = translations/hexalate_ca.ts \
	translations/hexalate_cs.ts \
	translations/hexalate_de.ts \
	translations/hexalate_en.ts \
	translations/hexalate_es.ts \
	translations/hexalate_et.ts \
	translations/hexalate_fr.ts

RESOURCES = icons/icon.qrc
macx {
	ICON = icons/hexalate.icns
} else:win32 {
	RC_FILE = icons/icon.rc
}

unix: !macx {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	isEmpty(BINDIR) {
		BINDIR = bin
	}

	target.path = $$PREFIX/$$BINDIR/

	icon.path = $$PREFIX/share/icons/hicolor/48x48/apps/
	icon.files = icons/hexalate.png

	desktop.path = $$PREFIX/share/applications/
	desktop.files = icons/hexalate.desktop

	qm.path = $$PREFIX/share/hexalate/translations/
	qm.files = translations/*.qm

	INSTALLS += target icon desktop qm
}
