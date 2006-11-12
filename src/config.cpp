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
#include "player.h"


#include <QDir>
#include <QDomDocument>
#include <QVariant>


/****************************************************************************/


Config::Config()
{
	m_userDir = QDir::homePath() + QDir::separator();
#ifdef WIN32
	m_dataDir = "";
	m_userDir += "OpenOrbiter\\";
#else
	m_dataDir = QDir(DATADIR).path() + QDir::separator();
	m_userDir += ".openorbiter/";
#endif

	m_gravityFactor = 1.0f;

	m_windowMaximized = false;
	m_windowFullScreen = false;
	m_windowShowStats = true;

	m_firstNodeTime = 60.0f;
	m_nextNodeTime = 10.0f;
	
	m_svgEnabled = true;

	m_mapColor = QColor(250,250,250);
}


/****************************************************************************/


void Config::setWindowGeometry(const QRect& g)
{
	m_windowGeometry = g;
}


/****************************************************************************/


void Config::setWindowMaximized(bool m)
{
	m_windowMaximized = m;
}


/****************************************************************************/


void Config::setWindowFullScreen(bool f)
{
	m_windowFullScreen = f;
}


/****************************************************************************/


void Config::setWindowShowStats(bool s)
{
	m_windowShowStats = s;
}


/****************************************************************************/


void Config::setLastMap(const QString& map)
{
	m_lastMap = map;
}


/****************************************************************************/


void Config::setStatsShown(const StringBoolMap& map)
{
	m_statsShown = map;
}


/****************************************************************************/


void Config::setMapColor(const QColor& color)
{
	m_mapColor = color;
}


/****************************************************************************/


void Config::setSvgEnabled(bool enabled)
{
	bool changed = m_svgEnabled ^ enabled;
	m_svgEnabled = enabled;
	if (changed)
		emit svgChanged(enabled);
}


/****************************************************************************/


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


/****************************************************************************/


#define SET_TEXT(base, name, val) \
{ \
	QDomElement tmp = doc.createElement(name); \
	base.appendChild(tmp); \
	tmp.appendChild(doc.createTextNode(val)); \
}


/****************************************************************************/


void Config::saveGame(QDomDocument& doc, QDomElement& root)
{	
	QDomElement game = doc.createElement("game");
	root.appendChild(game);

	game.setAttribute("gravity", m_gravityFactor);

	SET_TEXT(game, "lastMap", m_lastMap);
	SET_TEXT(game, "mapColor", m_mapColor.name());
	SET_TEXT(game, "useSVG", QVariant(m_svgEnabled).toString());
}


/****************************************************************************/


void Config::saveWindow(QDomDocument& doc, QDomElement& root)
{
	QDomElement window = doc.createElement("window");
	root.appendChild(window);

	window.setAttribute("posX", m_windowGeometry.left());
	window.setAttribute("posY", m_windowGeometry.top());
	window.setAttribute("width", m_windowGeometry.width());
	window.setAttribute("height", m_windowGeometry.height());
	window.setAttribute("maximized", QVariant(m_windowMaximized).toString());
	window.setAttribute("fullscreen", QVariant(m_windowFullScreen).toString());

	{
		QDomElement stats = doc.createElement("stats");
		window.appendChild(stats);
		stats.setAttribute("show", QVariant(m_windowShowStats).toString());

		for (StringBoolMap::const_iterator it = m_statsShown.begin(); it != m_statsShown.end(); it++)
			SET_TEXT(stats, it.key(),	QVariant(it.value()).toString());
	}
}


/****************************************************************************/


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


/****************************************************************************/


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


/****************************************************************************/


#define READ_ATTR_INT_FN(base, name, _fn) { bool _b; int _ret = base.attribute(name).toInt(&_b); if(_b) _fn(_ret); }
#define READ_ATTR_INT(base, name, var) { bool _b; int _ret = base.attribute(name).toInt(&_b); if(_b) var = _ret; }
#define READ_ATTR_FLOAT(base, name, var) { bool _b; float _ret = base.attribute(name).toFloat(&_b); if(_b) var = _ret; }
#define READ_ATTR_BOOL(base, name, var) var = QVariant(base.attribute(name)).toBool();

#define READ_BOOL(base, name, var) var = QVariant(base.firstChildElement(name).text()).toBool();
#define READ_STRING(base, name, var) var = base.firstChildElement(name).text();


/****************************************************************************/


void Config::loadGame(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("game");
	if (child.isNull())
		return;

	READ_ATTR_FLOAT(child, "gravity", m_gravityFactor);
	READ_STRING(child, "lastMap", m_lastMap);

	QString tmp;

	READ_STRING(child, "mapColor", tmp);
	QColor color = tmp;
	if (color.isValid())
		m_mapColor = color;

//	READ_STRING(child, "useSVG", tmp);
//	if (tmp.length() > 0)
//		m_svgEnabled = QVariant(tmp).toBool();
}


/****************************************************************************/


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


/****************************************************************************/


void Config::loadWindow(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("window");
	if (child.isNull())
		return;

	READ_ATTR_INT_FN(child, "posX", m_windowGeometry.setLeft);
	READ_ATTR_INT_FN(child, "posY", m_windowGeometry.setTop);
	READ_ATTR_INT_FN(child, "width", m_windowGeometry.setWidth);
	READ_ATTR_INT_FN(child, "height", m_windowGeometry.setHeight);

	READ_ATTR_BOOL(child, "fullscreen", m_windowFullScreen);
	READ_ATTR_BOOL(child, "maximized", m_windowMaximized);

	{
		QDomElement tmp = child.firstChildElement("stats");
		if (!tmp.isNull()) {
			READ_ATTR_BOOL(tmp, "show", m_windowShowStats);

			QDomElement stat = tmp.firstChildElement();
			while (!stat.isNull()) {
				m_statsShown.insert(stat.tagName(), QVariant(stat.text()).toBool());
				stat = stat.nextSiblingElement();
			}
		}
	}
}


/****************************************************************************/


Config* g_config = NULL;


void Config::create()
{
	Q_ASSERT(g_config == NULL);
	g_config = new Config;
}


void Config::destroy()
{
	Q_ASSERT(g_config != NULL);
	delete g_config;
}
