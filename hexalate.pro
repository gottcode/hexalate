TEMPLATE = app
CONFIG += warn_on release
macx {
	# Uncomment the following line to compile on PowerPC Macs
	# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG += x86 ppc
}

MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build

unix: !macx {
	TARGET = hexalate
} else {
	TARGET = Hexalate
}

HEADERS = src/board.h \
	src/piece.h \
	src/puzzle.h \
	src/window.h

SOURCES = src/board.cpp \
	src/main.cpp \
	src/piece.cpp \
	src/puzzle.cpp \
	src/window.cpp

TRANSLATIONS = translations/en_US.ts

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
