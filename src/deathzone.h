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


#ifndef DEATHZONE_H
#define DEATHZONE_H


#include "object.h"
#include "renderer.h"


class DeathZone
{
public:
	DeathZone(Float x, Float y, Float w, Float h, Color color = Color(0.0f, 0.0f, 0.0f));

	void	setColor(Color color);

public:
	bool	isInZone(Object& obj);
	void	render();

private:
	Float	m_X;
	Float	m_Y;
	Float	m_Width;
	Float	m_Height;
	
	Color	m_Color;
	Float	m_Time;
};


#endif
