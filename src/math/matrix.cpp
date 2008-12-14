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


#include "matrix.h"


using namespace Math;


void Matrix::identity()
{
	memset(this, 0, sizeof(Matrix));
	r.x = (Float) 1.0;
	u.y = (Float) 1.0;
	f.z = (Float) 1.0;
	d4  = (Float) 1.0;
}


Matrix&	Matrix::operator += (const Vector& vec)
{
	pos += vec;
	return *this;
}


Matrix&	Matrix::operator -= (const Vector& vec)
{
	pos -= vec;
	return *this;
}


Matrix& Matrix::operator *= (const Matrix& mat)
{
	return (*this = (*this * mat));
}


Matrix Matrix::operator * (const Matrix& mat) const
{
	Matrix tmp = *this;
	Vector a;
	Vector b;
	Vector c;
	
	a.x = r.x; a.y = u.x; a.z = f.x;
	b.x = r.y; b.y = u.y; b.z = f.y;
	c.x = r.z; c.y = u.z; c.z = f.z;
	
	tmp.r.x = mat.r.dot(a);
	tmp.r.y = mat.r.dot(b);
	tmp.r.z = mat.r.dot(c);

	tmp.u.x = mat.u.dot(a);
	tmp.u.y = mat.u.dot(b);
	tmp.u.z = mat.u.dot(c);
	
	tmp.f.x = mat.f.dot(a);
	tmp.f.y = mat.f.dot(b);
	tmp.f.z = mat.f.dot(c);

	return tmp;
}


Vector Matrix::operator - (const Matrix& mat) const
{
	return pos - mat.pos;
}


Vector Matrix::operator [] (unsigned int index) const
{
	Vector vec;
	switch (index) {
		case 0:  vec.x = r.x; vec.y = u.x; vec.z = f.x; break;
		case 1:  vec.x = r.y; vec.y = u.y; vec.z = f.y; break;
 		case 2:  vec.x = r.z; vec.y = u.z; vec.z = f.z; break;
		default: vec.zero();
	}
	
	return vec;
}


std::ostream& std::operator << (std::ostream& out, const Matrix& mat)
{
	out << "( ";
	out << mat.r << " " << mat.d1 << " | ";
	out << mat.u << " " << mat.d2 << " | ";
	out << mat.f << " " << mat.d3 << " | ";
	out << mat.pos << " " << mat.d4;
	out << " )";
	
	return out;
}
