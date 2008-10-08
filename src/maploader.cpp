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


#include "mapdef.h"
#include "maploader.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QVariant>


MapLoader::MapLoader() :
	m_map(NULL),
	m_document(NULL)
{
	
}


MapLoader::~MapLoader()
{
	delete m_map;
	delete m_document;
}


bool MapLoader::loadMap(const QString& filename)
{
	Q_ASSERT(m_document == NULL);
	m_document = new QDomDocument;
	
	try {
		load(filename);
		parse();
	} catch (const QString& error) {
		qDebug() << qPrintable(QString("Error in map '%1': %2!").arg(filename).arg(error));
		delete m_map;
		m_map = NULL;
		return false;
	}
	
	return true;
}


void MapLoader::load(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return;
	
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if (!m_document->setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
		throw QString("%1 (line %2, column %3)").arg(errorMsg).arg(errorLine).arg(errorColumn);
}


void MapLoader::parse()
{
	QDomElement docElem = m_document->documentElement();
	
	parseRoot(docElem);
	
	QDomNode n = docElem.firstChild();
	while (!n.isNull()) {
		QDomElement e = n.toElement();
		if(!e.isNull()) {
			if (e.tagName() == "spawn")
				parseSpawn(e);
			else if (e.tagName() == "node")
				parseNode(e);
			else if (e.tagName() == "bouncer")
				parseBouncer(e);
			else
				throw QString("Invalid element ''").arg(e.tagName());
		} else {
			throw QString("Unknown error");
		}
		
		n = n.nextSibling();
	}
	
	m_map->validate();
}


template <typename T>
T MapLoader::require(QDomElement e, const QString& name, const QString& error)
{
	if (!e.hasAttribute(name))
		throw error;
	
	T val = QVariant(e.attribute(name)).value<T>();
	return val;
}


template <typename T>
T MapLoader::optional(QDomElement e, const QString& name, const T& defaultValue)
{
	if (!e.hasAttribute(name))
		return defaultValue;
	
	T val = QVariant(e.attribute(name)).value<T>();
	return val;
}


void MapLoader::parseRoot(const QDomElement& e)
{
	if (e.tagName() != "oomap")
		throw QString("Not a valid OpenOrbiter map file");
	
	const QString mapName = require<QString>(e, "name", "No name given");
	if (mapName == "")
		throw QString("Name is empty");
	
	const float mapWidth = require<float>(e, "width", "No width defined");
	if (mapWidth < 5.0f)
		throw QString("Width is too small");
	
	const float mapHeight = require<float>(e, "height", "No height defined");
	if (mapHeight < 5.0f)
		throw QString("Height is too small");
	
	Vector gravity;
	gravity.x = optional(e, "gravityX", 0.0f);
	gravity.y = optional(e, "gravityY", 10.0f);
	
	m_map = new MapDef(mapName, mapWidth, mapHeight, gravity);
}


void MapLoader::parseSpawn(const QDomElement& e)
{
	Vector pos;
	pos.x = require<float>(e, "x", "Spawn point is missing x position");
	pos.y = require<float>(e, "y", "Spawn point is missing y position");
	m_map->addSpawn(pos);
}


void MapLoader::parseNode(const QDomElement& e)
{
	Vector pos;
	pos.x = require<float>(e, "x", "Node is missing x position");
	pos.y = require<float>(e, "y", "Node is missing y position");
	m_map->addNode(pos);
}


void MapLoader::parseBouncer(const QDomElement& e)
{
	Vector pos1;
	pos1.x = require<float>(e, "x1", "Bouncer is missing x1 position");
	pos1.y = require<float>(e, "y1", "Bouncer is missing y1 position");
	
	Vector pos2;
	pos2.x = require<float>(e, "x2", "Bouncer is missing x2 position");
	pos2.y = require<float>(e, "y2", "Bouncer is missing y2 position");
	
	float width = require<float>(e, "width", "Bouncer is missing width");
	float boost = optional<float>(e, "boost", 0.0f);
	bool unlink = optional<bool>(e, "unlink", true);
	
	BouncerDef def;
	def.setPositions(pos1, pos2);
	def.setWidth(width);
	def.setBoost(boost);
	def.setUnlink(unlink);
	
	m_map->addBouncer(def);
}
