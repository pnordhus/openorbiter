/***************************************************************************
 *   Copyright (C) 2008 Philipp Nordhus                                    *
 *   pnordhus@users.sourceforge.net                                        *
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


#include "rendermanager.h"
#include <QBrush>
#include <QPen>

#ifdef QT_SVG_LIB
#  include <QGraphicsSvgItem>
#endif


RenderManager* RenderManager::m_singleton = NULL;


QGraphicsItem* RenderManager::createNodeItem(float radius)
{
#ifdef QT_SVG_LIB
	if (m_rendererNode) {
		QGraphicsSvgItem* item = new QGraphicsSvgItem;
		item->setSharedRenderer(m_rendererNode);
		
		item->scale(0.01 * 2.0f * radius, 0.01 * 2.0f * radius);
		//TODO: disabling the cache resolves a mixing problem, find a better way
		item->setCacheMode(QGraphicsItem::NoCache);
		return item;
	}
#endif
	
	QGraphicsEllipseItem* item = new QGraphicsEllipseItem(0.0f, 0.0f, 2.0f * radius, 2.0f * radius);
	item->setPen(QPen(Qt::NoPen));
	item->setBrush(QBrush(Qt::white));
	return item;
}


QGraphicsItem* RenderManager::createOrbiterItem(float radius, const QColor& color)
{
#ifdef QT_SVG_LIB
	QSvgRenderer* renderer = m_rendererOrbiter.value(color.name());
	if (renderer) {
		QGraphicsSvgItem* item = new QGraphicsSvgItem;
		item->setSharedRenderer(renderer);
		item->scale(0.01 * 2.0f * radius, 0.01 * 2.0f * radius);
		
		//TODO: disabling the cache resolves a mixing problem, find a better way
		item->setCacheMode(QGraphicsItem::NoCache);
		return item;
	}
#endif
	
	QGraphicsEllipseItem* item = new QGraphicsEllipseItem(0.0f, 0.0f, 2.0f * radius, 2.0f * radius);
	item->setPen(QPen(Qt::NoPen));
	item->setBrush(QBrush(color));
	return item;
}


RenderManager::RenderManager()
{
#ifdef QT_SVG_LIB
	QSvgRenderer* renderer = new QSvgRenderer(QString(OO_DATADIR "/gfx/node.svg"));
	if (renderer->isValid()) {
		m_rendererNode = renderer;
	} else {
		m_rendererNode = NULL;
		delete renderer;
	}
	
	loadRenderer("red",			Qt::red);
	loadRenderer("green",		Qt::green);
	loadRenderer("blue",		Qt::blue);
	loadRenderer("yellow",		Qt::yellow);
	loadRenderer("cyan",		Qt::cyan);
	loadRenderer("magenta",		Qt::magenta);
	loadRenderer("darkYellow",	Qt::darkYellow);
	loadRenderer("lightGray",	Qt::lightGray);
#endif
}


RenderManager::~RenderManager()
{
#ifdef QT_SVG_LIB
	delete m_rendererNode;
	qDeleteAll(m_rendererOrbiter);
#endif
}


#ifdef QT_SVG_LIB
void RenderManager::loadRenderer(const QString& name, const QColor& color)
{
	QSvgRenderer* renderer = new QSvgRenderer(QString(OO_DATADIR "/gfx/%1.svg").arg(name));
	if (renderer->isValid())
		m_rendererOrbiter.insert(color.name(), renderer);
	else
		delete renderer;
}
#endif


void RenderManager::create()
{
	Q_ASSERT(!m_singleton);
	m_singleton = new RenderManager;
}


void RenderManager::destroy()
{
	Q_ASSERT(m_singleton);
	delete m_singleton;
	m_singleton = NULL;
}
