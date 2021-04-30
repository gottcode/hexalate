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

#ifndef HEXALATE_PUZZLE_H
#define HEXALATE_PUZZLE_H

#include <QList>
#include <QGraphicsScene>
class Piece;

class Puzzle : public QGraphicsScene
{
public:
	Puzzle(QObject* parent = nullptr);
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
	QList<Piece*> m_pieces;
	bool m_done;
};

#endif // HEXALATE_PUZZLE_H
