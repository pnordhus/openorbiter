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


#include "../build/configure.h"
#include "config.h"
#include "openorbiter.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QVariant>


Config::Config()
{
	m_dataDir = QDir(DATADIR).path() + QDir::separator();
	m_userDir = QDir::homePath() + QDir::separator();
#ifdef WIN32
	m_userDir += "OpenOrbiter\\";
#else
	m_userDir += ".openorbiter/";
#endif

	m_gravityFactor = 1.0f;

	m_windowPosX = 0;
	m_windowPosY = 0;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_windowMaximized = false;
	m_windowFullScreen = false;
	m_windowShowStats = true;

	m_firstNodeTime = 60.0f;
	m_nextNodeTime = 10.0f;

	m_mapColor = QColor(250,250,250);

	m_statsVisibility.clear();
	m_statsVisibility.append(false);
	m_statsVisibility.append(true);
	m_statsVisibility.append(true);
	m_statsVisibility.append(true);
	m_statsVisibility.append(false);
	m_statsVisibility.append(false);
	m_statsVisibility.append(false);
	m_statsVisibility.append(true);
}


void Config::setWindowPosX(int x)
{
	m_windowPosX = x;
}


void Config::setWindowPosY(int y)
{
	m_windowPosY = y;
}


void Config::setWindowWidth(int w)
{
	m_windowWidth = w;
}


void Config::setWindowHeight(int h)
{
	m_windowHeight = h;
}


void Config::setWindowMaximized(bool m)
{
	m_windowMaximized = m;
}


void Config::setWindowFullScreen(bool f)
{
	m_windowFullScreen = f;
}


void Config::setWindowShowStats(bool s)
{
	m_windowShowStats = s;
}


void Config::setLastMap(const QString& map)
{
	m_lastMap = map;
}


void Config::setStatsVisibility(const QList<bool>& list)
{
	Q_ASSERT(list.size() == 8);
	m_statsVisibility = list;
}


void Config::setMapColor(const QColor& color)
{
	m_mapColor = color;
}


// ***************************************************************************
// save configuration to xml file


void Config::save(const QString& filename)
{
	QDomDocument doc;

	QDomElement root = doc.createElement("ooconfig");
	doc.appendChild(root);

	saveGame(doc, root);
	savePlayers(doc, root);
	saveWindow(doc, root);

	QFile file(filename);
	file.open(QFile::WriteOnly);
	QTextStream s(&file);
	doc.save(s, 4);
}


#define SET_TEXT(base, name, val) \
{ \
	QDomElement tmp = doc.createElement(name); \
	base.appendChild(tmp); \
	tmp.appendChild(doc.createTextNode(val)); \
}


void Config::saveGame(QDomDocument& doc, QDomElement& root)
{	
	QDomElement game = doc.createElement("game");
	root.appendChild(game);

	game.setAttribute("gravity", m_gravityFactor);

	SET_TEXT(game, "lastMap", m_lastMap);
}


void Config::saveWindow(QDomDocument& doc, QDomElement& root)
{
	QDomElement window = doc.createElement("window");
	root.appendChild(window);

	window.setAttribute("posX", m_windowPosX);
	window.setAttribute("posY", m_windowPosY);
	window.setAttribute("width", m_windowWidth);
	window.setAttribute("height", m_windowHeight);
	window.setAttribute("maximized", QVariant(m_windowMaximized).toString());
	window.setAttribute("fullscreen", QVariant(m_windowFullScreen).toString());

	{
		QDomElement stats = doc.createElement("stats");
		window.appendChild(stats);
		stats.setAttribute("show", QVariant(m_windowShowStats).toString());

		SET_TEXT(stats, "avgSpeed",	QVariant(m_statsVisibility[0]).toString());
		SET_TEXT(stats, "frags",	QVariant(m_statsVisibility[1]).toString());
		SET_TEXT(stats, "key",		QVariant(m_statsVisibility[2]).toString());
		SET_TEXT(stats, "losses",	QVariant(m_statsVisibility[3]).toString());
		SET_TEXT(stats, "playTime",	QVariant(m_statsVisibility[4]).toString());
		SET_TEXT(stats, "topSpeed", QVariant(m_statsVisibility[5]).toString());
		SET_TEXT(stats, "way",		QVariant(m_statsVisibility[6]).toString());
		SET_TEXT(stats, "wins",		QVariant(m_statsVisibility[7]).toString());
	}
}


