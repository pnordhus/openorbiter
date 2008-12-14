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


#include "input.h"



// static variables
std::map<SDLKey, Player*>	Input::m_Bindings;



bool Input::bindKey(SDLKey key, Player& player)
{
	return m_Bindings.insert(std::make_pair(key, &player)).second;
}


void Input::unbindKey(SDLKey key)
{
	m_Bindings.erase(key);
}


void Input::unbindAll()
{
	m_Bindings.clear();
}


bool Input::query()
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
		
		case SDL_KEYDOWN:
			SDL_KeyboardEvent *kevent = (SDL_KeyboardEvent*) &event;
			if (kevent->keysym.sym == SDLK_ESCAPE)
				return false;
			
			processInput(kevent->keysym.sym);
			break;
		}
	}
	
	return true;
}


void Input::processInput(SDLKey key)
{
	std::map<SDLKey, Player*>::const_iterator it = m_Bindings.find(key);
	if (it != m_Bindings.end()) {
		(*it).second->toggleConnect();
	}
}
