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

#ifndef HEXALATE_BOARD_H
#define HEXALATE_BOARD_H

#include <QGraphicsView>
class Puzzle;

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
