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

#include "board.h"

#include "puzzle.h"

#include <QMessageBox>
#include <QSettings>
#include <QAction>

/*****************************************************************************/

Board::Board(QWidget* parent)
: QGraphicsView(parent) {
	setRenderHint(QPainter::Antialiasing, true);
	setBackgroundBrush(Qt::lightGray);

	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumSize(320, 320);

	m_puzzle = new Puzzle(this);
	setScene(m_puzzle);
}

/*****************************************************************************/

void Board::newGame() {
	if (!m_puzzle->isDone() && QMessageBox::question(this, tr("Question"), tr("Abort current game?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
		return;
	}
	m_puzzle->generate();
}

/*****************************************************************************/

void Board::resizeEvent(QResizeEvent* event) {
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

/*****************************************************************************/

int Board::getCurColor()
{
	return std::min(QSettings().value("Colors").toInt(), colorsMax);
}
int Board::getCurStyle()
{
	return std::min(QSettings().value("Styles").toInt(), stylesMax);
}

//  colors
void Board::colors0() {  colorsSet(0);  }
void Board::colors1() {  colorsSet(1);  }
void Board::colors2() {  colorsSet(2);  }
void Board::colors3() {  colorsSet(3);  }
void Board::colors4() {  colorsSet(4);  }
void Board::colors5() {  colorsSet(5);  }

void Board::colorsNext() {
	int clr = getCurColor();
	++clr;
	if (clr > colorsMax)
		clr = 0;
	colorsSet(clr);
}

void Board::colorsSet(int val) {
	QSettings().setValue("Colors", val);
	m_puzzle->redraw();

	for (int i=0; i < actionsColor.size(); ++i)
		actionsColor[i]->setChecked(i == val);
}

//  styles
void Board::styles0() {  stylesSet(0);  }
void Board::styles1() {  stylesSet(1);  }
void Board::styles2() {  stylesSet(2);  }
void Board::styles3() {  stylesSet(3);  }
void Board::styles4() {  stylesSet(4);  }
void Board::styles5() {  stylesSet(5);  }
void Board::styles6() {  stylesSet(6);  }
void Board::styles7() {  stylesSet(7);  }
void Board::styles8() {  stylesSet(8);  }
void Board::styles9() {  stylesSet(9);  }

void Board::stylesNext() {
	int style = getCurStyle();
	++style;
	if (style > stylesMax)
		style = 0;
	stylesSet(style);
}

void Board::stylesSet(int val) {
	QSettings().setValue("Styles", val);
	m_puzzle->redraw();

	for (int i=0; i < actionsStyle.size(); ++i)
		actionsStyle[i]->setChecked(i == val);
}

/*****************************************************************************/
