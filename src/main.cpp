/*
	SPDX-FileCopyrightText: 2009-2022 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "window.h"

#include "locale_dialog.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Hexalate");
	app.setApplicationVersion(VERSIONSTR);
	app.setApplicationDisplayName(Window::tr("Hexalate"));
	app.setOrganizationDomain("gottcode.org");
	app.setOrganizationName("GottCode");
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
	app.setWindowIcon(QIcon::fromTheme("hexalate", QIcon(":/hexalate.png")));
	app.setDesktopFileName("hexalate");
#endif

	const QString appdir = app.applicationDirPath();
	const QStringList datadirs{
#if defined(Q_OS_MAC)
		appdir + "/../Resources"
#elif defined(Q_OS_UNIX)
		DATADIR,
		appdir + "/../share/hexalate"
#else
		appdir
#endif
	};

	LocaleDialog::loadTranslator("hexalate_", datadirs);

	QCommandLineParser parser;
	parser.setApplicationDescription(Window::tr("A color matching game"));
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);

	Window window;
	window.show();

	return app.exec();
}
