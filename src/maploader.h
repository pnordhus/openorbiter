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


#ifndef MAPLOADER_H
#define MAPLOADER_H


#include <QString>


class Map;
class QDomDocument;
class QDomElement;


class MapLoader
{
public:
	MapLoader();
	~MapLoader();

public:
	bool		loadMap(const QString& filename);
	const Map&	map() const { Q_ASSERT(m_map); return *m_map; }
	
private:
	void		load(const QString& filename);
	void		parse();
	void		parseRoot(const QDomElement& e);
	void		parseSpawn(const QDomElement& e);
	void		parseNode(const QDomElement& e);
	void		parseBouncer(const QDomElement& e);
	
	template <typename T>
	T			require(QDomElement e, const QString& name, const QString& error);
	template <typename T>
	T			optional(QDomElement e, const QString& name, const T& defaultValue);


private:
	Map*			m_map;
	QDomDocument*	m_document;
};


#endif // MAPLOADER_H
