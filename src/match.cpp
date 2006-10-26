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
#include "match.h"
#include "openorbiter.h"
#include "randomizer.h"
#include <QDebug>


Match::Match(Map* map) :
	m_map(map),
	m_time(0.0),
	m_lastWinner(NULL),
	m_game(NULL)
{

}


Match::~Match()
{
	delete m_game;
}


void Match::startGame()
{
	Q_ASSERT(m_game == NULL);
	m_game = new Game(m_map);
}


void Match::process(float time)
{
//	Q_ASSERT(m_game);

	m_time += time;
	m_game->process(time);

	if (m_game->isOver()) {
		m_lastWinner = m_game->winner();

		delete m_game;
		m_game = NULL;

		startGame();
	}

/*
	if (!m_game->process(time)) { // game over
		m_lastWinner = m_game->winner();
		newGame();
	}
*/
}
