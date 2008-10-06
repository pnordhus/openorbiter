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


#ifndef MAP_H
#define MAP_H


#include "bouncerdef.h"
#include "physics/vector.h"
#include <QList>
#include <QString>



class Map
{
public:
	Map(const QString& name, float width, float height);

public:
	const QString&	name() const { return m_name; }
	float			width() const { return m_width; }
	float			height() const { return m_height; }
	const QList<Vector>& nodes() const { return m_nodes; }
	const QList<Vector>& spawns() const { return m_spawns; }
	const QList<BouncerDef>& bouncers() const { return m_bouncers; }
	const Vector&	gravity() const { return m_gravity; }
	void	setGravity(const Vector& gravity);
	
	void	setAuthor(const QString& name);
	void	setDescription(const QString& desc);

public:
	void	addNode(const Vector& pos);
	void	addSpawn(const Vector& spawn);
	void	addBouncer(const BouncerDef& def);

private:
	QString			m_name;
	QString			m_author;
	QString			m_description;
	float			m_width;
	float			m_height;
	Vector			m_gravity;
	
	QList<Vector>		m_nodes;
	QList<Vector>		m_spawns;
	QList<BouncerDef>	m_bouncers;
};


#endif // MAP_H
