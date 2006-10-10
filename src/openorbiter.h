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


#ifndef OPENORBITER_OPENORBITER_H
#define OPENORBITER_OPENORBITER_H


#include "config.h"
#include "match.h"
#include "map.h"
#include "player.h"

#include <QDir>
#include <QTime>


class OpenOrbiter
{
public:
	enum {
		MaxPlayers = 8
	};

private:
	OpenOrbiter();
	~OpenOrbiter();

public:
	typedef	QList<Map>		MapList;

	bool	isRunning() { return m_match != NULL; }
	bool	isPaused() { return m_paused; }

	void	pause();
	void	resume();

	void	newMatch(int map);

	bool	process();
	void	init(bool loadConfig);

	Config&	config() { return m_config; }
	Match*	match() { Q_ASSERT(m_match); return m_match; }
	Game*	game() { return m_match->game(); }

	static	void	create();
	static	void	destroy();

	const MapList&		getMaps() const { return m_maps; }
	const Map&			getMap(int index) const { return m_maps.at(index); }
	Player&				getPlayer(int index) { return *m_players[index]; }
	QList<Player*>		selectedPlayers();

	void	addMap(const Map& map);

	int		getLastMap() { return m_lastMap; }

private:
	void	initPlayers();
	void	loadConfig();
	void	loadMaps();
	void	loadMapsFromDir(const QDir& dir);

private:
	Config	m_config;
	MapList	m_maps;

	bool	m_paused;
	QTime	m_time;
	float	m_frameTime;

	Match*	m_match;

	Player*	m_players[MaxPlayers];

	static	bool	m_isCreated;

	int		m_lastMap;
};


extern OpenOrbiter* g_openorbiter;


#endif
