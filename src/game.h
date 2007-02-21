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


#ifndef OPENORBITER_GAME_H
#define OPENORBITER_GAME_H


#include "types.h"


class Map;
class Orbiter;
class Player;
class Vector;


class Game
{
public:
	Game(Map* map);
	~Game();

	void	process(float time);
	bool	isOver() { return m_isOver; }

	Player*	winner() { Q_ASSERT(m_players.size() == 1); return m_players.first(); }

	const QList<Player*>	players() const { return m_players; }
	const Map*				getMap() { return m_map; }
	const NodeList&			getNodes() { return m_nodes; }

	double	time() const { return m_time; }

private:
	bool			m_isOver;
	Map*			m_map;
	NodeList		m_nodes;
	float			m_nodeTimer;
	double			m_time;
	QList<Player*>	m_players;
	float			m_gravityFactor;
};


#endif
