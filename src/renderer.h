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


#ifndef RENDERER_H
#define RENDERER_H


#include "common/Types.h"
#include "lfontrenderer.h"


class Color
{
public:
	Color(Float r, Float g, Float b);

public:
	Float r;
	Float g;
	Float b;
};


class Renderer
{
public:
	static bool	init(int width, int height, int bpp, bool fullscreen);
	
	static void	beginFrame();
	static void	endFrame();
	
public:
	static LFontRenderer	m_Fonts;

private:
	static int	m_Width;
	static int	m_Height;
	static int	m_BPP;
	static bool	m_Fullscreen;

private:
	Renderer(); // disable constructor
};


#endif
