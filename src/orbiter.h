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


#ifndef ORBITER_H
#define ORBITER_H


#include "collider.h"
#include "node.h"
#include "solid.h"


class Orbiter : public Collider
{
public:
	Orbiter(Float x, Float y);

public:
	void		toggleConnect();

	virtual void	reset(Float x, Float y);
	virtual void	process();
	virtual void	render();
	virtual void	collide(Collider& collider);

private:	
	bool		m_Connected;
	bool		m_TryConnect;
	Node*		m_Node;
	float		m_OrbitRadius;
	float		m_Mag;
	float		m_Time;
};	


#endif
