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


#ifndef CLIENT_FONT_H
#define CLIENT_FONT_H


#include "FontData.h"
#include <set>


enum FontAlignment {
	FA_LEFT = 0,
	FA_RIGHT = 1,
	FA_CENTER = 2,
	FA_BLOCK = 3
};



enum PrintFlags {
	PF_DEFAULT	= 0x00,
	PF_RIGHT	= 0x01,
	PF_CENTER	= 0x03,
	PF_BOTTOM	= 0x04,
	PF_VCENTER	= 0x0C,
	PF_WRAP		= 0x10,
	PF_WRAP_UP	= 0x30,
};



class Texture;


class Font
{
public:
	Font(const string& name, UInt size, UInt flags = FF_NORMAL);
	const string&	getName() const;
	UInt		getSize() const;
	UInt		getFlags() const;
	UInt		getHeight();

public:
	UInt		printText(int x, int y, UInt width, const wstring& text, FontAlignment alignment, int cursorpos = -1);
	UInt		countLines(UInt width, const wstring& text);
	
	UInt		print(int x, int y, UInt w, UInt h, const wstring& text, PrintFlags flags, int cursor_pos = -1);

private:
	UInt		printNoWrap(int x, int y, UInt w, const wstring& text, PrintFlags flags, int cursor_pos);
	
public:
	static Font	defaultFont;

private:
	string		m_Name;
	UInt		m_Size;
	UInt		m_Flags;
	
	FontData*	m_FontData;
};


#endif
