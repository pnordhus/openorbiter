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
#include "mapdef.h"
#include "match.h"
#include "orbiter.h"
#include "player.h"
#include "scene.h"
#include <algorithm>


Match::Match(Scene& scene) :
	m_scene(scene),
	m_game(NULL),
	m_state(Setup)
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
	if (m_state != Ingame)
		return;
	
	if (m_textPrefix == "")
		m_textPrefix = QString("<center>%1<br></center>").arg(tr("Pause"));
	
	m_state = Pause;
	m_timer.stop();
	
	m_needKey.clear();
	foreach (Orbiter* orb, m_game->orbiters())
		m_needKey.append(orb->player());
	
	updatePauseText();
}


void Match::updatePauseText()
{
	QRectF rect(0,0,5,5);
	
	QString table;
	foreach (Player* player, m_players) {
		QString name = player->name();
		if (!m_needKey.contains(player))
			name = "<u>" + name + "</u>";
		
		table += QString("<tr><td><font color=\"%1\">%2</font></td><td width=\"20\"></td><td align=\"right\">%3</td></tr>").arg(player->color().name()).arg(name).arg(player->wins());
	}
	
	m_scene.showText(m_textPrefix + QString("<center><font size=\"1\"><table>%1</table><br></font></center><center>%2</center>").arg(table).arg(tr("Press your keys")));
}


void Match::resume()
{
	Q_ASSERT(m_state != Ingame);
	
	m_state = Ingame;
	m_timer.start(30);
	m_time.restart();
	m_scene.hideText();
	m_textPrefix = "";
}


void Match::keyPressed(int key)
{
	switch (m_state) {
	case Setup:
		{
			if (key == Qt::Key_Space)
				break;
			
			foreach (const Player* player, m_players) {
				if (player->key() == key)
					return;
			}
			
			Q_ASSERT(!m_needKey.isEmpty());
			Player* player = m_needKey.takeFirst();
			player->setKey(key);
			
			if (!requestKey()) {
				resume();
			}
		}
		break;
	
	case Ingame:
		if (key == Qt::Key_Space)
			pause();
		
		m_game->keyPressed(key);
		break;
	
	case Pause:
		Q_ASSERT(!m_needKey.isEmpty());
		for (int i = m_needKey.size() - 1; i >= 0; i--) {
			if (m_needKey.at(i)->key() == key) {
				m_needKey.removeAt(i);
				if (m_needKey.isEmpty()) {
					resume();
					return;
				}
			}
		}
		
		updatePauseText();
		break;
	
	default:
		break;
	}
}


bool Match::requestKey()
{
	if (m_needKey.isEmpty())
		return false;
	
	Player* player = m_needKey.first();
	m_scene.showText(QString("<center><font color=\"%1\">%2</font><br>%3</center>").arg(player->color().name()).arg(player->name()).arg(tr("choose your key")));
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


void Match::setMaps(const QList<MapDef>& maps)
{
	m_maps.clear();
	foreach (const MapDef& map, maps)
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
	
	Q_ASSERT(m_state == Ingame);
	
	const Player* winner = m_game->process(time); 
	if (winner) {
		m_textPrefix = QString("<center><font color=\"%1\">%2</font> %3<br></center>").arg(winner->color().name()).arg(winner->name()).arg(tr("wins"));
		newGame();
		pause();
	}
}


inline MapDef Match::nextMap()
{
	MapDef map = m_maps.dequeue();
	m_maps.enqueue(map);
	return map;
}
