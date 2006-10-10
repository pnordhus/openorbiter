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


#include "openorbiter.h"
#include "config_parser.h"


#include <QDebug>


OpenOrbiter* g_openorbiter;


OpenOrbiter::OpenOrbiter() :
	m_paused(true),
	m_match(NULL)
{
	m_time.start();
}


OpenOrbiter::~OpenOrbiter()
{
	delete m_match;

	m_config.save(m_config.getUserDir() + "config.xml");

	for (int i = 0; i < MaxPlayers; i++)
		delete m_players[i];
}


QList<Player*> OpenOrbiter::selectedPlayers()
{
	QList<Player*> ret;
	for (int i = 0; i < MaxPlayers; i++)
		if (m_players[i]->getSelected())
			ret.append(m_players[i]);
	return ret;
}


void OpenOrbiter::newMatch(int map)
{
	m_paused = true;
	delete m_match;
	m_match = new Match(getMap(map));

	m_lastMap = map;
	m_config.lastMap(getMap(map).getName());
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
}


void OpenOrbiter::initPlayers()
{
	m_players[0] = new Player(0, "Player 1", "red");
	m_players[1] = new Player(1, "Player 2", "green");
	m_players[2] = new Player(2, "Player 3", "blue");
	m_players[3] = new Player(3, "Player 4", "fuchsia");
	m_players[4] = new Player(4, "Player 5", "yellow");
	m_players[5] = new Player(5, "Player 6", "cyan");
	m_players[6] = new Player(6, "Player 7", "yellowgreen");
	m_players[7] = new Player(7, "Player 8", "maroon");
}


void OpenOrbiter::addMap(const Map& map)
{
	m_maps.append(map);
}


void OpenOrbiter::loadConfig()
{
	ConfigParser cp;
	QString file;

	file = m_config.getDataDir() + "config.xml";
	if (QFileInfo(file).isReadable()) {
		m_config.load(file);
		cp.parse(file, ConfigParser::CONFIG | ConfigParser::PLAYERS);
	}

	file = m_config.getUserDir() + "config.xml";
	if (QFileInfo(file).isReadable()) {
		m_config.load(file);
		cp.parse(file, ConfigParser::CONFIG | ConfigParser::PLAYERS);
	}
}


void OpenOrbiter::loadMaps()
{
	m_maps.clear();

	loadMapsFromDir(m_config.getDataDir() + "maps");
	loadMapsFromDir(m_config.getUserDir() + "maps");

	qStableSort(m_maps);
	
	// remove duplicate maps
	for (int i = m_maps.size() - 2; i >= 0; i--) {
		if (m_maps.value(i) == m_maps.value(i + 1)) {
			m_maps.removeAt(i + 1);
		}
	}
	m_lastMap = 0;
	for (int i = 0; i < m_maps.size(); i++) {
		if (m_maps.value(i).getName() == m_config.lastMap()) {
			m_lastMap = i;
			break;
		}
	}	
}


void OpenOrbiter::loadMapsFromDir(const QDir& dir)
{
	ConfigParser cp;

	qDebug() << "Looking for maps in" << dir.path();

	QStringList cfgFiles = dir.entryList(QStringList("*.xml"), QDir::Files | QDir::Readable, QDir::Name);
	foreach (QString file, cfgFiles) {
		cp.parse(dir.path() + QDir::separator() + file, ConfigParser::MAP);
	}
}


/************************************************
            static initialization
************************************************/


bool OpenOrbiter::m_isCreated = false;


void OpenOrbiter::create()
{
	Q_ASSERT(m_isCreated == false);
	g_openorbiter = new OpenOrbiter;
}


void OpenOrbiter::destroy()
{
	delete g_openorbiter;
}
