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
	typedef QList<Node*>		NodeList;
	typedef QList<SpawnPoint*>	SpawnPointList;

	~Map();

	void	start();

	const QString&	name() const		{ return m_name; }
	const QString&	description() const	{ return m_description; }
	const QString&	author() const		{ return m_author; }
	const Vector&	gravity() const		{ return m_gravity; }

	float	width() const		{ return m_width; }
	float	height() const		{ return m_height; }
	float	aspectRatio() const	{ return m_width / m_height; }

	const NodeList&			nodes() const		{ return m_nodes; }
	const SpawnPointList&	spawnPoints() const	{ return m_spawnPoints; }

private:
	// constructor is private, maps can only be created by static member fn
	Map() {}
	// copy constructor deactivated
	Map(const Map&);
	Map& operator = (const Map&);

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

public:
	static Map*	load(const QString& filename);

private:
	static Map*	parse(QDomElement&);
};


inline bool operator < (const Map& m1, const Map& m2)
{
	return m1.name().toLower() < m2.name().toLower();
}


inline bool operator == (const Map& m1, const Map& m2)
{
	return m1.name().toLower() == m2.name().toLower();
}


#endif
