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


#include <QColor>
#include <QList>
#include <QString>


class Config
{
public:
	Config();

	const QString&	getDataDir() const { return m_dataDir; }
	const QString&	getUserDir() const { return m_userDir; }

	float			getGravityFactor() const { return m_gravityFactor; }

	void	windowPosX(int x);
	void	windowPosY(int y);
	void	windowWidth(int w);
	void	windowHeight(int h);
	void	windowMaximized(bool m);
	void	windowFullScreen(bool f);
	void	windowShowStats(bool s);

	int		windowPosX() const { return m_windowPosX; }
	int		windowPosY() const { return m_windowPosY; }
	int		windowWidth() const { return m_windowWidth; }
	int		windowHeight() const { return m_windowHeight; }
	bool	windowMaximized() const { return m_windowMaximized; }
	bool	windowFullScreen() const { return m_windowFullScreen; }
	bool	windowShowStats() const { return m_windowShowStats; }

	void	statsVisibility(const QList<bool>& list);
	const QList<bool>& statsVisibility() const { return m_statsVisibility; }

	float	firstNodeTime() const { return m_firstNodeTime; }
	float	nextNodeTime() const { return m_nextNodeTime; }

	const QString&	lastMap() const { return m_lastMap; }
	void			lastMap(const QString&);

	const QColor&	mapColor() { return m_mapColor; }
	void			mapColor(const QColor&);

	void	save(const QString& filename);
	void	load(const QString& filename);

private:
	QString	m_dataDir;
	QString	m_userDir;
	float	m_gravityFactor;

	int		m_windowPosX;
	int		m_windowPosY;
	int		m_windowWidth;
	int		m_windowHeight;
	bool	m_windowMaximized;
	bool	m_windowFullScreen;
	bool	m_windowShowStats;

	QList<bool>	m_statsVisibility;

	float	m_firstNodeTime;
	float	m_nextNodeTime;
	QColor	m_mapColor;
	
	QString	m_lastMap;
};


#endif
