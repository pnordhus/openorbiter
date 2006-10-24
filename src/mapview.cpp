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


#include "mapview.h"
#include "openorbiter.h"
#include <QGraphicsView>


MapView::MapView(QWidget* parent) :
	QGraphicsView(parent),
	m_view(NULL),
	m_map(NULL)
{
	m_scene.setBackgroundBrush(QColor(0,0,0));
	m_background = m_scene.addRect(QRectF());

	m_background->setZValue(-100.0f);

	{
		QBrush brush(QColor(200,200,200));
		m_background->setBrush(brush);

		QPen pen;
		pen.setWidth(0);
		m_background->setPen(pen);
	}
}


void MapView::setView(QGraphicsView* view)
{
	Q_ASSERT(m_view == NULL);
	m_view = view;
	m_view->setScene(&m_scene);
	setMap(NULL);
}


void MapView::setMap(const Map* map)
{
	Q_ASSERT(m_view != NULL);

	m_map = map;

	QRectF rect;

	if (m_map) {
		rect = QRectF(0.0f, 0.0f, m_map->width(), m_map->height());
	} else {
		rect = QRectF(0.0f, 0.0f, 200.0f, 100.0f);
	}

	m_background->setRect(rect);
	m_scene.setSceneRect(rect);
	updateSize();
}


void MapView::updateSize()
{
	Q_ASSERT(m_view != NULL);

	const qreal wScale = (qreal) (m_view->width() - 40) / m_background->rect().width();
	const qreal hScale = (qreal) (m_view->height() - 40) / m_background->rect().height();
	const qreal scale  = qMin(wScale, hScale);

	QMatrix mat;
	mat.scale(scale, scale);
	m_view->setMatrix(mat);
}


void MapView::resizeEvent(QResizeEvent*)
{
	updateSize();
}


void MapView::process()
{
	foreach (const Node* node, g_openorbiter->game()->getNodes()) {
		QRectF rect(node->x - 0.3f, node->y - 0.3f, 0.6f, 0.6f);
		//m_scene.addEllipse(rect, QPen(), QBrush(node->getColor()));
	}
}
