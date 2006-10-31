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


/****************************************************************************/


GraphicsScene::GraphicsScene()
{
	setItemIndexMethod(NoIndex);
	setBackgroundBrush(Qt::black);

	m_backgroundLeft.setBrush(Qt::white);
	m_backgroundLeft.setPen(Qt::NoPen);
	m_backgroundLeft.setZValue(100.0f);

	m_backgroundRight.setBrush(Qt::white);
	m_backgroundRight.setPen(Qt::NoPen);
	m_backgroundRight.setZValue(100.0f);

	m_backgroundBottom.setBrush(Qt::white);
	m_backgroundBottom.setPen(Qt::NoPen);
	m_backgroundBottom.setZValue(100.0f);

	m_backgroundTop.setBrush(Qt::white);
	m_backgroundTop.setPen(Qt::NoPen);
	m_backgroundTop.setZValue(100.0f);

	addItem(&m_backgroundLeft);
	addItem(&m_backgroundRight);
	addItem(&m_backgroundBottom);
	addItem(&m_backgroundTop);

	m_map.setPen(Qt::NoPen);
	m_map.setZValue(0.0f);
	addItem(&m_map);
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
	m_map.setRect(rect);

	m_backgroundLeft.setRect(QRectF(-1.0f, -1.0f, 1.0f, h + 2.0f));
	m_backgroundRight.setRect(QRectF(w, -1.0f, 1.0f, h + 2.0f));
	m_backgroundTop.setRect(QRectF(-1.0f, -1.0f, w + 2.0f, 1.0f));
	m_backgroundBottom.setRect(QRectF(-1.0f, h, w + 2.0f, 1.0f));

	setSceneRect(rect.adjusted(-1.0f, -1.0f, 1.0f, 1.0f));

	emit sizeChanged(w + 2.0f, h + 2.0f);
}
