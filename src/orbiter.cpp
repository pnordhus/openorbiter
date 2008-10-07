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


#include "game.h"
#include "node.h"
#include "orbiter.h"
#include "rendermanager.h"
#include "scene.h"
#include "physics/circle.h"


Orbiter::Orbiter(Player* player) :
	m_player(player),
	m_game(NULL),
	m_node(NULL),
	m_connected(false),
	m_collisionTimer(0.0f)
{
	const float radius = 0.5f; 
	
	m_item = RenderManager::get().createOrbiterItem(radius, color());
	m_item->setZValue(100);
	
	m_connectionLine = new QGraphicsLineItem;
	m_connectionLine->setZValue(5);
	m_connectionLine->hide();
	
	m_circle = new Circle;
	m_circle->setRadius(radius);
	connect(m_circle, SIGNAL(collided(bool)), SLOT(collide(bool)));
}


Orbiter::~Orbiter()
{
	delete m_connectionLine;
	delete m_circle;
	delete m_item;
}


void Orbiter::enter(Game& game, World* world, Scene& scene)
{
	Q_ASSERT(m_game == NULL);
	
	m_game = &game;
	m_circle->setWorld(world);
	
	if (m_item->scene()) {
		m_item->scene()->removeItem(m_item);
		m_connectionLine->scene()->removeItem(m_connectionLine);
	}
	
	scene.addItem(m_item);
	scene.addItem(m_connectionLine);
}


void Orbiter::leave()
{
	Q_ASSERT(m_game != NULL);
	
	m_circle->setWorld(NULL);
	
	if (m_item->scene()) {
		m_item->scene()->removeItem(m_item);
		m_connectionLine->scene()->removeItem(m_connectionLine);
	}
	
	m_game = NULL;
}


void Orbiter::toggleConnect()
{
	if (m_node)
		disconnectNode();
	else
		connectNode();
}


void Orbiter::collide(bool timer)
{
	disconnectNode();
	if (timer)
		m_collisionTimer = 0.75f;
}


void Orbiter::connectNode()
{
	m_connectionLine->setPen(QPen(Qt::darkRed, 0.1f));
	
	m_node = m_game->nearestNode(m_circle->position());
	m_connected = false;
}


void Orbiter::disconnectNode()
{
	m_connectionLine->hide();
	m_connected = false;
	m_node = NULL;
	m_circle->unlink();
}


void Orbiter::process(float time)
{
	m_collisionTimer -= time;
	
	if (m_node && !m_connected && (m_collisionTimer <= 0.0f)) {
		m_connected = true;
		m_connectionLine->setPen(QPen(Qt::white, 0.1f));
		m_circle->link(m_node->position());
	}
}


Vector Orbiter::position() const
{
	return m_circle->position();
}


float Orbiter::radius() const
{
	return m_circle->radius();
}


void Orbiter::update()
{
	if (m_node) {
		m_connectionLine->setLine(m_circle->position().x, m_circle->position().y, m_node->position().x, m_node->position().y);
		m_connectionLine->show();
	}
	
	m_item->setPos(m_circle->position().x - radius(), m_circle->position().y - radius());
}


void Orbiter::setPosition(const Vector& pos)
{
	m_circle->setPosition(pos);
	m_item->setPos(pos.x - radius(), pos.y - radius());
}
