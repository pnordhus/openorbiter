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


#include "config.h"
#include "map.h"
#include "match.h"
#include "openorbiter.h"
#include "player.h"
#include "graphicsscene.h"


#include <QDebug>
#include <QGraphicsScene>
#include <QRect>


OpenOrbiter::OpenOrbiter() :
	m_paused(true),
	m_match(NULL),
	m_graphicsScene(NULL)
{
	m_time.start();
}


OpenOrbiter::~OpenOrbiter()
{
	delete m_match;

	g_config.save(g_config.userDir() + "config.xml");

	for (int i = 0; i < MaxPlayers; i++)
		delete m_players[i];

	foreach (Map* map, m_maps)
		delete map;

	delete m_graphicsScene;
}


QList<Player*> OpenOrbiter::selectedPlayers()
{
	QList<Player*> ret;
	for (int i = 0; i < MaxPlayers; i++)
		if (m_players[i]->getSelected())
			ret.append(m_players[i]);
	return ret;
}


void OpenOrbiter::startMatch(const Match& match)
{
	Q_ASSERT(m_match == NULL);
	m_paused = true;
	delete m_match;
	m_match = new Match(match);

	m_lastMap = m_match->map();
	g_config.setLastMap(m_lastMap->name());

	m_match->startGame();
}


void OpenOrbiter::pause()
{
//	qDebug("paused");
	m_paused = true;
}


void OpenOrbiter::resume()
{
//	qDebug("resumed");
	m_paused = false;
}


bool OpenOrbiter::process()
{
	m_frameTime = float(m_time.restart()) / 1000.0f;
	Q_ASSERT(m_match);

	if (!m_paused)
		m_match->process(m_frameTime);

	return true;
}


void OpenOrbiter::init(bool load)
{
	initPlayers();

	if (load)
		loadConfig();

	loadMaps();

	qDebug().nospace() << "Loading completed. " << m_maps.size() << ((m_maps.size() == 1) ? " map" : " maps") << " found.";

	m_graphicsScene = new GraphicsScene;
	m_graphicsScene->setSize(10.0f, 5.0f);
	m_graphicsScene->setBackgroundColor(g_config.mapColor());
}


void OpenOrbiter::initPlayers()
{
	m_players[0] = new Player(0, "Player 1", Qt::red);
	m_players[1] = new Player(1, "Player 2", Qt::green);
	m_players[2] = new Player(2, "Player 3", Qt::blue);
	m_players[3] = new Player(3, "Player 4", "fuchsia");
	m_players[4] = new Player(4, "Player 5", Qt::yellow);
	m_players[5] = new Player(5, "Player 6", Qt::cyan);
	m_players[6] = new Player(6, "Player 7", "yellowgreen");
	m_players[7] = new Player(7, "Player 8", "maroon");
}


void OpenOrbiter::loadConfig()
{
	QString file;

	file = g_config.dataDir() + "config.xml";
	if (QFileInfo(file).isReadable())
		g_config.load(file);

	file = g_config.userDir() + "config.xml";
	if (QFileInfo(file).isReadable())
		g_config.load(file);
}


void OpenOrbiter::loadMaps()
{
	m_maps.clear();

	loadMapsFromDir(g_config.dataDir() + "maps");
	loadMapsFromDir(g_config.userDir() + "maps");

	qStableSort(m_maps);
	
	// remove duplicate maps
	for (int i = m_maps.size() - 2; i >= 0; i--) {
		if (m_maps.value(i) == m_maps.value(i + 1)) {
			delete m_maps.takeAt(i + 1);
		}
	}

	m_lastMap = NULL;
	foreach (Map* map, m_maps) {
		if (map->name() == g_config.lastMap()) {
			m_lastMap = map;
			break;
		}
	}	
}


void OpenOrbiter::loadMapsFromDir(const QDir& dir)
{
	qDebug() << "Looking for maps in" << dir.path();

	QStringList cfgFiles = dir.entryList(QStringList("*.xml"), QDir::Files | QDir::Readable, QDir::Name);
	foreach (QString file, cfgFiles) {
		Map* map = Map::load(dir.path() + QDir::separator() + file);
		if (map)
			m_maps.append(map);
	}
}


/************************************************
            static initialization
************************************************/


OpenOrbiter* g_openorbiter = NULL;


void OpenOrbiter::create()
{
	Q_ASSERT(g_openorbiter == NULL);
	g_openorbiter = new OpenOrbiter;
}


void OpenOrbiter::destroy()
{
	Q_ASSERT(g_openorbiter != NULL);
	delete g_openorbiter;
}
