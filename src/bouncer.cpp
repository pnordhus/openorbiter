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


#include "bouncer.h"
#include "bouncerdef.h"
#include "scene.h"


Bouncer::Bouncer(Scene& scene, World* world)
{
	m_item = new QGraphicsRectItem;
	m_item->setPen(Qt::NoPen);
	m_item->setBrush(QBrush(Qt::gray));
	m_item->setZValue(4);
	scene.addItem(m_item);
	
	m_itemCap1 = new QGraphicsEllipseItem;
	m_itemCap1->setPen(Qt::NoPen);
	m_itemCap1->setBrush(QBrush(Qt::gray));
	m_itemCap1->setZValue(3);
	scene.addItem(m_itemCap1);
	
	m_itemCap2 = new QGraphicsEllipseItem;
	m_itemCap2->setPen(Qt::NoPen);
	m_itemCap2->setBrush(QBrush(Qt::gray));
	m_itemCap2->setZValue(3);
	scene.addItem(m_itemCap2);
	
	m_rect = new Rect;
	m_rect->setWorld(world);
}


Bouncer::~Bouncer()
{
	delete m_rect;
	delete m_itemCap1;
	delete m_itemCap2;
	delete m_item;
}


void Bouncer::setDef(const BouncerDef& def)
{
	m_rect->setPositions(def.position1(), def.position2());
	m_rect->setWidth(def.width());
	m_rect->setBoost(def.boost());
	m_rect->setUnlink(def.unlink());
	
	updateItem();
}


void Bouncer::updateItem()
{
	const Vector& center = m_rect->position();
	
	const Vector dir = m_rect->position2() - m_rect->position1();
	
	const float a = m_rect->width();
	const float b = dir.length() / 2.0f;
	
	m_item->setPos(center.x, center.y);
	m_item->setRect(-b, -a, 2.0f * b, 2.0f * a);
	m_item->setTransform(QTransform());
	m_item->rotate(dir.angle() / M_PI * 180.0f);
	
	m_itemCap1->setPos(m_rect->position1().x,  m_rect->position1().y);
	m_itemCap1->setRect(-a, -a, 2.0f * a, 2.0f * a);
	
	m_itemCap2->setPos(m_rect->position2().x,  m_rect->position2().y);
	m_itemCap2->setRect(-a, -a, 2.0f * a, 2.0f * a);
}
