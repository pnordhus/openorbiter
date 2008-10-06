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


#ifndef WORLD_H
#define WORLD_H


#include "object.h"
#include "vector.h"
#include <QList>


class Circle;
class Rect;


class World
{
public:
	World();
	~World();

public:
	void	setGravity(const Vector& gravity);
	void	process(float time);

private:
	void	collide();
	void	collide(Object* obj1, Object* obj2);
	void	collide2(Circle* circle, Rect* rect);
	void	collide2(Circle* obj1, Circle* obj2);
	void	registerObject(Object* obj);
	void	unregisterObject(Object* obj);

private:
	QList<Object*>	m_objects;
	Vector			m_gravity;

private:
	Q_DISABLE_COPY(World);

friend class Object;
};


#endif // WORLD_H
