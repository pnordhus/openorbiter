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


#ifndef RECT_H
#define RECT_H


#include "object.h"


class Rect : public Object
{
public:
	Rect();

public:
	void	setPositions(const Vector& pos1, const Vector& pos2);
	void	setWidth(float width);
	void	setBoost(float boost);
	void	setUnlink(bool unlink);
	float	width() const { return m_width; }
	float	boost() const { return m_boost; }
	bool	unlink() const { return m_unlink; }
	const Vector&	position1() const { return m_position1; }
	const Vector&	position2() const { return m_position2; }
	const Vector&	dir() const { return m_dir; }

private:
	Vector	m_position1;
	Vector	m_position2;
	Vector	m_dir;
	float	m_width;
	float	m_boost;
	bool	m_unlink;
};


#endif // RECT_H
