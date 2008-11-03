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


#include "node.h"
#include "rendermanager.h"
#include "scene.h"


const float Node::m_radius = 0.3f;


Node::Node(Scene& scene)
{
	m_item = RenderManager::get().createNodeItem(m_radius);
	m_item->setZValue(10);
	
	scene.addItem(m_item);
}


Node::~Node()
{
	delete m_item;
}


void Node::setPosition(const Vector& pos)
{
	m_item->setPos(pos.x - m_radius, pos.y - m_radius);
}


Vector Node::position() const
{
	return Vector(m_item->pos().x() + m_radius, m_item->pos().y() + m_radius);
}
