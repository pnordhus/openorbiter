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


#include "collider.h"


Collider::Collider(Float x, Float y, Float mass, Float radius, bool is_sphere, bool is_fixed) :
	Object(x, y, mass, radius, is_sphere, is_fixed),
	m_LastCollider(NULL),
	m_Contacts(0),
	m_OrgRadius(radius),
	m_OrgMass(mass)
{

}


int Collider::getContacts() const
{
	return m_Contacts;
}


bool Collider::checkCollision(Collider& collider)
{
	if (m_IsFixed && collider.m_IsFixed)
		return false;

	if (m_IsSphere && collider.m_IsSphere) {
		if (m_IsFixed)
			return checkSphereOnFixedSphere(collider, *this);
		if (collider.m_IsFixed)
			return checkSphereOnFixedSphere(*this, collider);
		return checkSphereOnSphere(*this, collider);
	}
	
	return false;
}


bool Collider::checkSphereOnSphere(Collider& sphere1, Collider& sphere2)
{
	Vector dist12 = sphere2 - sphere1;
	Vector dist21 = sphere1 - sphere2;
	
	Float mag = dist12.magnitude();
	if (mag > (sphere1.m_Radius + sphere2.m_Radius))
		return false;
	
	if (mag == 0.0f) {
		dist12.x = 0.00001f;
		dist21.x = 0.00001f;
	}
	
	dist12 %= 1.0f;
	dist21 %= 1.0f;
	
	Float mass12 = sphere1.m_Mass / sphere2.m_Mass;
	
	Float rat12 = sphere1.m_Speed.dot(dist12);
	Float rat21 = sphere2.m_Speed.dot(dist21);

	sphere1.m_Speed += (dist21 * rat21) - (dist12 * rat12);
	sphere2.m_Speed += (dist12 * rat12) - (dist21 * rat21);
	
	sphere1 += dist21 % (((sphere1.m_Radius * 1.0) + (sphere2.m_Radius * 1.0) - mag) / 2.0 + 0.00001);
	sphere2 += dist12 % (((sphere1.m_Radius * 1.0) + (sphere2.m_Radius * 1.0) - mag) / 2.0 + 0.00001);
	
	sphere1.collide(sphere2);
	sphere2.collide(sphere1);
	
	return true;
}


bool Collider::checkSphereOnFixedSphere(Collider& sphere1, Collider& sphere2)
{
	Vector dist12 = sphere2 - sphere1;
	
	Float mag = dist12.magnitude();
	if (mag > (sphere1.m_Radius + sphere2.m_Radius))
		return false;
	
	if (mag == 0.0f) {
		dist12.x = 0.00001f;
	}
	
	dist12 %= 1.0f;
	
	Float rat12 = sphere1.m_Speed.dot(dist12);

	sphere1.m_Speed -= (dist12 * rat12);
	sphere1.collide(sphere2);
	
	return true;
}


void Collider::collide(Collider& collider)
{
//	m_Radius *= 1.1;
//	if (m_Radius > 240.0)
//		m_Radius = 240.0;
		
//	m_Mass *= 1.1;

	++m_Contacts;
	m_LastCollider = &collider;
}


void Collider::reset(Float x, Float y)
{
	m_Contacts = 0;
	m_LastCollider = NULL;
	m_Radius = m_OrgRadius;
	m_Mass = m_OrgMass;
}


Collider* Collider::getLastCollider() const
{
	return m_LastCollider;
}
