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


#ifndef MATCH_H
#define MATCH_H


#include <QColor>
#include <QQueue>
#include <QTime>
#include <QTimer>


class Game;
class MapDef;
class Orbiter;
class Player;
class Scene;


class Match : public QObject
{
	Q_OBJECT

public:
	Match(Scene& scene);
	~Match();

public:
	void	start();
	void	setPlayers(const QList<Player>& players);
	void	setMaps(const QList<MapDef>& maps);

public slots:
	void	keyPressed(int key);
	void	pause();

private slots:
	void	process();

private:
	void	updatePauseText();
	void	resume();
	void	newGame();
	MapDef	nextMap();
	bool	requestKey();

private:
	enum State { Setup, Pause, Ingame, Finished };

private:
	QList<Player*>	m_players;
	QQueue<MapDef>	m_maps;
	
	Scene&			m_scene;
	Game*			m_game;
	QTimer			m_timer;
	QTime			m_time;
	QString			m_textPrefix;
	
	State			m_state;
	
	QList<Player*>	m_needKey;
};


#endif // MATCH_H
