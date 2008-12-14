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


#ifndef CLIENT_FONTDATA_H
#define CLIENT_FONTDATA_H


#include "Types.h"
#include <set>


enum FontFlags {
	FF_NORMAL	= 0x00,
	FF_BOLD		= 0x01,
	FF_ITALIC	= 0x02,
};


class Font;
class FontDataImpl;


class FontData
{
public:
	virtual UInt		getHeight() const = 0;
	virtual bool		isLoaded() const = 0;
	
	virtual void		load() = 0;
	virtual void		unload() = 0;

	virtual UInt		getNumChars(UInt width, const wstring& text, Float spacing) = 0;
	virtual UInt		getWidth(const wstring& text, UInt spacing = 0) = 0;
	virtual UInt		printLine(int x, int y, UInt width, const wstring& line, Float spacing, int cursorpos = -1, bool draw = true) = 0;

public:
	static FontData&	get(const Font& font);
	static void		unloadAll();
	
private:
	static std::set<FontDataImpl>	m_Fonts;
};


#endif
