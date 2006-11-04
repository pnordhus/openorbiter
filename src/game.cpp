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


#include "config.h"
#include "game.h"
#include "map.h"
#include "node.h"
#include "openorbiter.h"
#include "player.h"
#include "randomizer.h"
#include "spawnpoint.h"


Game::Game(Map* map) :
	m_isOver(false),
	m_map(map),
	m_time(0.0),
	m_gravityFactor(1.0f)
{
	m_players = g_openorbiter->selectedPlayers();

	m_nodeTimer = g_config->firstNodeTime();

	m_nodes = m_map->nodes();
	randomize(m_nodes);

	SpawnPointList spawns = m_map->spawnPoints();
	randomize(spawns);
	randomize(m_players);

	foreach (Player* player, m_players) {
		player->startGame();
		player->getOrbiter().setPosition(*spawns.takeLast());
	}

	m_map->start();
}


Game::~Game()
{
	/* if there is more than 1 player left the game has been aborted */
	if (m_players.size() > 1)
		foreach (Player* player, m_players)
			player->endGame(false);

	m_map->stop();
}


void Game::process(float time)
{
	Q_ASSERT(m_isOver == false);
	m_time += time;

	unsigned int iterations = 10;

	if (m_nodes.size() > 1) {
		m_nodeTimer -= time;
		if (m_nodeTimer <= 4.0f)
			m_nodes.last()->setMark(int(m_nodeTimer * 2.0f) & 1);

		if (m_nodeTimer <= 0.0f) {
			m_nodeTimer = g_config->nextNodeTime();
			foreach (Player* player, m_players) {
				if (player->getOrbiter().node() == m_nodes.last())
					player->getOrbiter().setNode(NULL);
			}
			m_nodes.takeLast();
		}
	} else {
		m_gravityFactor += time;
	}

	foreach (Player* player, m_players) {
		Orbiter& o = player->getOrbiter();
		if (o.needsNode()) {
			const Node* nearest = NULL;
			float distance = HUGE_VALF;
			foreach (const Node* node, m_nodes) {
				float d = o.getPosition().distance(*node);
				if (d < distance) {
					distance = d;
					nearest = node;
				}
			}
			Q_ASSERT(nearest != NULL);
			o.setNode(nearest);
		}
	}

	time /= float(iterations);

	randomize(m_players);

	for (unsigned int it = 0; it < iterations; it++) {
		Vector v = m_map->gravity() * g_config->gravityFactor() * m_gravityFactor * time;
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
			if ((p.x < 0.0) || (p.x >= m_map->width()) || (p.y < 0.0) || (p.y >= m_map->height())) {
				player->endGame(false);
				m_players.removeAt(i);
			}
		}

		if (m_players.size() == 1) {
			winner()->endGame(true);
	
			m_isOver = true;
			break;
		}
	}
}
