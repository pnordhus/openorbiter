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


#include "object.h"
#include "world.h"


Object::Object(Type type) :
	m_type(type),
	m_world(NULL),
	m_mass(1.0f),
	m_linked(false),
	m_radius(0.0),
	m_static(false)
{
	
}


Object::~Object()
{
	if (m_world)
		m_world->unregisterObject(this);
}


void Object::setRadius(float radius)
{
	m_radius = radius;
}


void Object::setStatic(bool enable)
{
	m_static = enable;
}


void Object::setWorld(World* world)
{
	if (m_world)
		m_world->unregisterObject(this);
	
	m_world = world;
	
	if (m_world)
		m_world->registerObject(this);
}


void Object::setPosition(const Vector& pos)
{
	m_position = pos;
}


void Object::accelerate(const Vector& acc)
{
	if (m_linked) {
		accelerate(acc.length());
		return;
	}
	
	m_speed += acc;
	if (m_speed.length() > 50.0f)
		m_speed.setLength(50.0f);
}


void Object::accelerate(float acc)
{
	Q_ASSERT(m_linked);
	
	if (m_linkSpeed > 0.0f)
		m_linkSpeed += acc;
	else
		m_linkSpeed -= acc;
	
	m_linkSpeed = qBound(-50.0f, m_linkSpeed, 50.0f);
}


void Object::move(float time)
{
	if (m_linked) {
		const float d = m_linkSpeed * time;
		const float angle = d / m_linkRadius;
		const Vector newDir((m_position - m_linkPos).angle() + angle);
		m_position = m_linkPos + newDir * m_linkRadius;
	} else {
		m_position += m_speed * time;
	}
}


void Object::move(const Vector& dir)
{
	m_position += dir;
}


void Object::link(const Vector& pos)
{
	m_linkPos = pos;
	m_linkRadius = (m_position - pos).length(); 
	
	const float factor = std::sin(m_speed.angle() - (m_position - pos).angle());
	m_linkSpeed = m_speed.length() * factor;
	
	m_linked = true;
}


void Object::unlink()
{
	if (!m_linked)
		return;
	
	m_linked = false;
	recalcSpeed();
}


void Object::recalcSpeed() const
{
	const Vector center = m_linkPos - m_position;
	if (m_linkSpeed > 0.0f) {
		m_speed.x = center.y;
		m_speed.y = -center.x;
		m_speed.setLength(m_linkSpeed);
	} else {
		m_speed.x = -center.y;
		m_speed.y = center.x;
		m_speed.setLength(-m_linkSpeed);
	}
}


void Object::collide(bool timer)
{
	emit collided(timer);
}
