/*
	SPDX-FileCopyrightText: 2009-2021 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "puzzle.h"

#include "piece.h"

#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QPainterPath>
#include <QSettings>

#include <algorithm>

//-----------------------------------------------------------------------------

static int match(int offset)
{
	if (offset > 2) {
		return offset - 3;
	} else {
		return offset + 3;
	}
}

static const QList<QPointF> positions = QList<QPointF>()
	<< QPointF(110, 110)
	<< QPointF(110, 10)
	<< QPointF(196.6, 60)
	<< QPointF(196.6, 160)
	<< QPointF(110, 210)
	<< QPointF(23.4, 160)
	<< QPointF(23.4, 60);

//-----------------------------------------------------------------------------

Puzzle::Puzzle(QObject* parent)
	: QGraphicsScene(0, 0, 320, 320, parent)
	, m_random(QRandomGenerator::securelySeeded())
	, m_done(false)
{
	if (!QSettings().contains("Current/Piece1") || !load()) {
		generate();
	}
}

//-----------------------------------------------------------------------------

Puzzle::~Puzzle()
{
	QSettings().remove("Current");
	if (!m_done) {
		save();
	}
}

//-----------------------------------------------------------------------------

void Puzzle::generate()
{
	// Remove old game
	clear();
	m_pieces.clear();
	m_done = false;

	// Add board background
	createBackground();

	// Initialize pieces
	for (int i = 0; i < 7; ++i) {
		m_pieces.append(new Piece(this));
	}

	// Create first piece
	QList<int> colors = m_pieces.first()->colors();
	std::shuffle(colors.begin(), colors.end(), m_random);
	for (int i = 0; i < 6; ++i) {
		int color = colors.at(i);
		m_pieces.first()->setConnector(i, color);
		m_pieces.at(i + 1)->setConnector(match(i), color);
	}

	// Create ring of pieces
	for (int i = 1; i < 7; ++i) {
		int connector = match(i - 1);
		int offset = (connector > 0) ? connector - 1 : 5;
		int next = (i < 6) ? (i + 1) : 1;
		QList<int> colors1 = m_pieces.at(i)->colors();
		QList<int> colors2 = m_pieces.at(next)->colors();
		colors = colors1 + colors2;
		colors.erase(std::set_intersection(colors1.begin(), colors1.end(), colors2.begin(), colors2.end(), colors.begin()), colors.end());
		int color = colors.at(m_random.bounded(colors.count()));
		m_pieces.at(i)->setConnector(offset, color);
		m_pieces.at(next)->setConnector(match(offset), color);
	}

	// Randomly set remaining connectors
	for (int i = 1; i < 7; ++i) {
		Piece* piece = m_pieces.at(i);
		colors = piece->colors();
		std::shuffle(colors.begin(), colors.end(), m_random);
		for (int j = 0; j < 6; ++j) {
			if (piece->connector(j) == -1) {
				piece->setConnector(j, colors.takeFirst());
			}
		}
	}

	// Randomize board
	std::shuffle(m_pieces.begin(), m_pieces.end(), m_random);
	for (Piece* piece : qAsConst(m_pieces)) {
		piece->spin(m_random.bounded(6));
	}

	// Position pieces
	positionPieces();
}

//-----------------------------------------------------------------------------

bool Puzzle::isDone()
{
	if (!m_done) {
		// Check center
		for (int i = 0; i < 6; ++i) {
			if (m_pieces.first()->connector(i) != m_pieces.at(i + 1)->connector(match(i))) {
				return false;
			}
		}

		// Check ring
		for (int i = 1; i < 7; ++i) {
			int connector = match(i - 1);
			int offset = (connector > 0) ? connector - 1 : 5;
			int next = (i < 6) ? (i + 1) : 1;
			if (m_pieces.at(i)->connector(offset) != m_pieces.at(next)->connector(match(offset))) {
				return false;
			}
		}

		// Disable interaction with completed board
		for (int i = 0; i < 7; ++i) {
			m_pieces.at(i)->setFlag(QGraphicsItem::ItemIsMovable, false);
		}

		// Show success message
		QGraphicsTextItem* message = addText(tr("Success"), QFont("Sans", 24));
		QRectF rect = message->boundingRect();
		message->setDefaultTextColor(Qt::white);
		message->setPos(rect.height() * .25, rect.height() * .25);

		QPainterPath path;
		path.addRoundedRect(0, 0, rect.width() + rect.height() * .5, rect.height() * 1.5, 10, 10);
		QGraphicsPathItem* background = addPath(path, Qt::NoPen, QColor(0, 0, 0, 200));
		message->setParentItem(background);
		background->setZValue(1000);
		background->setPos(sceneRect().center() - background->boundingRect().center());

		// Store status for future checks
		m_done = true;
	}
	return true;
}

//-----------------------------------------------------------------------------

void Puzzle::swapPieces(Piece* piece1, Piece* piece2)
{
	int pos1 = m_pieces.indexOf(piece1);
	int pos2 = m_pieces.indexOf(piece2);
	Q_ASSERT(pos1 != -1 && pos2 != -1);
#if (QT_VERSION >= QT_VERSION_CHECK(5,13,0))
	m_pieces.swapItemsAt(pos1, pos2);
#else
	m_pieces.swap(pos1, pos2);
#endif
}

//-----------------------------------------------------------------------------

bool Puzzle::load()
{
	// Add board background
	createBackground();

	// Create pieces
	QSettings settings;
	bool success = true;
	for (int i = 0; i < 7; ++i) {
		Piece* piece = new Piece(this);
		success &= piece->fromString(settings.value(QString("Current/Piece%1").arg(i+1)).toString());
		m_pieces.append(piece);
	}
	if (!success) {
		QMessageBox::information(0, tr("Sorry"), tr("Unable to load previous game. Starting a new game."));
		return false;
	}

	// Position pieces
	positionPieces();

	return true;
}

//-----------------------------------------------------------------------------

void Puzzle::save()
{
	QSettings settings;
	for (int i = 0; i < 7; ++i) {
		settings.setValue(QString("Current/Piece%1").arg(i+1), m_pieces[i]->toString());
	}
}

//-----------------------------------------------------------------------------

void Puzzle::createBackground()
{
	QGraphicsItem* background = addEllipse(10, 10, 300, 300, QPen(Qt::darkGray, 2), Qt::gray);
	for (const QPointF& position : positions) {
		QGraphicsItem* item = addEllipse(0, 0, 70, 70, Qt::NoPen, Qt::darkGray);
		item->setPos(position + QPointF(15,15));
		item->setParentItem(background);
	}
}

//-----------------------------------------------------------------------------

void Puzzle::positionPieces()
{
	for (int i = 0; i < 7; ++i) {
		m_pieces[i]->setPosition(positions[i]);
	}
}

//-----------------------------------------------------------------------------
