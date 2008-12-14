/***************************************************************************
 *   Copyright (C) 2004 by Philipp Nordhus                                 *
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


#include "match.h"
#include "orbiter.h"
#include "timer/Timer.h"
#include <GL/glu.h>


using namespace Mercury;


static GLUquadricObj* quadric = NULL;


Orbiter::Orbiter(Float x, Float y) :
	Collider(x, y, 1.0f, 40.0f, true, false)
{
	reset(x, y);
}


void Orbiter::toggleConnect()
{
	m_TryConnect = !m_TryConnect;
		
	if (m_TryConnect) {
		m_Node = Match::getMap().getNearestNode(*this);
		assert(m_Node);
	} else {
		m_Connected = false;
	}
}


void Orbiter::reset(Float x, Float y)
{
	Collider::reset(x, y);
	
	this->x = x;
	this->y = y;

	m_Speed.zero();
	m_Connected = false;
	m_TryConnect = false;
	m_Time = 0.0f;
	
	m_Node = NULL;

	m_OrbitRadius = 0.0f;
}


void Orbiter::process()
{
	Float time = Timer::frameTime * Match::getMap().getTimeFactor();
	
//	m_Radius -= 0.5 * time;
//	if (m_Radius < 20.0)
//		m_Radius = 20.0;

	if (m_Time > 0.0)
		m_Time -= Timer::frameTime;
		
	if (m_TryConnect) {
		if (!m_Node->isEnabled()) {
			m_TryConnect = false;
			m_Connected = false;
		}
	}

	if (!m_Connected && m_TryConnect && (m_Speed.magnitude() <= 3000.0f) && (m_Time <= 0.0f)) {
		m_Connected = true;
		m_OrbitRadius = (*m_Node - *this).magnitude();
		
		Vector p, r;
		p = *m_Node - *this;
		r.x = p.y;
		
		r.y = -p.x;
		r.z = p.z;
		r %= 1.0f;
		
		m_Speed = r * r.dot(m_Speed);
		m_Mag = m_Speed.magnitude();
	}
	
	if (m_Connected && (m_Speed.magnitude() > 3000.0f)) {
		m_Connected = false;
		m_TryConnect = false;
	}

	if (m_Connected) {
		Vector p, r;
		p = *m_Node - *this;
		r.x = p.y;
		r.y = -p.x;
		r.z = p.z;
		r %= 1.0f;
		
		m_Speed = r * r.dot(m_Speed);
		
		if ((m_Speed.x == 0.0f) && m_Speed.y == 0.0f)
			m_Speed.x = 0.0001f;
		m_Speed %= m_Mag;
		
		m_Mag += time * 75.0f;

		Vector new_pos, real_new_pos;
		new_pos = *this + m_Speed * Timer::frameTime;
		real_new_pos = new_pos - *m_Node;
		if (real_new_pos.magnitude() == 0.0f)
			real_new_pos.x += 0.00001f;
		real_new_pos %= m_OrbitRadius;
		real_new_pos += *m_Node;
		m_Speed = (real_new_pos - *this) / Timer::frameTime;
	} else {
		m_Speed.x -= m_Speed.x * time * 0.3f;
		
		if (m_TryConnect)
			m_Speed.y -= time * 100.0;
		else
			m_Speed.y += time * 750.0;
		
	}
	
	*this += m_Speed * Timer::frameTime;
}


void Orbiter::render()
{
	if (quadric == NULL) {
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
	}

       	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	
	glEnable(GL_LIGHTING);
	gluSphere(quadric, m_Radius, 40, 40);
	glDisable(GL_LIGHTING);

	if (m_Connected || m_TryConnect) {
	       	glLoadIdentity();
		if (m_Connected)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
			glVertex3f(m_Node->x, m_Node->y, -100.0f);
			glVertex3f(x, y, -100.0f);
		glEnd();
	}
}


void Orbiter::collide(Collider& collider)
{
	Collider::collide(collider);
	m_Connected = false;
	m_TryConnect = false;
	m_Time = 0.75f;
}
