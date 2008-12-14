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


#include "node.h"
#include "timer/Timer.h"
#include <GL/glu.h>


using namespace Mercury;


static GLuint renderList = 0;


Node::Node(Float x, Float y) :
	Collider(x, y, 1.0f, 25.0f, true, true),
	m_Enabled(true),
	m_DisableWait(false),
	m_NextNode(NULL)
{

	
}


bool Node::isEnabled() const
{
	return m_Enabled;
}


void Node::setEnabled(bool value, Node* next)
{
	m_NextNode = next;
	if (!value) {
		m_Time = 4.0f;
		m_DisableWait = true;
	} else {
		m_Enabled = true;
		m_DisableWait = false;
	}
}


void Node::render()
{
	if (!m_Enabled)
		return;
		
	m_Time -= Timer::frameTime;
	
	if (m_DisableWait && m_Time <= 0.0f) {
		m_DisableWait = false;
		m_Enabled = false;
		if (m_NextNode) {
			m_NextNode->setEnabled(true);
			m_NextNode = NULL;
		}
	}
	
	if (renderList == 0) {
		GLUquadricObj* quadric = gluNewQuadric();
        	gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		
		renderList = glGenLists(1);
        	glNewList(renderList, GL_COMPILE);
			gluSphere(quadric, 25, 25, 25);
		glEndList();
		
		gluDeleteQuadric(quadric);
	}

       	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	
	if (m_DisableWait && ((int) (m_Time * 2.0f) & 1))
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	else
		glColor4f(0.0f, 0.3f, 0.6f, 1.0f);

	glEnable(GL_LIGHTING);
	glCallList(renderList);
	glDisable(GL_LIGHTING);
}
