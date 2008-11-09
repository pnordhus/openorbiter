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
#include <QSettings>


MapDef::MapDef(const QString& name, float width, float height, const Vector& gravity, Difficulty diff) :
	m_name(name),
	m_width(width),
	m_height(height),
	m_scale(1.0f),
	m_gravity(gravity),
	m_difficulty(diff)
{
	
}


QString MapDef::nameTranslated() const
{
	const QString lang = QSettings().value("language").toString();
	if (m_names.contains(lang))
		return m_names.value(lang);
	
	return m_name;
}


void MapDef::setName(const QString& lang, const QString& name)
{
	m_names[lang] = name;
}


void MapDef::setAuthor(const QString& author)
{
	m_author = author;
}


void MapDef::addNode(const Vector& pos)
{
	m_nodes.append(pos);
}


void MapDef::addSpawn(const Vector& spawn)
{
	m_spawns.append(spawn);
}


void MapDef::addBouncer(const BouncerDef& def)
{
	m_bouncers.append(def);
}


void MapDef::setScale(float scale)
{
	m_scale = scale;
}


void MapDef::validate() const
{
	if (m_nodes.empty())
		throw QString("Map has no nodes");
	
	if (m_spawns.size() < 8)
		throw QString("Map has too few spawn points");
}


QString MapDef::difficultyString() const
{
	switch (m_difficulty) {
		case DiffEasy:		return QObject::tr("Easy");
		case DiffNormal:	return QObject::tr("Normal");
		case DiffHard:		return QObject::tr("Hard");
	}
	
	Q_ASSERT(false);
	return "";
}
