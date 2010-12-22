/***********************************************************************
 *
 * Copyright (C) 2009 Graeme Gott <graeme@gottcode.org>
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

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>

/*****************************************************************************/

Window::Window() {
	setWindowIcon(QIcon(":/hexalate.png"));
	setWindowTitle(tr("Hexalate"));

	// Create game board
	Board* board = new Board(this);
	setCentralWidget(board);

	// Create menus
	QMenu* menu = menuBar()->addMenu(tr("&Game"));
	menu->addAction(tr("&New"), board, SLOT(newGame()), tr("Ctrl+N"));
	menu->addSeparator();
	menu->addAction(tr("&Quit"), qApp, SLOT(quit()), tr("Ctrl+Q"));

	menu = menuBar()->addMenu(tr("&Help"));
	menu->addAction(tr("&About"), this, SLOT(about()));
	menu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

	// Restore size and position
	restoreGeometry(QSettings().value("Geometry").toByteArray());
}

/*****************************************************************************/

void Window::closeEvent(QCloseEvent* event) {
	QSettings().setValue("Geometry", saveGeometry());
	QMainWindow::closeEvent(event);
}

/*****************************************************************************/

void Window::about() {
	QMessageBox::about(this, tr("About Hexalate"), tr("<center><big><b>Hexalate %1</b></big><br/>A color matching game<br/><small>Copyright &copy; 2009 Graeme Gott</small></center>").arg(qApp->applicationVersion()));
}

/*****************************************************************************/
