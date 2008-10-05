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


#include <QXmlDefaultHandler>


class Map;


class MapLoader : private QXmlDefaultHandler
{
public:
	MapLoader();
	~MapLoader();

public:
	bool	loadMap(const QString& filename);
	Map*	takeMap();

private:
	bool	startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
	bool	endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
	bool	characters(const QString& text);

private:
	Map*	m_map;
	QString	m_text;
};


#endif // MAPLOADER_H