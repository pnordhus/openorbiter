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


#include "match.h"
#include "openorbiter.h"
#include "randomizer.h"
#include <QDebug>


Match::Match(const Map& map) :
	m_map(map),
	m_time(0.0),
	m_lastWinner(NULL),
	m_game(NULL)
{
	m_players = g_openorbiter->selectedPlayers();
	foreach (Player* player, m_players) {
		player->startMatch();
	}

	newGame();
}


Match::~Match()
{
	delete m_game;

	foreach (Player* player, m_players) {
		player->endMatch();
	}
}


void Match::setPlayers(QList<Player*>& players)
{
	foreach (Player* player, m_players) {
		if (!players.contains(player))
			player->endMatch();
	}

	foreach (Player* player, players) {
		if (!m_players.contains(player))
			player->startMatch();
	}

	m_players = players;
}


void Match::newGame()
{
	delete m_game;
	m_game = new Game(m_map, m_players);
}


void Match::process(float time)
{
	m_time += time;

	if (!m_game->process(time)) { // game over
		m_lastWinner = m_game->winner();
		newGame();
	}
}
