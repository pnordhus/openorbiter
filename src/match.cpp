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


#include "game.h"
#include "map.h"
#include "match.h"
#include "player.h"
#include "scene.h"
#include <algorithm>


Match::Match(Scene& scene) :
	m_scene(scene),
	m_game(NULL),
	m_paused(false),
	m_gameOver(true),
	m_countdown(0.0f)
{
	connect(&m_timer, SIGNAL(timeout()), SLOT(process()));
}


Match::~Match()
{
	qDeleteAll(m_players);
	delete m_game;
}


void Match::start()
{
	Q_ASSERT(m_players.size() >= 2);
	Q_ASSERT(m_maps.size() >= 1);
	
	newGame();
	requestKey();
}


void Match::pause()
{
	if (m_paused || m_gameOver)
		return;
	
	if (!m_needKey.isEmpty())
		return;
	
	m_paused = true;
	m_scene.showText("<center>Pause</center>");
}


void Match::resume()
{
	if (!m_paused || m_gameOver)
		return;
	
	if (!m_needKey.isEmpty())
		return;
	
	m_scene.hideText();
	m_paused = false;
}


void Match::keyPressed(int key)
{
	// key setup
	if (!m_needKey.isEmpty()) {
		if (key == Qt::Key_Space)
			return;
		
		foreach (const Player* player, m_players) {
			if (player->key() == key)
				return;
		}
		
		Player* player = m_needKey.takeFirst();
		player->setKey(key);
		
		if (!requestKey()) {
			m_scene.hideText();
			m_gameOver = false;
			m_timer.start(20);
			m_time.restart();
		}
		return;
	}
	
	if (key == Qt::Key_Space) {
		if (m_paused)
			resume();
		else
			pause();
		
		return;
	}
	
	// normal game
	if (m_game && !m_paused && !m_gameOver)
		m_game->keyPressed(key);
}


bool Match::requestKey()
{
	if (m_needKey.isEmpty())
		return false;
	
	Player* player = m_needKey.first();
	m_scene.showText(QString("<center><font color=\"%1\">%2</font><br>choose your key</center>").arg(player->color().name()).arg(player->name()));
	return true;
}


void Match::setPlayers(const QList<Player>& players)
{
	foreach (const Player& player, players) {
		Player* p = new Player(player);
		m_players.append(p);
		m_needKey.append(p);
	}
}


void Match::setMaps(const QList<Map>& maps)
{
	m_maps.clear();
	foreach (const Map& map, maps)
		m_maps.append(map);
	
	std::random_shuffle(m_maps.begin(), m_maps.end());
}


void Match::newGame()
{
	delete m_game;
	
	m_game = new Game(m_scene, nextMap(), m_players);
}


void Match::process()
{
	const float time = std::min(float(m_time.restart()) / 1000.0f, 0.5f);
	
	if (m_paused)
		return;
	
	if (m_countdown > 0.0f) {
		m_countdown -= time;
		int newVal = m_countdown + 1.0f;
		if (newVal == m_countdownInt)
			return;
		
		m_countdownInt = newVal;
		
		if (m_countdownInt > 0) {
			m_scene.showText(m_countdownText.arg(m_countdownInt));
		} else {
			m_scene.hideText();
			m_gameOver = false;
		}
		
		return;
	}
	
	const Player* winner = m_game->process(time); 
	if (winner) {
		m_gameOver = true;
		newGame();
		
		QString table;
		foreach (const Player* player, m_players) {
			table += QString("<tr><td><font color=\"%1\">%2</font></td><td width=\"20\"></td><td align=\"right\">%3</td></tr>").arg(player->color().name()).arg(player->name()).arg(player->wins());
		}
		
		m_countdownText = QString("<center><font color=\"%1\">%2</font> wins<br><font size=\"1\"><table>%3</table><br></font></center><center>New game starts in %4...</center>").arg(winner->color().name()).arg(winner->name()).arg(table);
		m_countdown = 3.0f;
		m_countdownInt = 3;
		m_scene.showText(m_countdownText.arg(m_countdownInt));
	}
}


Map Match::nextMap()
{
	Map map = m_maps.dequeue();
	m_maps.enqueue(map);
	return map;
}
