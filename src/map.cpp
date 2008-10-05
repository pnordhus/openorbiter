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


#include "map.h"


Map::Map(const QString& name, float width, float height) :
	m_name(name),
	m_width(width),
	m_height(height)
{
	
}


void Map::setAuthor(const QString& name)
{
	m_author = name;
}


void Map::setDescription(const QString& desc)
{
	m_description = desc;
}


void Map::addNode(const Vector& pos)
{
	m_nodes.append(pos);
}


void Map::addSpawn(const Vector& spawn)
{
	m_spawns.append(spawn);
}
