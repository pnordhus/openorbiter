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


#include "map.h"
#include "timer/Timer.h"
#include <GL/glu.h>
#include <cmath>


using namespace Mercury;


Map::Map(Float x, Float y, Float width, Float height) :
	m_X(x),
	m_Y(y),
	m_Width(width),
	m_Height(height),
	m_TimeFactor(1.0)
{

}


Map::~Map()
{

}


Float Map::getTimeFactor() const
{
	return m_TimeFactor;
}


void Map::addDeathZone(DeathZone dzone)
{
	m_DeathZones.push_back(dzone);
}


void Map::addNode(Node node)
{
	m_Nodes.push_back(node);
}


void Map::addStart(Float x, Float y)
{
	Math::Vector vec;
	vec.x = x;
	vec.y = y;
	m_Starts.push_back(vec);
}


void Map::addSolid(Solid solid)
{
	m_Solids.push_back(solid);
}


Math::Vector Map::getStart()
{
	assert(m_AvailableStarts.size() > 0);

	UInt rnd = (UInt) round(((Float) rand() / (Float) RAND_MAX) * ((Float) m_AvailableStarts.size() - 1));
	
	int index = m_AvailableStarts[rnd];
	m_AvailableStarts.erase(m_AvailableStarts.begin() + rnd, m_AvailableStarts.begin() + rnd + 1);
	
	return m_Starts[index];
}


Node* Map::getNearestNode(Object& obj)
{
	if (m_Nodes.size() == 0)
		return NULL;

	float mag, min;
	Node* node = NULL;
	min = 3000000000.0f;
	
	for (UInt i = 0; i < m_Nodes.size(); i++) {
		if (!m_Nodes[i].isEnabled())
			continue;

		mag = (m_Nodes[i] - obj).magnitude();
		if (mag < min) {
			node = &m_Nodes[i];
			min = mag;
		}
	}
	assert(node);
	return node;
}


bool Map::isInDeadZone(Object& obj)
{
	if ((obj.x < 0.0f) || (obj.x >= m_Width) || (obj.y < 0) || (obj.y >= m_Height))
		return true;

	for (UInt i = 0; i < m_DeathZones.size(); ++i) {
		if (m_DeathZones[i].isInZone(obj))
			return true;
	}
	return false;
}


void Map::reset()
{
	m_TimeFactor = 1.0;
	
	m_EnabledNodes.clear();
	m_AvailableStarts.clear();
	
	m_Time = 20.0f;
	for (UInt i = 0; i < m_Nodes.size(); ++i) {
		m_Nodes[i].setEnabled(true);
		m_EnabledNodes.push_back(i);
	}
	
	for (UInt i = 0; i < m_Starts.size(); ++i)
		m_AvailableStarts.push_back(i);
}


void Map::process()
{
	m_Time -= Timer::frameTime;
	if ((m_Time <= 0.0f) && (m_EnabledNodes.size() > 1)) {
		UInt rnd = (UInt) round(((Float) rand() / (Float) RAND_MAX) * ((Float) m_EnabledNodes.size() - 1));
		m_Nodes[m_EnabledNodes[rnd]].setEnabled(false);
		m_EnabledNodes.erase(m_EnabledNodes.begin() + rnd, m_EnabledNodes.begin() + rnd + 1);
		
		m_Time = 5.0f;
	}
/*
	if ((m_Time <= 0.0f) && (m_EnabledNodes.size() >= 1) && (m_Nodes.size() > 1)) {
		Int rnd_old = (UInt) round(((Float) rand() / (Float) RAND_MAX) * ((Float) m_EnabledNodes.size() - 1));
		Int rnd_new = (UInt) round(((Float) rand() / (Float) RAND_MAX) * ((Float) m_Nodes.size() - 1));
		if (rnd_new == rnd_old)
			++rnd_new;
		m_Nodes[m_EnabledNodes[rnd_old]].setEnabled(false, &m_Nodes[rnd_new]);
		m_EnabledNodes.erase(m_EnabledNodes.begin() + rnd_old, m_EnabledNodes.begin() + rnd_old + 1);
		m_EnabledNodes.push_back(rnd_new);
		m_Time = 5.0f;
		m_TimeFactor += (0.1 * Timer::frameTime);
	}
*/	
	if (m_Time < -5.0) {
		m_TimeFactor += (0.1 * Timer::frameTime);
	}
}


void Map::render()
{
	for (UInt i = 0; i < m_DeathZones.size(); ++i)
		m_DeathZones[i].render();
	
	for (UInt i = 0; i < m_Solids.size(); ++i)
		m_Solids[i].render();
	
	for (UInt i = 0; i < m_Nodes.size(); ++i)
		m_Nodes[i].render();
}


void Map::checkCollision(Orbiter& orb)
{
/*	for (int i = 0; i < m_Solids.size(); ++i) {
		orb.checkCollision(m_Solids[i]);
	}*/
}
