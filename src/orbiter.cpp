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


#include "../build/configure.h"


#include "config.h"
#include "node.h"
#include "orbiter.h"
#include "player.h"


#include <QGraphicsEllipseItem>
#include <QGraphicsScene>


#ifdef USE_SVG
#  include <QGraphicsSvgItem>
#  include <QSvgRenderer>
#endif


Orbiter::Orbiter(Player& player, const QString& filename) :
	m_player(player),
	m_line(this, 0),
	m_svgFilename(filename),
	m_isSvg(false)
{
	m_line.setPen(QPen(Qt::white, 0.05f));
	m_line.setZValue(10.0f);

	m_item = new Graphic<QGraphicsEllipseItem>(this, 1);
	toggleSvg(g_config->svgEnabled());
	updateItem();

#ifdef USE_SVG
	connect(g_config, SIGNAL(svgChanged(bool)), this, SLOT(toggleSvg(bool)));
#endif
}


Orbiter::~Orbiter()
{
	delete m_item;
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

	updateItem();

	m_line.hide();
}


void Orbiter::process(float t, const Vector& gravity)
{
	if (m_collisionTimer > 0.0f)
		m_collisionTimer -= t;

	if (m_tryConnect && m_connectionNode) {
		if (!m_connected && (m_collisionTimer <= 0.0f) && ((m_position - *m_connectionNode).length() > 1.5f * m_radius)) {
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
//	m_item.setRect(-m_radius, -m_radius, 2.0f * m_radius, 2.0f * m_radius);
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


void Orbiter::connectScene(QGraphicsScene* scene)
{
	Q_ASSERT(scene != NULL);

	scene->addItem(m_item);
	scene->addItem(&m_line);
}


void Orbiter::disconnectScene()
{
	Q_ASSERT(m_item->scene());
	Q_ASSERT(m_line.scene());

	m_item->scene()->removeItem(m_item);
	m_line.scene()->removeItem(&m_line);
}


void Orbiter::updateItem()
{
#ifdef USE_SVG
	if (m_isSvg) {
		Graphic<QGraphicsSvgItem>* item = static_cast<Graphic<QGraphicsSvgItem>*>(m_item);
		item->setZValue(30.0f);
		item->setMatrix(QMatrix());
		item->setPos(-m_radius, -m_radius);
		item->scale(0.01 * 2.0f * m_radius, 0.01 * 2.0f * m_radius);
		m_lightAngle = 0.0f;
		return;
	}
#endif

	Graphic<QGraphicsEllipseItem>* item = static_cast<Graphic<QGraphicsEllipseItem>*>(m_item);
	item->setPen(Qt::NoPen);
	item->setZValue(30.0f);
	item->setRect(-m_radius, -m_radius, 2.0f * m_radius, 2.0f * m_radius);
	item->setBrush(m_player.getColor());
}


void Orbiter::update(int id)
{
	switch (id) {
	case 0:
		if (m_connectionNode) {
			QPen pen = m_line.pen();
			if (m_connected)
				pen.setColor(Qt::white);
			else
				pen.setColor(Qt::red);
			m_line.setPen(pen);
			m_line.setLine(m_position.x, m_position.y, m_connectionNode->x, m_connectionNode->y);
			m_line.show();
		} else {
			m_line.hide();
		}
		break;

	case 1:
		{
			Graphic<QGraphicsEllipseItem>* item = static_cast<Graphic<QGraphicsEllipseItem>*>(m_item);
			QRectF rect = item->rect();
			rect.moveTo(m_position.x - m_radius, m_position.y - m_radius);
			item->setRect(rect);
		}
		break;
#ifdef USE_SVG
	case 2:
		{
			float angle = 360.0f * asin(m_position.y / m_position.length()) / (2.0f * M_PI);
			m_item->rotate(angle - m_lightAngle);
			m_lightAngle = angle;

			QPointF p = m_item->mapToScene(50.0f, 50.0f);
			QPointF off = QPointF(m_position.x, m_position.y) - p;
			m_item->moveBy(off.x(), off.y());
		}
		break;
#endif
	}
}


#ifndef USE_SVG
void Orbiter::toggleSvg(bool) {}
#else
void Orbiter::toggleSvg(bool enable)
{
	QGraphicsScene* scene = m_item->scene();
	if (scene)
		scene->removeItem(m_item);

	delete m_item;
	m_item = NULL;

	if (enable) {
		Graphic<QGraphicsSvgItem>* item = new Graphic<QGraphicsSvgItem>(this, 2, g_config->dataDir() + "gfx/" + m_svgFilename);
		if (item->renderer()->isValid()) {
			m_item = item;
			m_isSvg = true;
		} else {
			delete item;
		}
	}

	if (m_item == NULL) {
		m_item = new Graphic<QGraphicsEllipseItem>(this, 1);
		m_isSvg = false;
	}

	updateItem();

	if (scene)
		scene->addItem(m_item);
}
#endif


template <typename T>
Orbiter::Graphic<T>::Graphic(Orbiter* orb, int id) :
	m_orbiter(orb),
	m_id(id)
{

}


template <typename T>
Orbiter::Graphic<T>::Graphic(Orbiter* orb, int id, const QString& filename) :
	T(filename),
	m_orbiter(orb),
	m_id(id)
{

}


template <typename T>
void Orbiter::Graphic<T>::advance(int phase)
{
	if (phase == 1)
		m_orbiter->update(m_id);
}
