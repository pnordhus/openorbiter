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


#include "../build/configure.h"
#include "config.h"
#include "openorbiter.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QVariant>


Config::Config()
{
	m_dataDir = QDir(DATADIR).path() + QDir::separator();
	m_userDir = QDir::homePath() + QDir::separator();
#ifdef WIN32
	m_userDir += "OpenOrbiter\\";
#else
	m_userDir += ".openorbiter/";
#endif

	m_gravityFactor = 1.0f;

	m_windowPosX = 0;
	m_windowPosY = 0;
	m_windowWidth = 0;
	m_windowHeight = 0;
	m_windowMaximized = false;
	m_windowFullScreen = false;
	m_windowShowStats = true;

	m_firstNodeTime = 60.0f;
	m_nextNodeTime = 10.0f;

	m_mapColor = QColor(200,200,200);

	m_statsVisibility.clear();
	m_statsVisibility.append(false);
	m_statsVisibility.append(true);
	m_statsVisibility.append(true);
	m_statsVisibility.append(false);
	m_statsVisibility.append(false);
	m_statsVisibility.append(false);
	m_statsVisibility.append(true);
}


void Config::windowPosX(int x)
{
	m_windowPosX = x;
}


void Config::windowPosY(int y)
{
	m_windowPosY = y;
}


void Config::windowWidth(int w)
{
	m_windowWidth = w;
}


void Config::windowHeight(int h)
{
	m_windowHeight = h;
}


void Config::windowMaximized(bool m)
{
	m_windowMaximized = m;
}


void Config::windowFullScreen(bool f)
{
	m_windowFullScreen = f;
}


void Config::windowShowStats(bool s)
{
	m_windowShowStats = s;
}


void Config::lastMap(const QString& map)
{
	m_lastMap = map;
}


void Config::save(const QString& filename)
{
	QDomDocument doc;

	QDomElement root = doc.createElement("openorbiter");
	doc.appendChild(root);

	{
		QDomElement config = doc.createElement("config");
		root.appendChild(config);

		{
			QDomElement window = doc.createElement("window");
			config.appendChild(window);

			window.setAttribute("posX", m_windowPosX);
			window.setAttribute("posY", m_windowPosY);
			window.setAttribute("width", m_windowWidth);
			window.setAttribute("height", m_windowHeight);
			window.setAttribute("maximized", QVariant(m_windowMaximized).toString());
			window.setAttribute("fullscreen", QVariant(m_windowFullScreen).toString());
			window.setAttribute("stats", QVariant(m_windowShowStats).toString());

			QString statsVisibility;
			foreach (bool b, m_statsVisibility) {
				if (b)
					statsVisibility += '1';
				else
					statsVisibility += '0';
			}
			window.setAttribute("statsVisibility", statsVisibility);
		}

		{	
			QDomElement game = doc.createElement("game");
			config.appendChild(game);
	
			game.setAttribute("gravity", m_gravityFactor);
			game.setAttribute("lastMap", m_lastMap);
		}
	}

	{
		QDomElement players = doc.createElement("players");
		root.appendChild(players);

		for (int i = 0; i < OpenOrbiter::MaxPlayers; i++) {
			QDomElement player = doc.createElement("player");
			players.appendChild(player);

			player.setAttribute("id", g_openorbiter->getPlayer(i).getID());
			player.setAttribute("name", g_openorbiter->getPlayer(i).getName());
			player.setAttribute("selected", QVariant(g_openorbiter->getPlayer(i).getSelected()).toString());
		}
	}

	QFile file(filename);
	file.open(QFile::WriteOnly);
	QTextStream s(&file);
	doc.save(s, 4);
}


void Config::statsVisibility(const QList<bool>& list)
{
	Q_ASSERT(list.size() == 7);
	m_statsVisibility = list;
}


void Config::mapColor(const QColor& color)
{
	m_mapColor = color;
}


void Config::load(const QString& filename)
{
	QDomDocument doc;
	{
		QFile file(filename);
		doc.setContent(&file);
	}

	
}
