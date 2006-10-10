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


#ifndef OPENORBITER_MATCH_H
#define OPENORBITER_MATCH_H


#include "game.h"
#include "map.h"


class Orbiter;
class Player;


class Match
{
public:
	Match(const Map& map);
	~Match();

	void	setPlayers(QList<Player*>& players);

	void	process(float time);
	double	time() const { return m_time; }

	const Player*	lastWinner() const { return m_lastWinner; }
	Game*			game() { return m_game; }

private:
	void	newGame();
	
private:
	QList<Player*>	m_players;
	const Map&		m_map;
	double			m_time;
	Player*			m_lastWinner;
	Game*			m_game;
};


#endif
