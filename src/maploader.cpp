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
#include "maploader.h"
#include <QVariant>
#include <QXmlInputSource>


MapLoader::MapLoader() :
	m_map(NULL)
{
	
}


MapLoader::~MapLoader()
{
	delete m_map;
}


bool MapLoader::loadMap(const QString& filename)
{
	delete m_map;
	m_map = NULL;
	
	QFile file(filename);
	QXmlInputSource source(&file);
	QXmlSimpleReader xmlReader;
	xmlReader.setContentHandler(this);
	if (!xmlReader.parse(source))
		return false;
	
	return m_map;
}


Map* MapLoader::takeMap()
{
	Q_ASSERT(m_map);
	Map* map = m_map;
	m_map = NULL;
	return map;
}


bool MapLoader::startElement(const QString&, const QString&, const QString& qName, const QXmlAttributes& atts)
{
	if (qName == "oomap") {
		const QString name = atts.value("name");
		const float width  = QVariant(atts.value("width")).toDouble();
		const float height = QVariant(atts.value("height")).toDouble();
		if (name.isEmpty())
			return false;
		
		if (width < 5.0f)
			return false;
		
		if (height < 5.0f)
			return false;
		
		delete m_map;
		m_map = new Map(name, width, height);
	} else if (qName == "spawn") {
		const float x = QVariant(atts.value("x")).toDouble();
		const float y = QVariant(atts.value("y")).toDouble();
		m_map->addSpawn(Vector(x, y));
	} else if (qName == "node") {
		const float x = QVariant(atts.value("x")).toDouble();
		const float y = QVariant(atts.value("y")).toDouble();
		m_map->addNode(Vector(x, y));
	}
	
	m_text.clear();
	
	return true;
}


bool MapLoader::endElement(const QString&, const QString&, const QString& qName)
{
	if (!m_map)
		return true;
	
	if (qName == "author")
		m_map->setAuthor(m_text);
	else if (qName == "description")
		m_map->setDescription(m_text); 
	
	return true;
}


bool MapLoader::characters(const QString& text)
{
	m_text += text;
	return true;
}
 