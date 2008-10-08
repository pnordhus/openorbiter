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


#include "bouncerdef.h"


BouncerDef::BouncerDef() :
	m_width(0.0f),
	m_boost(0.0f),
	m_boostScale(1.0f),
	m_unlink(true)
{
	
}


void BouncerDef::setPositions(const Vector& pos1, const Vector& pos2)
{
	m_position1 = pos1;
	m_position2 = pos2;
}


void BouncerDef::setWidth(float width)
{
	m_width = width;
}


void BouncerDef::setBoost(float boost)
{
	m_boost = boost;
}


void BouncerDef::setBoostScale(float scale)
{
	m_boostScale = scale;
}


void BouncerDef::setUnlink(bool unlink)
{
	m_unlink = unlink;
}


void BouncerDef::scale(float scale)
{
	m_position1 *= scale;
	m_position2 *= scale;
}
