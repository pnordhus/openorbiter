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


#include "deathzone.h"
#include "match.h"
#include "timer/Timer.h"
#include <GL/glu.h>


DeathZone::DeathZone(Float x, Float y, Float w, Float h, Color color) :
	m_X(x),
	m_Y(y),
	m_Width(w),
	m_Height(h),
	m_Color(color),
	m_Time(0.0f)
{

}


void DeathZone::setColor(Color color)
{
	m_Color = color;
}


bool DeathZone::isInZone(Object& obj)
{
	return ((obj.x >= m_X) && (obj.x < m_X + m_Width) && (obj.y >= m_Y) && (obj.y < m_Y + m_Height));
}


void DeathZone::render()
{
	Color color = m_Color;
	if ((m_Color.r == 0.0f) && (m_Color.g == 0.0f) && (m_Color.b == 0.0f))
		color = Match::getColor();
	else
		color = m_Color;
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor4f(color.r, color.g, color.b, 1.0f);
		
		glVertex2d(m_X, m_Y);
		glVertex2d(m_X, m_Y + m_Height);
		glVertex2d(m_X + m_Width, m_Y + m_Height);
		glVertex2d(m_X + m_Width, m_Y);
	glEnd();
}
