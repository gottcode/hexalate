/*
	SPDX-FileCopyrightText: 2009-2014 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "piece.h"

#include "puzzle.h"

#include <QApplication>
#include <QGraphicsItemAnimation>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPointF>
#include <QPolygonF>
#include <QRadialGradient>
#include <QTimeLine>
#include <QTimer>

//-----------------------------------------------------------------------------

Piece::Piece(Puzzle* puzzle)
	: QGraphicsEllipseItem(0, 0, 100, 100)
	, m_puzzle(puzzle)
	, m_gradient(50, 50, 90)
	, m_rotations(0)
	, m_swap_piece(nullptr)
	, m_clicked(false)
	, m_clicked_right(false)
{
	for (int i = 0; i < 6; ++i) {
		m_colors.append(i);
		m_connectors.append(-1);
	}
	setFlag(ItemIsMovable, true);
	setHighlight(false);

	for (int i = 0; i < 24; ++i) {
		m_gradient.setColorAt(static_cast<float>(i) / 24.0f, Qt::black);
	}

	m_rotate_timer = new QTimer(this);
	connect(m_rotate_timer, &QTimer::timeout, this, &Piece::rotateConnectors);
	m_rotate_timer->setInterval(40);

	m_puzzle->addItem(this);

	// Create middle of piece
	QGraphicsEllipseItem* overlay = new QGraphicsEllipseItem(0, 0, 100, 100, this);
	overlay->setPen(Qt::NoPen);
	QRadialGradient gradient(50, 50, 50);
	gradient.setColorAt(0.05, Qt::black);
	gradient.setColorAt(0.15, Qt::transparent);
	overlay->setBrush(gradient);
}

//-----------------------------------------------------------------------------

bool Piece::setConnector(int offset, int value)
{
	if (m_colors.contains(value)) {
		m_connectors[offset] = value;
		m_colors.removeOne(value);

		static QColor connector_colors[6] = {
			QColor(255, 0, 0),
			QColor(255, 255, 0),
			QColor(0, 255, 0),
			QColor(0, 255, 255),
			QColor(0, 0, 255),
			QColor(255, 0, 255)
		};
		m_gradient.setColorAt(static_cast<float>(6 - offset) / 6.0f, connector_colors[value]);
		if (offset == 0) {
			m_gradient.setColorAt(0, connector_colors[value]);
		}
		setBrush(m_gradient);

		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------

void Piece::setHighlight(bool highlight)
{
	setPen(QPen(highlight ? Qt::white : Qt::darkGray, 2));
	setZValue(highlight ? 2 : 1);
}

//-----------------------------------------------------------------------------

void Piece::setPosition(const QPointF& position)
{
	m_position = position;
	setPos(m_position);
}

//-----------------------------------------------------------------------------

void Piece::spin(int rotations)
{
	int angle = 90;
	for (int i = 0; i < rotations; ++i) {
		m_connectors.move(5, 0);
		angle -= 60;
	}
	if (angle < 0) {
		angle += 360;
	}
	m_gradient.setAngle(angle);
	setBrush(m_gradient);
}

//-----------------------------------------------------------------------------

bool Piece::fromString(const QString& string)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
	const QStringList values = string.split(",", Qt::SkipEmptyParts);
#else
	const QStringList values = string.split(",", QString::SkipEmptyParts);
#endif
	if (values.count() != 6) {
		return false;
	}

	for (int i = 0; i < 6; ++i) {
		if (!setConnector(i, values[i].toInt())) {
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------

QString Piece::toString() const
{
	QStringList values;
	for (int i = 0; i < 6; ++i) {
		values.append(QString::number(m_connectors[i]));
	}
	return values.join(",");
}

//-----------------------------------------------------------------------------

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	// Do not rotate because piece was dragged
	if ((m_start_position - event->screenPos()).manhattanLength() >= QApplication::startDragDistance()) {
		m_clicked = false;
	}

	// Find piece containing center
	Piece* swap_piece = nullptr;
	const QList<QGraphicsItem*> items = scene()->items(sceneBoundingRect().center());
	for (QGraphicsItem* item : items) {
		Piece* piece = qgraphicsitem_cast<Piece*>(item);
		if (piece && piece != this) {
			swap_piece = piece;
			break;
		}
	}

	// Highlight swap piece
	if (swap_piece) {
		if (swap_piece != m_swap_piece) {
			if (m_swap_piece) {
				m_swap_piece->setHighlight(false);
			}
			m_swap_piece = swap_piece;
			m_swap_piece->setHighlight(true);
		}
	} else {
		if (m_swap_piece) {
			m_swap_piece->setHighlight(false);
			m_swap_piece = nullptr;
		}
	}

	QGraphicsEllipseItem::mouseMoveEvent(event);
}

//-----------------------------------------------------------------------------

void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		setZValue(3);
		m_swap_piece = nullptr;
	}
	m_start_position = event->screenPos();
	m_clicked = true;
	m_clicked_right = event->button() == Qt::RightButton;
	QGraphicsEllipseItem::mousePressEvent(event);
}

//-----------------------------------------------------------------------------

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_swap_piece) {
		m_puzzle->swapPieces(this, m_swap_piece);
		qSwap(m_position, m_swap_piece->m_position);
		m_swap_piece->setHighlight(false);
		m_swap_piece->setZValue(2);
		m_swap_piece->moveTo(m_swap_piece->m_position);
		m_swap_piece = nullptr;
	} else if (m_clicked) {
		rotate();
		m_clicked = false;
	}
	moveTo(m_position);
	QGraphicsEllipseItem::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------

void Piece::rotateConnectors()
{
	float angle = m_gradient.angle();
	if (m_rotations > 0) {
		angle -= 20;
	} else {
		angle += 20;
	}
	if (angle < 0) {
		angle += 360;
	} else if (angle > 360) {
		angle -= 360;
	}
	m_gradient.setAngle(angle);
	setBrush(m_gradient);

	if (m_rotations > 0) {
		--m_rotations;
	} else if (m_rotations < 0) {
		++m_rotations;
	}
	if (m_rotations == 0) {
		m_rotate_timer->stop();
		actionFinished();
	}
}

//-----------------------------------------------------------------------------

void Piece::actionFinished()
{
	setZValue(1);
	if (!m_puzzle->isDone()) {
		setFlag(ItemIsMovable, true);
	}
}

//-----------------------------------------------------------------------------

void Piece::moveTo(const QPointF& new_pos)
{
	setFlag(ItemIsMovable, false);

	QTimeLine* timeline = new QTimeLine(200, this);

	QGraphicsItemAnimation* animation = new QGraphicsItemAnimation(this);
	animation->setItem(this);
	animation->setTimeLine(timeline);
	animation->setPosAt(0, pos());
	animation->setPosAt(1, new_pos);

	connect(timeline, &QTimeLine::finished, this, &Piece::actionFinished);
	connect(timeline, &QTimeLine::finished, animation, &QGraphicsItemAnimation::deleteLater);
	connect(timeline, &QTimeLine::finished, timeline, &QTimeLine::deleteLater);
	timeline->start();
}

//-----------------------------------------------------------------------------

void Piece::rotate()
{
	if (!m_clicked_right) {
		m_connectors.move(5, 0);
		m_rotations += 3;
	} else {
		m_connectors.move(0, 5);
		m_rotations -= 3;
	}
	if (!m_rotate_timer->isActive()) {
		m_rotate_timer->start();
		setFlag(ItemIsMovable, false);
	}
}

//-----------------------------------------------------------------------------
