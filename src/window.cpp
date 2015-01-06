/***********************************************************************
 *
 * Copyright (C) 2009, 2010, 2013, 2014, 2015 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "window.h"

#include "board.h"
#include "locale_dialog.h"

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>

/*****************************************************************************/

Window::Window() {
	// Create game board
	Board* board = new Board(this);
	setCentralWidget(board);

	// Create menus
	QMenu* menu = menuBar()->addMenu(tr("&Game"));
	menu->addAction(tr("&New"), board, SLOT(newGame()), QKeySequence::New);
	menu->addSeparator();
	QAction* action = menu->addAction(tr("&Quit"), qApp, SLOT(quit()), QKeySequence::Quit);
	action->setMenuRole(QAction::QuitRole);

	menu = menuBar()->addMenu(tr("&Settings"));
	menu->addAction(tr("Application &Language..."), this, SLOT(setLocale()));

	menu = menuBar()->addMenu(tr("&Help"));
	action = menu->addAction(tr("&About"), this, SLOT(about()));
	action->setMenuRole(QAction::AboutRole);
	action = menu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	action->setMenuRole(QAction::AboutQtRole);

	// Restore size and position
	restoreGeometry(QSettings().value("Geometry").toByteArray());
}

/*****************************************************************************/

void Window::closeEvent(QCloseEvent* event) {
	QSettings().setValue("Geometry", saveGeometry());
	QMainWindow::closeEvent(event);
}

/*****************************************************************************/

void Window::setLocale() {
	LocaleDialog dialog(this);
	dialog.exec();
}

/*****************************************************************************/

void Window::about() {
	QMessageBox::about(this, tr("About Hexalate"), QString("<p align='center'><big><b>%1 %2</b></big><br/>%3<br/><small>%4<br/>%5</small></p>")
		.arg(tr("Hexalate"), QCoreApplication::applicationVersion(),
			tr("A color matching game"),
			tr("Copyright &copy; 2009-%1 Graeme Gott").arg("2015"),
			tr("Released under the <a href=%1>GPL 3</a> license").arg("\"http://www.gnu.org/licenses/gpl.html\""))
	);
}

/*****************************************************************************/
