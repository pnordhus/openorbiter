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


#include "map.h"
#include "node.h"
#include "openorbiter.h"
#include "graphicsscene.h"
#include "spawnpoint.h"


Map::~Map()
{
	foreach (Node* node, m_nodes)
		delete node;

	foreach (SpawnPoint* spawn, m_spawnPoints)
		delete spawn;
}


void Map::start()
{
	qDebug() << "Map::start()";

	GraphicsScene* scene = g_openorbiter->graphicsScene();
	scene->setSize(m_width, m_height);

	foreach (Node* node, m_nodes)
		node->connectScene(scene);
}


void Map::stop()
{
	qDebug() << "Map::stop()";

	foreach (Node* node, m_nodes)
		node->disconnectScene();
}


/* statics to load map from file */


Map* Map::load(const QString& filename)
{
	QDomDocument doc;

	{
	    QFile file(filename);
    	if (!doc.setContent(&file)) {
			qWarning() << "Invalid XML file:" << filename;
    	    return NULL;
		}
    }

	QDomElement root = doc.documentElement();

	Map* map = parse(root);
	if (!map) {
		qWarning() << "Invalid map:" << filename;
		return NULL;
	}

	map->m_filename = filename;
	//qDebug().nospace() << "Map " << map->name() << " (" << filename << ") loaded";

	return map;
}


#define ABORT_CHECK(val)	if (!(val)) { delete map; return NULL; }


Map* Map::parse(QDomElement& root)
{
	if (root.tagName() != "oomap")
		return NULL;

	Map* map = new Map;
	bool b;

	map->m_name  = root.attribute("name");
	ABORT_CHECK(map->m_name.length() > 0);

	map->m_width = root.attribute("width").toFloat(&b);
	ABORT_CHECK(b && (map->m_width >= 0.0f) && (map->m_width <= 4000.0f));

	map->m_height = root.attribute("height").toFloat(&b);
	ABORT_CHECK(b && (map->m_height >= 0.0f) && (map->m_height <= 4000.0f));


	map->m_gravity.x = root.attribute("gravityX", "0.0").toFloat(&b);
	if (!b)
		map->m_gravity.x = 0.0f;
	map->m_gravity.y = root.attribute("gravityY", "10.0").toFloat(&b);
	if (!b)
		map->m_gravity.y = 10.0f;

	map->m_author = root.firstChildElement("author").text();
	ABORT_CHECK(map->m_author.length() > 0);

	map->m_description = root.firstChildElement("description").text();
	ABORT_CHECK(map->m_description.length() > 0);

	QDomElement child;

	child = root.firstChildElement("spawn");
	while (!child.isNull()) {
		float x,y;
		x = child.attribute("x").toFloat(&b);
		ABORT_CHECK(b);
		y = child.attribute("y").toFloat(&b);
		ABORT_CHECK(b);

		map->m_spawnPoints.append(new SpawnPoint(x, y));
		child = child.nextSiblingElement("spawn");
	}

	child = root.firstChildElement("node");
	while (!child.isNull()) {
		float x,y;
		x = child.attribute("x").toFloat(&b);
		ABORT_CHECK(b);
		y = child.attribute("y").toFloat(&b);
		ABORT_CHECK(b);

		map->m_nodes.append(new Node(x, y));
		child = child.nextSiblingElement("node");
	}

	return map;
}


bool Map::operator == (const Map& m) const
{
	return m_name.toLower() == m.m_name.toLower();
}
