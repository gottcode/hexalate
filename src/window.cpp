/*
	SPDX-FileCopyrightText: 2009-2022 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "window.h"

#include "board.h"
#include "locale_dialog.h"

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>

//-----------------------------------------------------------------------------

Window::Window()
{
	// Create game board
	Board* board = new Board(this);
	setCentralWidget(board);

	// Create menus
	QMenu* menu = menuBar()->addMenu(tr("&Game"));
	menu->addAction(tr("&New"), board, &Board::newGame, QKeySequence::New);
	menu->addSeparator();
	QAction* action = menu->addAction(tr("&Quit"), qApp, &QApplication::quit, QKeySequence::Quit);
	action->setMenuRole(QAction::QuitRole);

	menu = menuBar()->addMenu(tr("&Settings"));
	menu->addAction(tr("Application &Language..."), this, &Window::setLocale);

	menu = menuBar()->addMenu(tr("&Help"));
	action = menu->addAction(tr("&About"), this, &Window::about);
	action->setMenuRole(QAction::AboutRole);
	action = menu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	action->setMenuRole(QAction::AboutQtRole);

	// Restore size and position
	restoreGeometry(QSettings().value("Geometry").toByteArray());
}

//-----------------------------------------------------------------------------

void Window::closeEvent(QCloseEvent* event)
{
	QSettings().setValue("Geometry", saveGeometry());
	QMainWindow::closeEvent(event);
}

//-----------------------------------------------------------------------------

void Window::setLocale()
{
	LocaleDialog dialog(this);
	dialog.exec();
}

//-----------------------------------------------------------------------------

void Window::about()
{
	QMessageBox::about(this, tr("About Hexalate"), QString("<p align='center'><big><b>%1 %2</b></big><br/>%3<br/><small>%4<br/>%5</small></p>")
		.arg(tr("Hexalate"), QCoreApplication::applicationVersion(),
			tr("A color matching game"),
			tr("Copyright &copy; 2009-%1 Graeme Gott").arg("2022"),
			tr("Released under the <a href=%1>GPL 3</a> license").arg("\"http://www.gnu.org/licenses/gpl.html\""))
	);
}

//-----------------------------------------------------------------------------
