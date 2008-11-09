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


#ifndef MAPDEF_H
#define MAPDEF_H


#include "bouncerdef.h"
#include "physics/vector.h"
#include <QList>
#include <QMap>
#include <QString>


class MapDef
{
public:
	enum Difficulty { DiffEasy, DiffNormal, DiffHard };

public:
	MapDef(const QString& name, float width, float height, const Vector& gravity, Difficulty diff);

public:
	const QString&		name() const { return m_name; }
	QString				nameTranslated() const;
	const QString&		author() const { return m_author; }
	float				width() const { return m_width; }
	float				height() const { return m_height; }
	float				scale() const { return m_scale; }
	QList<Vector>		nodes() const { return m_nodes; }
	QList<Vector>		spawns() const { return m_spawns; }
	QList<BouncerDef>	bouncers() const { return m_bouncers; }
	Vector				gravity() const { return m_gravity; }
	Difficulty			difficulty() const { return m_difficulty; }
	QString				difficultyString() const;
	void				validate() const;

public:
	void	setScale(float scale);
	void	setAuthor(const QString& author);
	void	setName(const QString& lang, const QString& name);
	void	addNode(const Vector& pos);
	void	addSpawn(const Vector& spawn);
	void	addBouncer(const BouncerDef& def);

private:
	QString			m_name;
	QString			m_author;
	float			m_width;
	float			m_height;
	float			m_scale;
	Vector			m_gravity;
	Difficulty		m_difficulty;
	
	QList<Vector>			m_nodes;
	QList<Vector>			m_spawns;
	QList<BouncerDef>		m_bouncers;
	QMap<QString, QString>	m_names;
};


#endif // MAPDEF_H
