/***********************************************************************
 *
 * Copyright (C) 2009, 2010, 2013, 2014, 2015, 2016, 2017, 2018, 2019 Graeme Gott <graeme@gottcode.org>
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
#include <QStatusBar>

/*****************************************************************************/

void Window::fillActionStyle(QAction* action, bool checked)
{
	action->setCheckable(true);
	action->setChecked(checked);
	board->actionsStyle.append(action);
}

void Window::fillActionColor(QAction* action, bool checked)
{
	action->setCheckable(true);
	action->setChecked(checked);
	board->actionsColor.append(action);
}

/*****************************************************************************/

Window::Window() {
	// Create game board
	board = new Board(this);
	setCentralWidget(board);

	// Create menus
	QMenu* menu = menuBar()->addMenu(tr("&Game"));
	QAction* action = menu->addAction(tr("&New"), board, SLOT(newGame()), QKeySequence::New);
	action = menu->addAction(tr("&Quit"), qApp, SLOT(quit()), QKeySequence::Quit);
	action->setMenuRole(QAction::QuitRole);

	menu = menuBar()->addMenu(tr("&Settings"));
	menu->addAction(tr("Application &Language..."), this, SLOT(setLocale()));
	menu->addSeparator();

	int style = board->getCurStyle();
	QMenu* menu2 = menu->addMenu(tr("&Styles"));
	action = menu2->addAction(tr("&Next"), board, SLOT(stylesNext()), QKeySequence("F3"));
	menu2->addSeparator();
	board->actionsColor.clear();
	action = menu2->addAction(tr("&Short"),  board, SLOT(styles0()));  fillActionStyle(action, style == 0);
	action = menu2->addAction(tr("&Medium"), board, SLOT(styles1()));  fillActionStyle(action, style == 1);
	action = menu2->addAction(tr("&Thick"),  board, SLOT(styles2()));  fillActionStyle(action, style == 2);
	action = menu2->addAction(tr("&Full"),   board, SLOT(styles3()));  fillActionStyle(action, style == 3);
	menu2->addSeparator();
	action = menu2->addAction(tr("Two"),       board, SLOT(styles4()));  fillActionStyle(action, style == 4);
	action = menu2->addAction(tr("Two fat"),   board, SLOT(styles5()));  fillActionStyle(action, style == 5);
	action = menu2->addAction(tr("X thin"),    board, SLOT(styles6()));  fillActionStyle(action, style == 6);
	action = menu2->addAction(tr("X fat"),     board, SLOT(styles7()));  fillActionStyle(action, style == 7);
	action = menu2->addAction(tr("Odd 1"),     board, SLOT(styles8()));  fillActionStyle(action, style == 8);
	action = menu2->addAction(tr("Odd 1 fat"), board, SLOT(styles9()));  fillActionStyle(action, style == 9);

	int clr = board->getCurColor();
	menu2 = menu->addMenu(tr("&Colors"));
	action = menu2->addAction(tr("&Next"), board, SLOT(colorsNext()), QKeySequence("F4"));
	menu2->addSeparator();
	board->actionsColor.clear();
	action = menu2->addAction(tr("&Basic"),  board, SLOT(colors0()));  fillActionColor(action, clr == 0);
	action = menu2->addAction(tr("&New"),    board, SLOT(colors1()));  fillActionColor(action, clr == 1);
	action = menu2->addAction(tr("&Jungle"), board, SLOT(colors2()));  fillActionColor(action, clr == 2);
	action = menu2->addAction(tr("&Fire"),   board, SLOT(colors3()));  fillActionColor(action, clr == 3);
	action = menu2->addAction(tr("&Dark"),   board, SLOT(colors4()));  fillActionColor(action, clr == 4);
	action = menu2->addAction(tr("&Light"),  board, SLOT(colors5()));  fillActionColor(action, clr == 5);

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
			tr("Copyright &copy; 2009-%1 Graeme Gott").arg("2019"),
			tr("Released under the <a href=%1>GPL 3</a> license").arg("\"http://www.gnu.org/licenses/gpl.html\""))
	);
}

/*****************************************************************************/
