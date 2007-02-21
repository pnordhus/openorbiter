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


class Game;
class GraphicsScene;
class Map;
class Match;
class Player;


class QSvgRenderer;


class OpenOrbiter : public QObject
{
	Q_OBJECT

public:
	enum {
		MaxPlayers = 8
	};

private:
	OpenOrbiter();
	~OpenOrbiter();

public:
	typedef	QList<Map*>		MapList;

	bool	isRunning() { return m_match != NULL; }
	bool	isPaused() { return m_paused; }

	void	pause();
	void	resume();
	void	togglePause();

	void	startMatch(const Match& match);

	void	init(bool loadConfig);

	Match*	match() { Q_ASSERT(m_match); return m_match; }

	QSvgRenderer*	nodeRenderer() { Q_ASSERT(m_nodeRenderer); return m_nodeRenderer; }

	static	void	create();
	static	void	destroy();

	const MapList&		getMaps() const { return m_maps; }
	Map*				getMap(int index) { return m_maps.at(index); }
	Player&				getPlayer(int index) { return *m_players[index]; }
	QList<Player*>		selectedPlayers();

	Map*	lastMap() { return m_lastMap; }

	GraphicsScene*	graphicsScene() { Q_ASSERT(m_graphicsScene != NULL); return m_graphicsScene; }

public slots:
	void	process();

signals:
	void	pauseToggled(bool);

private:
	void	initPlayers();
	void	loadConfig();
	void	loadMaps();
	void	loadMapsFromDir(const QDir& dir);

private:
	MapList	m_maps;

	bool	m_paused;
	QTime	m_time;
	float	m_frameTime;

	Match*	m_match;

	Player*	m_players[MaxPlayers];

	Map*	m_lastMap;
	GraphicsScene*	m_graphicsScene;
	QSvgRenderer*	m_nodeRenderer;
};


extern OpenOrbiter* g_openorbiter;


#endif
