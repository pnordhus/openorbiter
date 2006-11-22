/***************************************************************************
 *   Copyright (C) 2006 by Philipp Nordhus                                 *
 *   philipp.nordhus@web.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "graphicsscene.h"
#include "openorbiter.h"


#include <QDebug>


/****************************************************************************/


GraphicsScene::GraphicsScene()
{
	setItemIndexMethod(NoIndex);
	setBackgroundBrush(Qt::black);

	{
		QPen pen(Qt::white);
		pen.setWidthF(0.5f);
		pen.setJoinStyle(Qt::RoundJoin);

		m_background.setBrush(Qt::NoBrush);
		m_background.setPen(pen);
		m_background.setZValue(100.0f);
		addItem(&m_background);
	}

	{
		QPen pen(Qt::red);
		pen.setWidthF(0.75f);
		pen.setJoinStyle(Qt::RoundJoin);

		m_backgroundStatus.setBrush(Qt::NoBrush);
		m_backgroundStatus.setPen(pen);
		m_backgroundStatus.setZValue(0.0f);
		addItem(&m_backgroundStatus);
	}


	m_map.setPen(Qt::NoPen);
	m_map.setZValue(0.0f);
	addItem(&m_map);

	m_pauseTextBackground.setPen(Qt::NoPen);
	m_pauseTextBackground.setBrush(QColor(0, 0, 30, 150));
	m_pauseTextBackground.setZValue(10000.0f);

	m_pauseText.setDefaultTextColor(Qt::white);
	m_pauseText.setZValue(10010.0f);
	m_pauseText.setPlainText("Press Space to start a new game");

	addItem(&m_pauseText);
	addItem(&m_pauseTextBackground);

	connect(g_openorbiter, SIGNAL(pauseToggled(bool)), this, SLOT(togglePaused(bool)));
}


/****************************************************************************/


void GraphicsScene::setMapColor(const QColor& color)
{
	m_map.setBrush(color);
}


/****************************************************************************/


void GraphicsScene::setSize(float w, float h)
{
	const QRectF rect(0.0f, 0.0f, w, h);
	m_map.setRect(rect.adjusted(-0.1f, -0.1f, 0.2f, 0.2f));

	m_background.setRect(QRectF(-0.25f, -0.25f, w + 0.5f, h + 0.5f));
	m_backgroundStatus.setRect(QRectF(-0.5f, -0.5f, w + 1.0f, h + 1.0f));

	setSceneRect(rect.adjusted(-1.0f, -1.0f, 1.0f, 1.0f));
	updatePauseText();

	emit sizeChanged(w + 2.0f, h + 2.0f);
}


/****************************************************************************/


void GraphicsScene::updatePauseText()
{
	const float w = m_map.rect().width();
	const float h = m_map.rect().height();

	const float tw = m_pauseText.boundingRect().width();
	const float th = m_pauseText.boundingRect().height();
	const float scaleF = w / (tw + 20.0f);

	m_pauseText.setMatrix(QMatrix());
	m_pauseText.scale(scaleF, scaleF);
	m_pauseText.setPos(0.5f * (w - tw * scaleF), 0.5f * (h - th * scaleF));

	m_pauseTextBackground.setMatrix(m_pauseText.matrix());
	m_pauseTextBackground.setRect(m_pauseText.boundingRect());
	m_pauseTextBackground.setPos(m_pauseText.pos());
}


/****************************************************************************/


void GraphicsScene::setStatusColor(const QColor& color)
{
	QPen pen = m_backgroundStatus.pen();
	pen.setColor(color);
	m_backgroundStatus.setPen(pen);
}


/****************************************************************************/


void GraphicsScene::togglePaused(bool paused)
{
	m_pauseText.setPlainText("Press Space to resume");

	m_pauseText.setVisible(paused);
	m_pauseTextBackground.setVisible(paused);

	updatePauseText();
}
