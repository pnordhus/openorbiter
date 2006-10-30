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


#ifndef OPENORBITER_NODE_H
#define OPENORBITER_NODE_H


#include "vector.h"
#include <QColor>
#include <QGraphicsEllipseItem>


class Node : public Vector
{
public:
	Node(float x, float y);
	~Node();

	void	setMark(bool b);
	QColor	getColor() const;
	
	QGraphicsEllipseItem&	item() { return m_item; }

	void	connectScene(QGraphicsScene*);
	void	disconnectScene();

private:
	bool					m_mark;
	QGraphicsEllipseItem	m_item;

	static const float		m_radius;

private:
	Q_DISABLE_COPY(Node);
};


#endif
