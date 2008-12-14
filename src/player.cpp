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


#include "player.h"
#include <GL/gl.h>


Player::Player(Color color, SDLKey key) :
	Orbiter(0.0f, 0.0f),
	m_Key(key),
	m_Color(color),
	m_Wins(0),
	m_TotalFrags(0),
	m_Frags(0)
{

}


Player::~Player()
{

}


SDLKey Player::getKey() const
{
	return m_Key;
}


Color Player::getColor() const
{
	return m_Color;
}


int Player::getWins() const
{
	return m_Wins;
}


int Player::getFrags() const
{
	return m_Frags;
}


int Player::getTotalFrags() const
{
	return m_TotalFrags;
}


void Player::reset(Float x, Float y)
{
	Orbiter::reset(x, y);
	m_Frags = 0;
}


void Player::process()
{
	Orbiter::process();
}


void Player::render()
{
	glColor4f(m_Color.r, m_Color.g, m_Color.b, 1.0f);
	Orbiter::render();
}


void Player::win()
{
	m_Wins++;
}


void Player::frag()
{
	++m_Frags;
	++m_TotalFrags;
}


void Player::collide(Collider& collider)
{
	Orbiter::collide(collider);
}
