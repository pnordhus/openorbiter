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


#ifndef GAME_H
#define GAME_H


#include "map.h"
#include <QObject>


class Orbiter;
class Player;
class Vector;


class Game : public QObject, public Map
{
	Q_OBJECT

public:
	Game(Scene& scene, const MapDef& map, const QList<Player*>& players);
	~Game();

public:
	Node*			nearestNode(const Vector& pos);
	void			keyPressed(int key);
	const QList<Orbiter*>&	orbiters() const { return m_orbiters; }

public slots:
	const Player*	process(float time);

private:
	QList<Orbiter*>	m_orbiters;
};


#endif // GAME_H
