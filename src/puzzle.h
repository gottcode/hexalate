/*
	SPDX-FileCopyrightText: 2009-2021 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HEXALATE_PUZZLE_H
#define HEXALATE_PUZZLE_H

class Piece;

#include <QList>
#include <QGraphicsScene>
#include <QRandomGenerator>

class Puzzle : public QGraphicsScene
{
public:
	explicit Puzzle(QObject* parent = nullptr);
	~Puzzle();

	void generate();

	bool isDone();

	Piece* piece(int offset) const
	{
		return m_pieces.at(offset);
	}

	void swapPieces(Piece* piece1, Piece* piece2);

private:
	bool load();
	void save();
	void createBackground();
	void positionPieces();

private:
	QRandomGenerator m_random;
	QList<Piece*> m_pieces;
	bool m_done;
};

#endif // HEXALATE_PUZZLE_H
