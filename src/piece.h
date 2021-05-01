/***********************************************************************
 *
 * Copyright (C) 2009, 2010 Graeme Gott <graeme@gottcode.org>
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

#ifndef HEXALATE_PIECE_H
#define HEXALATE_PIECE_H

class Puzzle;

#include <QConicalGradient>
#include <QList>
#include <QGraphicsEllipseItem>
#include <QObject>
class QTimer;

class Piece: public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT

public:
	explicit Piece(Puzzle* puzzle);

	QList<int> colors() const
	{
		return m_colors;
	}

	int connector(int offset) const
	{
		return m_connectors.at(offset);
	}

	bool setConnector(int offset, int value);
	void setHighlight(bool highlight);
	void setPosition(const QPointF& position);
	void spin(int rotations);

	bool fromString(const QString& string);
	QString toString() const;

	// Enable qgraphicsitem_cast
	enum
	{
		Type = UserType + 1
	};

	int type() const override
	{
		return Type;
	}

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
	void rotateConnectors();
	void actionFinished();

private:
	void moveTo(const QPointF& new_pos);
	void rotate();

private:
	Puzzle* m_puzzle;
	QList<int> m_colors;
	QList<int> m_connectors;
	QConicalGradient m_gradient;
	QTimer* m_rotate_timer;
	int m_rotations;
	QPointF m_position;
	Piece* m_swap_piece;
	QPoint m_start_position;
	bool m_clicked;
};

#endif // HEXALATE_PIECE_H
