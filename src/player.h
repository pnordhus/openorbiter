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


#ifndef PLAYER_H
#define PLAYER_H


#include "SDL.h"
#include "orbiter.h"
#include "renderer.h"


class Player : public Orbiter
{
public:
	Player(Color color, SDLKey key);
	virtual ~Player();
	SDLKey	getKey() const;
	Color	getColor() const;
	int	getWins() const;
	int	getContacts() const;
	int	getLastContacts() const;
	int	getFrags() const;
	int	getTotalFrags() const;

public:
	void	reset(Float x, Float y);
	void	process();
	void	render();
	
	void	win();
	void	frag();
	virtual void	collide(Collider& collider);
	
private:
	SDLKey	m_Key;
	Color	m_Color;
	int	m_Wins;
	int	m_TotalFrags;
	int	m_Frags;
};


#endif
