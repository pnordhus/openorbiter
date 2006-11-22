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


#ifndef OPENORBITER_CONFIG_H
#define OPENORBITER_CONFIG_H


#include "types.h"


#include <QColor>
#include <QRect>
#include <QSettings>
#include <QString>


class QDomDocument;
class QDomElement;


class Config : public QObject
{
	Q_OBJECT

public:
	Config();

public:
	QVariant	get(const QString& name) const;
	bool		getBool(const QString& name) const;
	QColor		getColor(const QString& name) const;
	float		getFloat(const QString& name) const;
	QRect		getRect(const QString& name) const;
	QString		getString(const QString& name) const;

	void		set(const QString& name, const QVariant& value);

	void					setStatsShown(const StringBoolMap&);
	const StringBoolMap&	statsShown() const	{ return m_statsShown; }

public:
	void	save(const QString& filename);
	void	load(const QString& filename);

private:
	template <typename T>
	inline T	getType(const QString& name) const;

	void	saveGame(QDomDocument&, QDomElement&);
	void	savePlayers(QDomDocument&, QDomElement&);
	void	saveWindow(QDomDocument&, QDomElement&);

	void	loadGame(const QDomElement&);
	void	loadPlayers(const QDomElement&);
	void	loadWindow(const QDomElement&);

private:
	StringBoolMap	m_statsShown;

	typedef QMap<QString, QVariant> ValueMap;
	ValueMap	m_values;

signals:
	void	changed(const QString& name);

public:
	static	void	create();
	static	void	destroy();
};


extern Config* g_config;


#endif // OPENORBITER_CONFIG_H
