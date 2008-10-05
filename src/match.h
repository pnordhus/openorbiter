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
class Map;
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
	void	setMaps(const QList<Map>& maps);

public slots:
	void	keyPressed(int key);
	void	pause();
	void	resume();

private slots:
	void	process();

private:
	void	newGame();
	Map		nextMap();
	bool	requestKey();

private:
	QList<Player*>	m_players;
	QQueue<Map>		m_maps;
	
	Scene&			m_scene;
	Game*			m_game;
	QTimer			m_timer;
	QTime			m_time;
	bool			m_paused;
	bool			m_gameOver;
	QString			m_countdownText;
	float			m_countdown;
	int				m_countdownInt;
	
	QList<Player*>	m_needKey;
};


#endif // MATCH_H