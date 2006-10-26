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
#include <QList>
#include <QRect>
#include <QString>


class QDomDocument;
class QDomElement;


class Config
{
public:
	Config();

	const QString&	dataDir() const	{ return m_dataDir; }
	const QString&	userDir() const	{ return m_userDir; }

	const QRect&	windowGeometry() const		{ return m_windowGeometry; }
	bool			windowMaximized() const 	{ return m_windowMaximized; }
	bool			windowFullScreen() const	{ return m_windowFullScreen; }
	bool			windowShowStats() const		{ return m_windowShowStats; }

	float					gravityFactor() const	{ return m_gravityFactor; }
	const StringBoolMap&	statsShown() const	{ return m_statsShown; }

	void	setWindowGeometry(const QRect& g);
	void	setWindowMaximized(bool);
	void	setWindowFullScreen(bool);
	void	setWindowShowStats(bool);

	void	setGravityFactor(float);
	void	setStatsShown(const StringBoolMap&);


	float	firstNodeTime() const { return m_firstNodeTime; }
	float	nextNodeTime() const { return m_nextNodeTime; }

	const QString&	lastMap() const { return m_lastMap; }
	void			setLastMap(const QString&);

	const QColor&	mapColor() { return m_mapColor; }
	void			setMapColor(const QColor&);

	void	save(const QString& filename);
	void	load(const QString& filename);

private:
	void	saveGame(QDomDocument&, QDomElement&);
	void	savePlayers(QDomDocument&, QDomElement&);
	void	saveWindow(QDomDocument&, QDomElement&);

	void	loadGame(const QDomElement&);
	void	loadPlayers(const QDomElement&);
	void	loadWindow(const QDomElement&);

private:
	QString	m_dataDir;
	QString	m_userDir;
	float	m_gravityFactor;

	QRect	m_windowGeometry;
	bool	m_windowMaximized;
	bool	m_windowFullScreen;
	bool	m_windowShowStats;

	StringBoolMap	m_statsShown;

	float	m_firstNodeTime;
	float	m_nextNodeTime;
	QColor	m_mapColor;
	
	QString	m_lastMap;
};


extern Config g_config;


#endif
