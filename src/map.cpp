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

#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>


Map::Map(const QString& name, const QString& author, const QString& description, float width, float height, float scale, const Vector& gravity, const NodeList& nodes, const SpawnPointList& spawns) :
	m_name(name),
	m_author(author),
	m_description(description),
	m_width(width),
	m_height(height),
	m_scale(scale),
	m_gravity(gravity),
	m_nodes(nodes),
	m_spawnPoints(spawns)
{

}


bool Map::parseNode(const QDomElement& elem)
{
	float x, y;
	bool b;
	bool success = true;
	
	x = elem.attribute("x").toFloat(&b);
	success &= b;
	y = elem.attribute("y").toFloat(&b);
	success &= b;

	m_nodes.push_back(Node(x, y));

	return success;
}


bool Map::parseSpawnPoint(const QDomElement& elem)
{
	float x, y;
	bool b;
	bool success = true;
	
	x = elem.attribute("x").toFloat(&b);
	success &= b;
	y = elem.attribute("y").toFloat(&b);
	success &= b;

	m_spawnPoints.push_back(SpawnPoint(x, y));

	return success;
}


float Map::getAspectRatio() const
{
	return m_width / m_height;
}
