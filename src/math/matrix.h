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


#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H


#include "vector.h"


namespace Math
{

	class Matrix
	{
	public:
		Vector		r;
		Float		d1;
		Vector		u;
		Float		d2;
		Vector		f;
		Float		d3;
		Vector		pos;
		Float		d4;
		
	public:
		Matrix&		operator += (const Vector&);
		Matrix&		operator -= (const Vector&);
		Matrix&		operator *= (const Matrix&);
		
		Matrix		operator *  (const Matrix&) const;
		
		Vector		operator -  (const Matrix&) const;
		
		Vector		operator [] (unsigned int index) const;
		
	public:
		void		identity();
	} NO_ALIGN;

}


namespace std
{
	ostream& operator << (ostream& out, const Math::Matrix& mat);
}


#endif
