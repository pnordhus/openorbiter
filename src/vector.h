/***************************************************************************
 *   Copyright (C) 2006 by Philipp Nordhus                                 *
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


#ifndef OPENORBITER_VECTOR_H
#define OPENORBITER_VECTOR_H


#include <QDebug>
#include <QRect>
#include <cmath>


class Vector
{
public:
	Vector(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	Vector&	operator += (const Vector& v) { x += v.x; y += v.y; return *this; }
	Vector	operator * (float f) const { return Vector(x * f, y * f); }
	Vector	operator - () const { return Vector(-x, -y); }
	Vector	operator - (const Vector& v) const { return Vector(x - v.x, y - v.y); }
	Vector	operator + (const Vector& v) const { return Vector(x + v.x, y + v.y); }

	inline float	distance(const Vector&) const;
	inline float	length() const;
	inline float	lengthSquare() const;
	inline void		setLength(float l);
	inline void		normalize();
	inline Vector	normalized() const;
	inline float	dot(const Vector&) const;
	inline Vector	vertical() const;
	inline bool		isNull() const;

public:
	float	x;
	float	y;
};


QDebug& operator << (QDebug& s, const Vector& v);


inline float Vector::distance(const Vector& v) const
{
	return (*this - v).length();
}


inline float Vector::length() const
{
	return sqrtf(lengthSquare());
}


inline float Vector::lengthSquare() const
{
	return x * x + y * y;
}


inline void Vector::setLength(float l)
{
	float f = length();
	Q_ASSERT(f != 0.0f);
	f = l / f;
	x *= f;
	y *= f;
}


inline void Vector::normalize()
{
	setLength(1.0f);
}


inline Vector Vector::normalized() const
{
	Vector v = *this;
	v.normalize();
	return v;
}


inline float Vector::dot(const Vector& v) const
{
	return (x * v.x) + (y * v.y);
}


inline Vector Vector::vertical() const
{
	return Vector(-y, x);
}


inline bool Vector::isNull() const
{
	return (x == 0.0f) && (y == 0.0f);
}


#endif
