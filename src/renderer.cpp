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


#include <config.h>
#include "renderer.h"
#include <stdlib.h>
#include "SDL.h"
#include <GL/glu.h>
#include "confpaths.h"

GLfloat spec[]={1.0, 1.0 ,1.0 ,1.0};      //sets specular highlight of balls
GLfloat posl[]={0,0,50,1};               //position of ligth source
GLfloat amb[]={0.2f, 0.2f, 0.2f ,1.0f};   //global ambient
GLfloat amb2[]={0.3f, 0.3f, 0.3f ,1.0f};  //ambient of lightsource





Color::Color(Float r, Float g, Float b) :
	r(r),
	g(g),
	b(b)
{

}


// static variables
int	Renderer::m_Width;
int	Renderer::m_Height;
int	Renderer::m_BPP;
bool	Renderer::m_Fullscreen;
LFontRenderer Renderer::m_Fonts;



bool Renderer::init(int width, int height, int bpp, bool fullscreen)
{
	m_Width		= width;
	m_Height	= height;
	m_BPP		= bpp;
	m_Fullscreen	= fullscreen;
	
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		return false;
	
	atexit(SDL_Quit);
	
	SDL_ShowCursor(SDL_DISABLE);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	int flags = m_Fullscreen ? SDL_FULLSCREEN : 0;
	
	SDL_Surface* screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_HWPALETTE | flags);
	if (!screen)
		return false;

	SDL_WM_SetCaption("OpenOrbiter", NULL);
	
       	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
       	glClearDepth(1.0f);
        
	glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat) m_Width / (GLfloat) m_Height, 0.01f, 1000.0f);
	
/*	
	Float df = 100.0f;

 
        glClearDepth(1.0f);                                                                     // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                                                        // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                                         // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations

        glShadeModel(GL_SMOOTH);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT,GL_SHININESS,&df);
        
	glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
        glEnable(GL_LIGHT0);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

        glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
*/
        float df=100.0;

        glClearDepth(1.0f);                                                                     // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                                                        // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                                                         // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations

        glClearColor(0,0,0,0);
        glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

        glShadeModel(GL_SMOOTH);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);


        glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
        glMaterialfv(GL_FRONT,GL_SHININESS,&df);

        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0,GL_POSITION,posl);
        glLightfv(GL_LIGHT0,GL_AMBIENT,amb2);
        glEnable(GL_LIGHT0);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

        glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 4000, 3000, 0, -500.0, 500.0);

	m_Fonts.LoadFont("font1", PKGDATADIR"/verdana-36.fnt");
	
	return true;
}


void Renderer::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Renderer::endFrame()
{
	m_Fonts.Draw();
	SDL_GL_SwapBuffers();
}
