/*
	SPDX-FileCopyrightText: 2009 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HEXALATE_BOARD_H
#define HEXALATE_BOARD_H

class Puzzle;

#include <QGraphicsView>

class Board : public QGraphicsView
{
	Q_OBJECT

public:
	explicit Board(QWidget* parent = nullptr);

public slots:
	void newGame();

protected:
	void resizeEvent(QResizeEvent* event) override;

private:
	Puzzle* m_puzzle;
};

#endif // HEXALATE_BOARD_H
