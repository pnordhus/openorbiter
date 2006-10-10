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


#ifndef OPENORBITER_MAP_H
#define OPENORBITER_MAP_H


#include "node.h"
#include "spawnpoint.h"

#include <QHash>
#include <QList>
#include <QString>


class QDomElement;


class Map
{
public:
	typedef QList<Node>			NodeList;
	typedef QList<SpawnPoint>	SpawnPointList;

	Map() { Q_ASSERT(false); }
	Map(const QString& name, const QString& author, const QString& description, float width, float height, float scale, const Vector& gravity, const NodeList& nodes, const SpawnPointList& spawns);

	const QString&	getName() const { return m_name; }
	const QString&	getDescription() const { return m_description; }
	const QString&	getAuthor() const { return m_author; }
	const Vector&	getGravity() const { return m_gravity; }
	const NodeList&	getNodes() const { return m_nodes; }
	const SpawnPointList&	getSpawnPoints() const { return m_spawnPoints; }
	float			getAspectRatio() const;
	float			getWidth() const { return m_width; }
	float			getHeight() const { return m_height; }

private:
	bool	parseNode(const QDomElement& elem);
	bool	parseSpawnPoint(const QDomElement& elem);

private:
	QString	m_name;
	QString	m_author;
	QString	m_description;
	float	m_width;
	float	m_height;
	float	m_scale;

	Vector	m_gravity;

	NodeList		m_nodes;
	SpawnPointList	m_spawnPoints;
};


inline bool operator < (const Map& m1, const Map& m2)
{
	return m1.getName().toLower() < m2.getName().toLower();
}


inline bool operator == (const Map& m1, const Map& m2)
{
	return m1.getName().toLower() == m2.getName().toLower();
}


#endif
