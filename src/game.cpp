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


#include "game.h"
#include "openorbiter.h"
#include "player.h"
#include "randomizer.h"


Game::Game(const Map& map, const QList<Player*>& p) :
	m_map(map),
	m_time(0.0),
	m_players(p),
	m_gravityFactor(1.0f)
{
	m_nodeTimer = g_openorbiter->config().firstNodeTime();

	m_nodes = m_map.getNodes();
	randomize(m_nodes);

	Map::SpawnPointList spawns = m_map.getSpawnPoints();
	randomize(spawns);
	randomize(m_players);

	foreach (Player* player, m_players) {
		player->startGame();
		player->getOrbiter().setPosition(spawns.takeLast());
	}
}


bool Game::process(float time)
{
	m_time += time;

	unsigned int iterations = 10;

	if (m_nodes.size() > 1) {
		m_nodeTimer -= time;
		if (m_nodeTimer <= 4.0f)
			m_nodes.last().setMark(int(m_nodeTimer * 2.0f) & 1);

		if (m_nodeTimer <= 0.0f) {
			m_nodeTimer = g_openorbiter->config().nextNodeTime();
			foreach (Player* player, m_players) {
				if (player->getOrbiter().node() == &m_nodes.last())
					player->getOrbiter().setNode(NULL);
			}
			m_nodes.takeLast();
		}
	} else {
		//m_gravityFactor *= 1.0f + time;
	}

	foreach (Player* player, m_players) {
		Orbiter& o = player->getOrbiter();
		if (o.needsNode()) {
			const Node* nearest = NULL;
			float distance = HUGE_VALF;
			foreach (const Node& node, m_nodes) {
				float d = o.getPosition().distance(node);
				if (d < distance) {
					distance = d;
					nearest = &node;
				}
			}
			Q_ASSERT(nearest != NULL);
			o.setNode(nearest);
		}
	}

	time /= float(iterations);

	randomize(m_players);

	for (unsigned int it = 0; it < iterations; it++) {
		Vector v = m_map.getGravity() * g_openorbiter->config().getGravityFactor() * m_gravityFactor * time;
		foreach (Player* player, m_players) {
			player->getOrbiter().process(time, v);
		}

		foreach (Player* p1, m_players) {
			foreach (Player* p2, m_players) {
				if (p1 != p2)
					Orbiter::collide(p1->getOrbiter(), p2->getOrbiter());
			}
		}

		for (int i = m_players.size() - 1; (i >= 0) && (m_players.size() > 1); i--) {
			Player* player = m_players.value(i);
			const Vector& p = player->getOrbiter().getPosition();
			if ((p.x < 0.0) || (p.x >= m_map.getWidth()) || (p.y < 0.0) || (p.y >= m_map.getHeight())) {
				player->endGame(false);
				m_players.removeAt(i);
			}
		}

		if (m_players.size() == 1) {
			winner()->endGame(true);
	
			return false;
		}
	}
	return true;
}


QPoint Game::drawingPos(const Vector& v, int width) const
{
	float factor = float(width) / m_map.getWidth();
	return QPoint(int(v.x * factor), int(v.y * factor));
}


QRect Game::drawingRect(const Orbiter& o, int width) const
{
	return drawingRect(o.getPosition(), o.getRadius(), width);
}


QRect Game::drawingRect(const Node& n, int width) const
{
	return drawingRect(n, 0.4f, width);
}


QRect Game::drawingRect(const Vector& pos, float radius, int width) const
{
	float factor = float(width) / m_map.getWidth();

	return QRect(int((pos.x - radius) * factor), int((pos.y - radius) * factor), int(2.0f * radius * factor), int(2.0f * radius * factor));
}
