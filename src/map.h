/***************************************************************************
 *   Copyright (C) 2004 by Philipp Nordhus                                 *
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


#ifndef MAP_H
#define MAP_H


#include "deathzone.h"
#include "node.h"
#include <vector>
#include "solid.h"
#include "orbiter.h"


class Map
{
public:
	Map(Float x, Float y, Float width, Float height);
	~Map();

public:
	Float			getTimeFactor() const;

public:
	void			addDeathZone(DeathZone dzone);
	void			addNode(Node node);
	void			addStart(Float x, Float y);
	void			addSolid(Solid solid);
	
	Math::Vector		getStart();
	
	Node*			getNearestNode(Object& obj);
	bool			isInDeadZone(Object& obj);

	void			reset();
	void			process();
	void			render();
	
	void			checkCollision(Orbiter& orb);

private:
	std::vector <Node>	m_Nodes;
	std::vector <int>	m_EnabledNodes;
	std::vector <DeathZone>	m_DeathZones;
	std::vector <Solid>	m_Solids;
	
	std::vector <Math::Vector> m_Starts;
	std::vector <int>	m_AvailableStarts;
	
	Float			m_Time;
	Float			m_X;
	Float			m_Y;
	Float			m_Width;
	Float			m_Height;
	Float			m_TimeFactor;
};


#endif
