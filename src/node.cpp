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


#include "node.h"


#include <QGraphicsScene>


const float Node::m_radius = 0.3f;


Node::Node(float x, float y) :
	Vector(x, y),
	m_mark(false)
{
	m_item.setBrush(Qt::white);
	m_item.setZValue(20.0f);
	m_item.setRect(x - m_radius, y - m_radius, 2.0f * m_radius, 2.0f * m_radius);
}


Node::~Node()
{

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

	scene->addItem(&m_item);
}


void Node::disconnectScene()
{
	Q_ASSERT(m_item.scene());

	m_item.scene()->removeItem(&m_item);
}
