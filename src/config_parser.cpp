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


#include "config_parser.h"
#include "openorbiter.h"
#include "map.h"

#include <QDebug>
#include <QFile>
#include <QVariant>


bool ConfigParser::parse(const QString& filename, int type)
{
	m_type = type;
	m_filename = filename;
	if (!open())
		return false;

	QDomElement root = m_document.documentElement();
	
	if (root.tagName() != "openorbiter") {
		qWarning() << QString("'%1' is not a valid config file!").arg(filename);
		return false;
	}

	QDomElement element = root.firstChildElement();
	while (!element.isNull()) {
		parseElement(element);
		element = element.nextSiblingElement();
	}

	return true;
}


bool ConfigParser::open()
{
	QFile file(m_filename);
	if (!file.open(QFile::ReadOnly)) {
		qWarning().nospace() << "Unable to open " << m_filename << ": " << file.errorString();
		return false;
	}
	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!m_document.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
		qWarning().nospace() << "Unable to parse " << m_filename
			<< ", error at line " << errorLine
			<< ", column " << errorColumn
			<< ": " << errorStr;
		return false;
	}

	return true;
}


void ConfigParser::parseElement(QDomElement element)
{
	if (element.tagName() == "config")
		parseConfig(element);
	else if (element.tagName() == "map")
		parseMap(element);
	else if (element.tagName() == "players")
		parsePlayers(element);
	else
		qWarning().nospace() << "Unknown tag " << element.tagName() << " found, ignoring...";
}


void ConfigParser::parsePlayers(QDomElement element)
{
	if ((m_type & PLAYERS) != PLAYERS) {
		qDebug() << "Ignoring players in" << m_filename;
		return;
	}

	QDomElement child = element.firstChildElement("player");
	while (!child.isNull()) {
		int id = convertAttribute(child.attribute("id"), -1);

		QString name;
		name = child.attribute("name");

		bool selected = convertAttribute(child.attribute("selected"), false);

		if (name.length() == 0) {
			qWarning() << "Player without name found, ignoring...";
		} else if ((id < 0) || (id >= OpenOrbiter::MaxPlayers)) {
			qWarning() << "Player" << name << "has invalid id, ignoring...";
		} else {
			Player& player = g_openorbiter->getPlayer(id);
			player.setName(name);
			player.setSelected(selected);
		}

		child = child.nextSiblingElement("player");
	}	
}


#define GET_CONFIG_ATTRIBUTE(attr, config)	config(convertAttribute(child.attribute(attr), config()));


void ConfigParser::parseConfig(QDomElement element)
{
	if ((m_type & CONFIG) != CONFIG) {
		qDebug() << "Ignoring config in" << m_filename;
		return;
	}

	Config& config = g_openorbiter->config();
	
	QDomElement child;
	child = element.firstChildElement("window");
	while (!child.isNull()) {
		GET_CONFIG_ATTRIBUTE("posX",		config.windowPosX);
		GET_CONFIG_ATTRIBUTE("posY",		config.windowPosY);
		GET_CONFIG_ATTRIBUTE("width",		config.windowWidth);
		GET_CONFIG_ATTRIBUTE("height", 		config.windowHeight);
		GET_CONFIG_ATTRIBUTE("maximized",	config.windowMaximized);
		GET_CONFIG_ATTRIBUTE("fullscreen",	config.windowFullScreen);
		GET_CONFIG_ATTRIBUTE("stats",		config.windowShowStats);

		QString statsVisibility = child.attribute("statsVisibility");
		if (statsVisibility.length() == 7) {
			QList<bool> list;
			for (int i = 0; i < 7; i++) {
				list.append(statsVisibility[i] != '0');
			}
			config.statsVisibility(list);
		}

		child = child.nextSiblingElement("window");
	}
	
	child = element.firstChildElement("game");
	while (!child.isNull()) {
		config.lastMap(child.attribute("lastMap"));

		child = child.nextSiblingElement("game");
	}
}


void ConfigParser::parseMap(QDomElement element)
{
	if ((m_type & MAP) != MAP) {
		qDebug() << "Ignoring map in" << m_filename;
		return;
	}

	bool b;

	QString name = element.attribute("name");
	if (name.isEmpty()) {
		qWarning() << "Map without name found, skipping map...";
		return;
	}

	float width = element.attribute("width").toFloat(&b);
	if ((width < 1.0) || !b) {
		qWarning() << "Map" << name << "has invalid width, skipping map...";
		return;
	}

	float height = element.attribute("height").toFloat(&b);
	if ((height < 1.0) || !b) {
		qWarning() << "Map" << name << "has invalid height, skipping map...";
		return;
	}

	float scale = 1.0;
	if (element.hasAttribute("scale")) {
		scale = element.attribute("scale").toFloat(&b);
		if (!b) {
			qWarning() << "Map" << name << "has invalid scale, skipping map...";
			return;
		}
	}

	float gravityX = 0.0;
	if (element.hasAttribute("gravityX")) {
		gravityX = element.attribute("gravityX").toFloat(&b);
		if (!b) {
			qWarning() << "Map" << name << "has invalid gravityX, skipping map...";
			return;
		}
	}

	float gravityY = 5.0;
	if (element.hasAttribute("gravityY")) {
		gravityY = element.attribute("gravityY").toFloat(&b);
		if (!b) {
			qWarning() << "Map" << name << "has invalid gravityY, skipping map...";
			return;
		}
	}

	Map::NodeList nodes;
	Map::SpawnPointList spawns;

	QDomElement child;

	child = element.firstChildElement("node");
	while (!child.isNull()) {
		float x, y;
		bool b, success = true;
		x = child.attribute("x").toFloat(&b);
		success &= b;
		y = child.attribute("y").toFloat(&b);
		success &= b;

		if (!success) {
			qWarning() << "Map" << name << "has invalid node, skipping map...";
			return;
		}
		nodes.push_back(Node(x, y));

		child = child.nextSiblingElement("node");
	}

	child = element.firstChildElement("spawn");
	while (!child.isNull()) {
		float x, y;
		bool b, success = true;
		x = child.attribute("x").toFloat(&b);
		success &= b;
		y = child.attribute("y").toFloat(&b);
		success &= b;

		if (!success) {
			qWarning() << "Map" << name << "has invalid spawnpoint, skipping map...";
			return;
		}
		spawns.push_back(SpawnPoint(x, y));

		child = child.nextSiblingElement("spawn");
	}

	QString description;
	child = element.firstChildElement("description");
	if (!child.isNull()) {
		description = child.text();

		child = child.nextSiblingElement("description");
		if (!child.isNull())
			qWarning() << "Map" << name << "has more than one description, ignoring...";
	}

	QString author;
	child = element.firstChildElement("author");
	if (!child.isNull()) {
		author = child.text();

		child = child.nextSiblingElement("author");
		if (!child.isNull())
			qWarning() << "Map" << name << "has more than one author, ignoring...";
	}

	Map newMap(name, author, description, width, height, scale, Vector(gravityX, gravityY), nodes, spawns);

	g_openorbiter->addMap(newMap);
}


template <typename T>
T ConfigParser::convertAttribute(const QString& attr, T def)
{
	bool b;
	T ret;

	if (typeid(T) == typeid(int))
		ret = (T) attr.toInt(&b);
	else if (typeid(T) == typeid(float))
		ret = (T) attr.toFloat(&b);
	else if (typeid(T) == typeid(bool)) {
		ret = QVariant(attr).toBool();
	} else
		Q_ASSERT(false);

	if (b)
		return ret;

	return def;
}
