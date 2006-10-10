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
#include "orbiter.h"
#include "player.h"


Orbiter::Orbiter(Player& player) :
	m_player(player)
{

}


void Orbiter::reset()
{
	m_tryConnect = false;
	m_connected = false;
	m_connectionNode = NULL;
	m_radius = 0.5f;
	m_mass = 1.0f;
	m_nodeRadius = 0.0f;
	m_angleSpeed = 0.0;
	m_collisionTimer = 0.0f;
	m_position = Vector();
	m_speed = Vector();
}


void Orbiter::process(float t, const Vector& gravity)
{
	if (m_collisionTimer > 0.0f)
		m_collisionTimer -= t;

	if (m_tryConnect && m_connectionNode) {
		if (!m_connected && (m_collisionTimer <= 0.0f)) {
			m_nodeRadius = m_connectionNode->distance(m_position);
			if (m_nodeRadius > 0.0f) {
				Vector p = m_position - *m_connectionNode;
				if (p.x == 0.0f) {
					m_angle = copysignf(0.5f, p.y) * M_PI;
				} else if (p.x > 0.0f) {
					m_angle = atanf(p.y / p.x);
				} else if (p.y >= 0.0f) {
					m_angle = atanf(p.y / p.x) + M_PI;
				} else {
					m_angle = atanf(p.y / p.x) - M_PI;
				}
				
				p = (m_position - *m_connectionNode).vertical();

				float directionFactor = 0.0f;
				if (!m_speed.isNull()) // if speed is 0 we would have null division
					directionFactor = m_speed.normalized().dot(p.normalized());

				m_angleSpeed = directionFactor * m_speed.length() / m_nodeRadius;
				m_connected = true;
			}
		}
	} else {
		m_connected = false;
	}

	float way;
	if (m_connected) {
		//qDebug() << m_angleSpeed;
		if (m_angleSpeed >= 0.0f)
			m_angleSpeed += 0.0001f;
		else
			m_angleSpeed -= 0.0001f;

		float angleDiff = m_angleSpeed *t;
		m_angle += angleDiff;
		while (m_angle >= 2.0f * M_PI)
			m_angle -= 2.0f * M_PI;

		while (m_angle < 0.0f)
			m_angle += 2.0f * M_PI;

		way = fabsf(angleDiff) * m_nodeRadius;

		m_position = *m_connectionNode + Vector(m_nodeRadius * cosf(m_angle), m_nodeRadius * sinf(m_angle));
		m_speed = (m_position - *m_connectionNode).vertical();
		m_speed.setLength(m_angleSpeed * m_nodeRadius);
	} else {
		m_speed += gravity;
		if (!gravity.isNull() && !m_speed.isNull()) {
			Vector wind = gravity.vertical().normalized();
			float l = wind.dot(m_speed);
			if (fabsf(l) > t * 0.3f)
				l = copysign(t * 0.3f, l);
			m_speed = m_speed - wind * l;
		}

		Vector v = m_speed * t;
		way = v.length();
		m_position += v;
	}

	m_player.matchStats().incTime(t);
	m_player.matchStats().incWay(way);
	m_player.matchStats().checkTopSpeed(way / t);
}


void Orbiter::setPosition(const Vector& pos)
{
	m_position = pos;
}


void Orbiter::toggle()
{
	m_tryConnect = !m_tryConnect;
	if (!m_tryConnect) {
		m_connectionNode = NULL;
	}
}


void Orbiter::setNode(const Node* node)
{
	m_connectionNode = node;
	if (!m_connectionNode) {
		m_tryConnect = false;
	}
}


bool Orbiter::needsNode() const
{
	return (m_tryConnect && (m_connectionNode == NULL));
}


void Orbiter::collide(Orbiter& orb1, Orbiter& orb2)
{
	Vector vec12 = orb2.getPosition() - orb1.getPosition();
	float distance = vec12.length();

	if (distance > orb1.m_radius + orb2.m_radius)
		return;

	if (distance == 0.0f) {
		vec12.x = 0.00001f;
		distance = vec12.length();
	}

	vec12.normalize();
	Vector vec21 = -vec12;

	

//	qDebug() << "collision" << distance;
        
	float rat12 = orb1.m_speed.dot(vec12);
	float rat21 = orb2.m_speed.dot(vec21);

	orb1.m_speed += (vec21 * rat21) - (vec12 * rat12);
	orb2.m_speed += (vec12 * rat12) - (vec21 * rat21);

	vec21 = vec21 * (((orb1.m_radius * 1.0) + (orb2.m_radius * 1.0) - distance) / 2.0 + 0.00001);
	vec12 = vec12 * (((orb1.m_radius * 1.0) + (orb2.m_radius * 1.0) - distance) / 2.0 + 0.00001);

	orb1.m_position += vec21;
	orb2.m_position += vec12;

	orb1.m_collisionTimer = 0.75f;
	orb2.m_collisionTimer = 0.75f;
	orb1.setNode(NULL);
	orb2.setNode(NULL);
}
