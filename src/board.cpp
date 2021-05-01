/*
	SPDX-FileCopyrightText: 2009 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "board.h"

#include "puzzle.h"

#include <QMessageBox>

//-----------------------------------------------------------------------------

Board::Board(QWidget* parent)
	: QGraphicsView(parent)
{
	setRenderHint(QPainter::Antialiasing, true);
	setBackgroundBrush(Qt::lightGray);

	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumSize(320, 320);

	m_puzzle = new Puzzle(this);
	setScene(m_puzzle);
}

//-----------------------------------------------------------------------------

void Board::newGame()
{
	if (!m_puzzle->isDone() && QMessageBox::question(this, tr("Question"), tr("Abort current game?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
		return;
	}
	m_puzzle->generate();
}

//-----------------------------------------------------------------------------

void Board::resizeEvent(QResizeEvent* event)
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

//-----------------------------------------------------------------------------
