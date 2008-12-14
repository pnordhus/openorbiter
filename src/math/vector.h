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


#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H


#include "common/Types.h"


namespace Math
{

	class Vector
	{
	public:
		Float		x;
		Float		y;
		Float		z;
	
	public:
		Vector&		operator += (const Vector&);
		Vector&		operator -= (const Vector&);
		Vector&		operator *= (const Vector&);
		Vector&		operator *= (Float);
		Vector&		operator /= (Float);
		Vector&		operator %= (Float);
		
		Vector		operator +  (const Vector&) const;
		Vector		operator -  (const Vector&) const;
		Vector		operator *  (const Vector&) const;
		Vector		operator *  (Float) const;
		Vector		operator /  (Float) const;
		Vector		operator %  (Float) const;
		
	public:
		Float		dot(const Vector&) const;
		Float		magnitude() const;
		void		zero();
		
	} NO_ALIGN;

}


namespace std
{
	ostream& operator << (ostream& out, const Math::Vector& vec);
}


#endif
