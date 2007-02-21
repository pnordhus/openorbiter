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


#include "config.h"
#include "node.h"
#include "openorbiter.h"


const float Node::m_radius = 0.3f;


Node::Node(float x, float y) :
	Vector(x, y),
	m_mark(false),
	m_isSvg(false)
{
	m_item = new QGraphicsEllipseItem;

#ifdef BUILD_SVG
	setSvg("useSVG");
	connect(g_config, SIGNAL(changed(const QString&)), this, SLOT(setSvg(const QString&)));
#endif // BUILD_SVG

	updateItem();
}


Node::~Node()
{
	delete m_item;
}


void Node::setMark(bool b)
{
	m_mark = b;
}


QColor Node::getColor() const
{
	if (m_mark)
		return "red";

	return "white";
}


void Node::connectScene(QGraphicsScene* scene)
{
	Q_ASSERT(scene != NULL);

	scene->addItem(m_item);
}


void Node::disconnectScene()
{
	Q_ASSERT(m_item->scene());

	m_item->scene()->removeItem(m_item);
}


#ifdef BUILD_SVG
void Node::setSvg(const QString& name)
{
	if (name != "useSVG")
		return;

	bool enable = g_config->getBool("useSVG");

	QGraphicsScene* scene = m_item->scene();

	delete m_item;
	m_item = NULL;

	if (enable) {
		QSvgRenderer* renderer = g_openorbiter->nodeRenderer();
		if (renderer->isValid()) {
			QGraphicsSvgItem* item = new QGraphicsSvgItem;
			item->setSharedRenderer(renderer);
			m_item = item;
			m_isSvg = true;
		}
	}

	if (m_item == NULL) {
		m_item = new QGraphicsEllipseItem;
		m_isSvg = false;
	}

	updateItem();

	if (scene)
		scene->addItem(m_item);
}
#endif // BUILD_SVG


void Node::updateItem()
{
#ifdef BUILD_SVG
	if (m_isSvg) {
		QGraphicsSvgItem* item = static_cast<QGraphicsSvgItem*>(m_item);
		item->setMatrix(QMatrix());
		item->scale(0.01 * 2.0f * m_radius, 0.01 * 2.0f * m_radius);
		item->setZValue(20.0f);
		item->setPos(x - m_radius, y - m_radius);
		return;
	}
#endif // BUILD_SVG

	QGraphicsEllipseItem* item = static_cast<QGraphicsEllipseItem*>(m_item);
	item->setPen(Qt::NoPen);
	item->setBrush(Qt::white);
	item->setZValue(20.0f);
	item->setRect(x - m_radius, y - m_radius, 2.0f * m_radius, 2.0f * m_radius);
}
