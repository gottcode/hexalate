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

#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QSettings>
class Puzzle;

class Board : public QGraphicsView {
	Q_OBJECT
public:
	Board(QWidget* parent = 0);

	const static int colorsMax = 5;
	const static int stylesMax = 9;

	static int getCurColor();
	static int getCurStyle();

	QList<QAction*> actionsColor;  // for menu checkboxes
	QList<QAction*> actionsStyle;

public slots:
	void newGame();

	void colorsNext();
	void colors0(); void colors1(); void colors2(); void colors3();
	void colors4(); void colors5();

	void stylesNext();
	void styles0(); void styles1(); void styles2(); void styles3();
	void styles4(); void styles5(); void styles6(); void styles7();
	void styles8(); void styles9();

protected:
	virtual void resizeEvent(QResizeEvent* event);

private:
	Puzzle* m_puzzle;

	void colorsSet(int val);
	void stylesSet(int val);
};

#endif
