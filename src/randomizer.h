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


#ifndef OPENORBITER_RANDOMIZER_H
#define OPENORBITER_RANDOMIZER_H


#include <cmath>


template<typename Container>
inline void randomize(Container &c)
{
    if (c.size() < 2)
		return;

	Container ret;

	while (!c.empty()) {
		//int num = c.size() - 1;
		//int index = lrint(float(num) * float(random()) / float(RAND_MAX));
		int index = random() % c.size();
		ret.append(c.takeAt(index));
	}

	c = ret;
}


#endif