void Config::savePlayers(QDomDocument& doc, QDomElement& root)
{
	QDomElement players = doc.createElement("players");
	root.appendChild(players);

	for (int i = 0; i < OpenOrbiter::MaxPlayers; i++) {
		QDomElement player = doc.createElement("player");
		players.appendChild(player);

		player.setAttribute("id", g_openorbiter->getPlayer(i).getID());
		player.setAttribute("name", g_openorbiter->getPlayer(i).getName());
		player.setAttribute("selected", QVariant(g_openorbiter->getPlayer(i).getSelected()).toString());
	}
}


// ***************************************************************************
// load configuration from xml file


void Config::load(const QString& filename)
{
	QDomDocument doc;

	{
	    QFile file(filename);
    	if (!doc.setContent(&file)) {
			qWarning() << "Invalid XML file:" << filename;
    	    return;
		}
    }

	QDomElement root = doc.documentElement();
	if (root.tagName() != "ooconfig")
		return;

	loadGame(root);
	loadPlayers(root);
	loadWindow(root);
}


#define READ_ATTR_INT(base, name, var) { bool _b; int _ret = base.attribute(name).toInt(&_b); if(_b) var = _ret; }
#define READ_ATTR_FLOAT(base, name, var) { bool _b; float _ret = base.attribute(name).toFloat(&_b); if(_b) var = _ret; }
#define READ_ATTR_BOOL(base, name, var) var = QVariant(base.attribute(name)).toBool();

#define READ_BOOL(base, name, var) var = QVariant(base.firstChildElement(name).text()).toBool();
#define READ_STRING(base, name, var) var = base.firstChildElement(name).text();


void Config::loadGame(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("game");
	if (child.isNull())
		return;

	READ_ATTR_FLOAT(child, "gravity", m_gravityFactor);
	READ_STRING(child, "lastMap", m_lastMap);
}


void Config::loadPlayers(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("players");
	child = child.firstChildElement("player");
	while (!child.isNull()) {
		bool b;
		uint id = child.attribute("id").toUInt(&b);
		if (b && (id < OpenOrbiter::MaxPlayers)) {
			QString name = child.attribute("name");
			if (name.length() > 0) {
				bool selected = QVariant(child.attribute("selected")).toBool();
				Player& player = g_openorbiter->getPlayer(id);
				player.setName(name);
				player.setSelected(selected);
			}
		}
		child = child.nextSiblingElement("player");
	}
}


void Config::loadWindow(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("window");
	if (child.isNull())
		return;

	READ_ATTR_INT(child, "posX", m_windowPosX);
	READ_ATTR_INT(child, "posY", m_windowPosY);
	READ_ATTR_INT(child, "width", m_windowWidth);
	READ_ATTR_INT(child, "height", m_windowHeight);

	READ_ATTR_BOOL(child, "fullscreen", m_windowFullScreen);
	READ_ATTR_BOOL(child, "maximized", m_windowMaximized);

	{
		QDomElement tmp = child.firstChildElement("stats");
		if (!tmp.isNull()) {
			READ_ATTR_BOOL(tmp, "show", m_windowShowStats);
			
			READ_BOOL(tmp, "avgSpeed",	m_statsVisibility[0]);
			READ_BOOL(tmp, "frags",		m_statsVisibility[1]);
			READ_BOOL(tmp, "key",		m_statsVisibility[2]);
			READ_BOOL(tmp, "losses",	m_statsVisibility[3]);
			READ_BOOL(tmp, "playTime",	m_statsVisibility[4]);
			READ_BOOL(tmp, "topSpeed",	m_statsVisibility[5]);
			READ_BOOL(tmp, "way",		m_statsVisibility[6]);
			READ_BOOL(tmp, "wins",		m_statsVisibility[7]);
		}
	}
}
