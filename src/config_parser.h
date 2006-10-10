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


#ifndef OPENORBITER_CONFIG_PARSER_H
#define OPENORBITER_CONFIG_PARSER_H


#include <QDomDocument>
#include <QString>


class ConfigParser
{
public:
	enum { MAP, CONFIG, PLAYERS };

	bool	parse(const QString& filename, int type);

private:
	bool	open();
	void	parseElement(QDomElement element);
	void	parseConfig(QDomElement element);
	void	parsePlayers(QDomElement element);
	void	parseMap(QDomElement element);

	template <typename T>
	static	T	convertAttribute(const QString& attr, T def);

private:
	QString			m_filename;
	QDomDocument	m_document;
	int				m_type;
};


#endif
