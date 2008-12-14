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


#ifndef TYPES_H
#define TYPES_H


typedef double			Float;
typedef int			Int;
typedef unsigned int		UInt;
	
typedef signed char		Int8;
typedef signed short		Int16;
typedef signed int		Int32;
typedef signed long long	Int64;
	
typedef unsigned char		UInt8;
typedef unsigned short		UInt16;
typedef unsigned int		UInt32;
typedef unsigned long long	UInt64;


#include <cassert>
#include <iostream>
#include <stdexcept>


#define NO_ALIGN                __attribute__ ((packed))


using namespace std;


#endif
