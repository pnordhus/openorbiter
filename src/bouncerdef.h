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


#ifndef BOUNCERDEF_H
#define BOUNCERDEF_H


#include "physics/vector.h"


class BouncerDef
{
public:
	BouncerDef();

public:
	void	setPositions(const Vector& pos1, const Vector& pos2);
	void	setWidth(float width);
	void	setBoost(float boost);

public:
	const Vector&	position1() const { return m_position1; }
	const Vector&	position2() const { return m_position2; }
	float			width() const { return m_width; }
	float			boost() const { return m_boost; }

private:
	Vector	m_position1;
	Vector	m_position2;
	float	m_width;
	float	m_boost;
};


#endif // BOUNCERDEF_H
