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
	QString userDir, dataDir;

#ifdef WIN32
	userDir = QDir(QString(getenv("APPDATA")) + QDir::separator() + "OpenOrbiter").path() + QDir::separator();
	dataDir = "";
#else
	userDir = QDir(QDir::homePath() + QDir::separator() + ".openorbiter").path() + QDir::separator();
	dataDir = QDir(DATADIR).path() + QDir::separator();
#endif
	QDir().mkpath(userDir);

	m_values.insert("userDir",			userDir);
	m_values.insert("dataDir",			dataDir);
	m_values.insert("lastMap",			QString());
	m_values.insert("mapColor",			QColor(0, 0, 10));

	m_values.insert("useSVG",			true);
	m_values.insert("useOpenGL",		true);

	m_values.insert("windowMaximized",	false);
	m_values.insert("windowFullScreen",	false);
	m_values.insert("windowShowStats",	true);

	m_values.insert("firstNodeTime",	60.0f);
	m_values.insert("nextNodeTime",		10.0f);
	m_values.insert("gravityFactor",	1.0f);

	m_values.insert("windowGeometry",	QRect());
}


/****************************************************************************/


QVariant Config::get(const QString& name) const
{
	ValueMap::const_iterator it = m_values.find(name);
	Q_ASSERT_X(it != m_values.end(),
		"get()",
		qPrintable(QString("Option '") + name + "' is invalid!"));

	return *it;
}


/****************************************************************************/


#define GET_FN_GEN(_type, _name) \
_type Config::get ## _name(const QString& name) const \
{ \
	return getType<_type>(name); \
}


GET_FN_GEN(bool,	Bool)
GET_FN_GEN(QColor,	Color)
GET_FN_GEN(float,	Float)
GET_FN_GEN(QRect,	Rect)
GET_FN_GEN(QString,	String)


/****************************************************************************/


template <typename T>
T Config::getType(const QString& name) const
{
	QVariant t;
	t.setValue(T());

	QVariant v = get(name);
	Q_ASSERT_X(v.canConvert<T>(),
		"get()",
		qPrintable(QString("Option '") + name + "' is not " + t.typeName() + ", but " + v.typeName()));

	return v.value<T>();
}


/****************************************************************************/


void Config::set(const QString& name, const QVariant& value)
{
	ValueMap::iterator it = m_values.find(name);
	Q_ASSERT_X(it != m_values.end(),
		"set()", qPrintable(QString("Option '") + name + "' is invalid!"));
	Q_ASSERT_X(value.canConvert(it->type()),
		"set()", qPrintable(QString("Option '") + name + "' is not " + it->typeName() + ", but " + value.typeName() + "!"));

	if (*it != value) {
		*it = value;
		emit changed(name);
	}
}


/****************************************************************************/


void Config::setStatsShown(const StringBoolMap& map)
{
	m_statsShown = map;
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

	game.setAttribute("gravity", get("gravityFactor").toString());

	SET_TEXT(game, "lastMap", getString("lastMap"));
//	SET_TEXT(game, "mapColor", m_mapColor.name());
	SET_TEXT(game, "useSVG", get("useSVG").toString());
}


/****************************************************************************/


void Config::saveWindow(QDomDocument& doc, QDomElement& root)
{
	QDomElement window = doc.createElement("window");
	root.appendChild(window);

	QRect g = getRect("windowGeometry");

	window.setAttribute("posX", g.left());
	window.setAttribute("posY", g.top());
	window.setAttribute("width", g.width());
	window.setAttribute("height", g.height());
	window.setAttribute("maximized", get("windowMaximized").toString());
	window.setAttribute("fullscreen", get("windowFullScreen").toString());

	{
		QDomElement stats = doc.createElement("stats");
		window.appendChild(stats);
		stats.setAttribute("show", get("windowShowStats").toString());

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
#define READ_ATTR_FLOAT(base, name, var) { bool _b; float _ret = base.attribute(name).toFloat(&_b); if(_b) set(var, _ret); }
#define READ_ATTR_BOOL(base, name, var) set(var, base.attribute(name));

//#define READ_BOOL(base, name, var) set(var, base.firstChildElement(name).text());
#define READ_STRING(base, name, var) var = base.firstChildElement(name).text();


/****************************************************************************/


void Config::loadGame(const QDomElement& elem)
{
	QDomElement child;
	child = elem.firstChildElement("game");
	if (child.isNull())
		return;

	READ_ATTR_FLOAT(child, "gravity", "gravityFactor");

	QString tmp;
	READ_STRING(child, "lastMap", tmp);
	set("lastMap", tmp);

//	READ_STRING(child, "mapColor", tmp);
//	QColor color = tmp;
//	if (color.isValid())
//		m_mapColor = color;

	READ_STRING(child, "useSVG", tmp);
	if (tmp.length() > 0)
		set("useSVG", tmp);
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

	QRect g = getRect("windowGeometry");
	READ_ATTR_INT_FN(child, "posX", g.setLeft);
	READ_ATTR_INT_FN(child, "posY", g.setTop);
	READ_ATTR_INT_FN(child, "width", g.setWidth);
	READ_ATTR_INT_FN(child, "height", g.setHeight);
	set("windowGeometry", g);

	READ_ATTR_BOOL(child, "fullscreen", "windowFullScreen");
	READ_ATTR_BOOL(child, "maximized", "windowMaximized");

	{
		QDomElement tmp = child.firstChildElement("stats");
		if (!tmp.isNull()) {
			READ_ATTR_BOOL(tmp, "show", "windowShowStats");

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
