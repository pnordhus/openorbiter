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


#include "input.h"
#include "match.h"
#include <GL/glu.h>
#include "renderer.h"
#include <sstream>
#include "Font.h"



// static variables
Map			Match::m_Map(0.0f, 0.0f, 0.0f, 0.0f);
std::vector<Player>	Match::m_Players;
std::vector<Player*>	Match::m_Active;
Color			Match::m_Color(1.0f, 0.0f, 0.0f);



Color Match::getColor()
{
	return m_Color;
}


Map& Match::getMap()
{
	return m_Map;
}


void Match::setMap(Map& map)
{
	m_Map = map;
}


void Match::addPlayer(Player player)
{
	m_Players.push_back(player);
}


void Match::reset()
{
	m_Map.reset();
	m_Active.clear();
	
	Input::unbindAll();
	
	Math::Vector vec;
	
	for (UInt i = 0; i < m_Players.size(); ++i) {
		vec = m_Map.getStart();
		m_Players[i].reset(vec.x, vec.y);
		m_Active.push_back(&m_Players[i]);
		assert(Input::bindKey(m_Players[i].getKey(), m_Players[i]));
	}
}


void Match::process()
{
	m_Map.process();

	for (UInt i = 0; i < m_Active.size(); ++i)
		m_Active[i]->process();
		
	for (UInt i = 0; i < m_Active.size(); ++i) {
		for (UInt j = i + 1; j < m_Active.size(); ++j) {
			m_Active[i]->checkCollision(*m_Active[j]);
			m_Map.checkCollision(*m_Active[i]);
		}
	}
	
	for (int i = m_Active.size() - 1; i >= 0; --i) {
		if (m_Map.isInDeadZone(*m_Active[i])) {
			if (m_Active[i]->getLastCollider()) {
				Player* player = dynamic_cast<Player*>(m_Active[i]->getLastCollider());
				if (player)
					player->frag();
			}
			Input::unbindKey(m_Active[i]->getKey());
			m_Active.erase(m_Active.begin() + i, m_Active.begin() + i + 1);
			
			if (m_Active.size() == 1) {
				m_Color = m_Active[0]->getColor();
				m_Active[0]->win();
				reset();
				break;
			}
		}
	}
}


void Match::render()
{
	m_Map.render();
	for (UInt i = 0; i < m_Active.size(); ++i)
		m_Active[i]->render();
	
	glLoadIdentity();
	glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
		
		glVertex3f(150.0f, 2900.0f, 10.0f);
		glVertex3f(150.0f, 2970.0f, 10.0f);
		glVertex3f(3850.0f, 2970.0f, 10.0f);
		glVertex3f(3850.0f, 2900.0f, 10.0f);
	glEnd();
	
	for (UInt i = 0; i < m_Players.size(); ++i) {
		std::stringstream stream;
		stream << "Frags: " << m_Players[i].getFrags() << ", ";
		stream << "Total frags: " << m_Players[i].getTotalFrags() << ", ";
		stream << "Wins: " << m_Players[i].getWins();
		Color color = m_Players[i].getColor();
		Renderer::m_Fonts.SetColor(color.r, color.g, color.b);
		//Renderer::m_Fonts.StringOut(300 * (i + 1), 2900, stream.str());
		Renderer::m_Fonts.StringOut(300, 2700 + 50 * i, stream.str());
		/*
		std::wstring str;
		str.resize(stream.str().size());
		for (int i = 0; i < stream.str().size(); i++)
			str[i] = static_cast<unsigned char>(stream.str()[i]);
		
        	glPushAttrib(GL_ALL_ATTRIB_BITS);

        	glViewport(0, 4000, 3000, 0);

        	glMatrixMode(GL_PROJECTION);
        	glPushMatrix();

        	glLoadIdentity();
        	glOrtho(0, 4000, 3000, 0, 1, 10);

        	glMatrixMode(GL_MODELVIEW);
        	glPushMatrix();
	        glLoadIdentity();

	        glTranslatef(0.0, 0.0, -1.0);
	
		Font font("data/vera.ttf", 12);
		font.print(300, 2700 + 50 * i, 4000, 30, L"Hallo", PF_DEFAULT);
		
		glMatrixMode(GL_PROJECTION);
	        glPopMatrix();
        	glMatrixMode(GL_MODELVIEW);
        	glPopMatrix();
        	glPopAttrib();*/
	}
}
