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


#include "solid.h"
#include <GL/gl.h>


Solid::Solid()
{

}


void Solid::addVertex(Float x, Float y)
{
	Math::Vector vec;
	vec.x = x;
	vec.y = y;
	vec.z = 0.0f;
	m_Vertices.push_back(vec);
}


void Solid::render()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor4f(0.3f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		for (UInt i = 0; i < m_Vertices.size(); ++i)
			glVertex3f(m_Vertices[i].x, m_Vertices[i].y, m_Vertices[i].z);
	glEnd();
}
