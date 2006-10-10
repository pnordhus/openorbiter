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


#ifndef OPENORBITER_PLAYER_H
#define OPENORBITER_PLAYER_H


#include "orbiter.h"
#include "gamestats.h"
#include "matchstats.h"

#include <QString>
#include <QColor>
#include <QKeySequence>


class Player
{
public:
	Player(int id, const QString& name, const QColor& color);

	void			setName(const QString& name);
	void			setSelected(bool selected);

	int				getID() const { return m_id; }
	const QString&	getName() const { return m_name; }
	const QColor&	getColor() const { return m_color; }
	bool			getSelected() const { return m_isSelected; }

	Orbiter&		getOrbiter() { return m_orbiter; }
	const Orbiter&	getOrbiter() const { return m_orbiter; }

	void				setKey(const QKeySequence& key);
	const QKeySequence&	getKey() const { return m_key; }

	void				startMatch();	
	void				endMatch();	
	MatchStats&			matchStats() { return m_matchStats; }
	const MatchStats&	matchStats() const { return m_matchStats; }

	void				startGame();	
	void				endGame(bool won);
	GameStats&			gameStats() { return m_gameStats; }
	const GameStats&	gameStats() const { return m_gameStats; }

	void	toggle();

private:
	int				m_id;
	QString			m_name;
	Orbiter			m_orbiter;
	QColor			m_color;
	QKeySequence	m_key;
	bool			m_isSelected;
	bool			m_lost;
	bool			m_tryConnect;

	MatchStats		m_matchStats;
	GameStats		m_gameStats;

private:
	Player(const Player&);
	Player& operator = (const Player&);
};


#endif
