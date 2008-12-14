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


#include "vector.h"
#include <cmath>


using namespace Math;


Vector& Vector::operator += (const Vector& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}


Vector& Vector::operator -= (const Vector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}


Vector& Vector::operator *= (const Vector& vec)
{
	Vector tmp = *this;
	x = tmp.y * vec.z - tmp.z * vec.y;
	y = tmp.z * vec.x - tmp.x * vec.z;
	z = tmp.x * vec.y - tmp.y * vec.x;
	return *this;
}


Vector& Vector::operator *= (Float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}


Vector& Vector::operator /= (Float value)
{
	assert(value != (Float) 0.0);

	x /= value;
	y /= value;
	z /= value;
	return *this;
}


Vector& Vector::operator %= (Float length)
{
	assert(magnitude() != (Float) 0.0);
	
	return (*this *= (length / magnitude()));
}


Vector Vector::operator + (const Vector& vec) const
{
	Vector newvec;
	newvec.x = x + vec.x;
	newvec.y = y + vec.y;
	newvec.z = z + vec.z;
	return newvec;
}


Vector Vector::operator - (const Vector& vec) const
{
	Vector newvec;
	newvec.x = x - vec.x;
	newvec.y = y - vec.y;
	newvec.z = z - vec.z;
	return newvec;
}


Vector Vector::operator * (const Vector& vec) const
{
	Vector newvec;
	newvec.x = x * vec.x;
	newvec.y = y * vec.y;
	newvec.z = z * vec.z;
	return newvec;
}


Vector Vector::operator * (Float value) const
{
	Vector newvec;
	newvec.x = x * value;
	newvec.y = y * value;
	newvec.z = z * value;
	return newvec;
}


Vector Vector::operator / (Float value) const
{
	assert(value != (Float) 0.0);
	
	Vector newvec;
	newvec.x = x / value;
	newvec.y = y / value;
	newvec.z = z / value;
	return newvec;
}


Vector Vector::operator %  (Float length) const
{
	assert(magnitude() != (Float) 0.0);
	
	return (*this * (length / magnitude()));
}


Float Vector::dot(const Vector& vec) const
{
	return (Float) x*vec.x + y*vec.y + z*vec.z;
}


Float Vector::magnitude() const
{
	return (Float) sqrt(x*x + y*y + z*z);
}


void Vector::zero()
{
	memset(this, 0, sizeof(Vector));
}


std::ostream& std::operator << (std::ostream& out, const Vector& vec)
{
	out << vec.x << " " << vec.y << " " << vec.z;
	return out;
}
