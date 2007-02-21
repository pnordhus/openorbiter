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


OpenOrbiter::OpenOrbiter() :
	m_paused(false),
	m_match(NULL),
	m_graphicsScene(NULL),
	m_nodeRenderer(NULL)
{
	m_time.start();
}


OpenOrbiter::~OpenOrbiter()
{
	delete m_match;

	g_config->save(g_config->getString("userDir") + "config.xml");

	for (int i = 0; i < MaxPlayers; i++)
		delete m_players[i];

	qDeleteAll(m_maps);

#ifdef BUILD_SVG
	delete m_nodeRenderer;
#endif // BUILD_SVG

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
	//Q_ASSERT(m_match == NULL);
	delete m_match;
	m_match = new Match(match);

	m_lastMap = m_match->map();
	g_config->set("lastMap", m_lastMap->name());

	m_match->startGame();

	if (isPaused())
		resume();

	pause();
}


void OpenOrbiter::pause()
{
	Q_ASSERT(m_paused == false);

	m_paused = true;
	emit pauseToggled(m_paused);
}


void OpenOrbiter::resume()
{
	Q_ASSERT(m_paused == true);

	m_paused = false;
	emit pauseToggled(m_paused);
}


void OpenOrbiter::togglePause()
{
	m_paused = !m_paused;
	emit pauseToggled(m_paused);
}


void OpenOrbiter::process()
{
	if (!g_openorbiter->isRunning())
		return;

	m_frameTime = float(m_time.restart()) / 1000.0f;
	Q_ASSERT(m_match);

	if (!isPaused()) {
		m_match->process(m_frameTime);

		graphicsScene()->advance();
	}
}


void OpenOrbiter::init(bool load)
{
#ifdef BUILD_SVG
	m_nodeRenderer = new QSvgRenderer(g_config->getString("dataDir") + "gfx/node.svg");
#endif // BUILD_SVG

	initPlayers();

	if (load)
		loadConfig();

	loadMaps();

	qDebug().nospace() << "Loading completed. " << m_maps.size() << ((m_maps.size() == 1) ? " map" : " maps") << " found.";

	foreach (Map* map, m_maps)
		qDebug().nospace() << "  " << map->name() << " (" << map->filename() << ")";

	m_graphicsScene = new GraphicsScene;
	m_graphicsScene->setSize(50.0f, 25.0f);
	m_graphicsScene->setMapColor(g_config->getColor("mapColor"));
}


void OpenOrbiter::initPlayers()
{
	m_players[0] = new Player(0, "Player 1", Qt::red, "red.svg");
	m_players[1] = new Player(1, "Player 2", Qt::green, "green.svg");
	m_players[2] = new Player(2, "Player 3", Qt::blue, "blue.svg");
	m_players[3] = new Player(3, "Player 4", "fuchsia", "fuchsia.svg");
	m_players[4] = new Player(4, "Player 5", Qt::yellow, "yellow.svg");
	m_players[5] = new Player(5, "Player 6", Qt::cyan, "cyan.svg");
	m_players[6] = new Player(6, "Player 7", "yellowgreen", "yellowgreen.svg");
	m_players[7] = new Player(7, "Player 8", "maroon", "maroon.svg");
}


void OpenOrbiter::loadConfig()
{
	QString file;

	file = g_config->getString("dataDir") + "config.xml";
	if (QFileInfo(file).isReadable())
		g_config->load(file);

	file = g_config->getString("userDir") + "config.xml";
	if (QFileInfo(file).isReadable())
		g_config->load(file);
}


void OpenOrbiter::loadMaps()
{
	qDeleteAll(m_maps);
	m_maps.clear();
	m_lastMap = NULL;

	loadMapsFromDir(g_config->getString("userDir") + "maps");
	loadMapsFromDir(g_config->getString("dataDir") + "maps");
}


void OpenOrbiter::loadMapsFromDir(const QDir& dir)
{
	qDebug() << "Looking for maps in" << dir.path();

	QStringList cfgFiles = dir.entryList(QStringList("*.xml"), QDir::Files | QDir::Readable, QDir::Name);
	foreach (QString file, cfgFiles) {
		Map* map = Map::load(dir.path() + QDir::separator() + file);
		if (map) {
			MapList::iterator it = qLowerBound(m_maps.begin(), m_maps.end(), map, Map::Less());
			if ((it != m_maps.end()) && (**it == *map)) {
				delete map;
				continue;
			}
			m_maps.insert(it, map);
			if (map->name() == g_config->getString("lastMap"))
				m_lastMap = map;
		}
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
