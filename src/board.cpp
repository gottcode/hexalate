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
