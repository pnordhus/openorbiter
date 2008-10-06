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


#include "rect.h"


Rect::Rect() :
	Object(Object::Rect),
	m_width(0.0f),
	m_boost(0.0f)
{
	setStatic(true);
}


void Rect::setPositions(const Vector& pos1, const Vector& pos2)
{
	m_position1 = pos1;
	m_position2 = pos2;
	m_dir = (m_position2 - m_position1).normalized();
	Object::setPosition((pos1 + pos2) * 0.5f);
	setWidth(m_width);
}


void Rect::setWidth(float width)
{
	m_width = width;
	
	const float a = width + (m_position1 - m_position2).length() / 2.0f;
	setRadius(std::sqrt(width * width + a * a));
}


void Rect::setBoost(float boost)
{
	m_boost = boost;
}
