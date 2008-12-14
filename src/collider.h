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


#ifndef COLLIDER_H
#define COLLIDER_H


#include "object.h"


class Collider : public Object
{
public:
	Collider(Float x, Float y, Float mass, Float radius, bool is_sphere, bool is_fixed);
	
	int		getContacts() const;

public:
	bool		checkCollision(Collider& collider);
	virtual void	collide(Collider& collider);
	virtual void	reset(Float x, Float y);
	
	Collider*	getLastCollider() const;
	
private:
	static bool	checkSphereOnSphere(Collider& sphere1, Collider& sphere2);
	static bool	checkSphereOnFixedSphere(Collider& sphere1, Collider& sphere2);
	
private:
	Collider*	m_LastCollider;
	int		m_Contacts;
	Float		m_OrgRadius;
	Float		m_OrgMass;
};


#endif
