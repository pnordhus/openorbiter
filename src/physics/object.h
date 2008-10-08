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


#ifndef OBJECT_H
#define OBJECT_H


#include "vector.h"
#include <QObject>


class World;


class Object : public QObject
{
	Q_OBJECT

public:
	enum Type { Circle, Rect };

public:
	Type			type() const { return m_type; }
	const Vector&	position() const { return m_position; }
	const Vector&	speed() const { if (m_linked) recalcSpeed(); return m_speed; }
	float			linkSpeed() const { Q_ASSERT(m_linked); return m_linkSpeed; }
	float			mass() const { return m_mass; }
	void			accelerate(const Vector& acc);
	void			accelerate(float acc);
	void			move(float time);
	void			move(const Vector& dir);
	void			setWorld(World* world);
	void			link(const Vector& pos);
	void			unlink();
	bool			isLinked() const { return m_linked; }
	void			collide(bool timer);
	float			radius() const { return m_radius; }
	void			setStatic(bool enable);
	bool			isStatic() const { return m_static; }

protected:
	void			setPosition(const Vector& pos);
	void			setRadius(float radius);

signals:
	void			collided(bool timer);

private:
	void			recalcSpeed() const;

protected:
	Object(Type type);
	virtual ~Object();

private:
	const Type	m_type;
	World*		m_world;
	Vector		m_position;
	mutable Vector	m_speed;
	float		m_mass;
	Vector		m_linkPos;
	bool		m_linked;
	float		m_linkSpeed;
	float		m_linkRadius;
	float		m_radius;
	bool		m_static;

private:
	Q_DISABLE_COPY(Object);
};


#endif // OBJECT_H
