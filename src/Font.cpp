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


#include "Font.h"
#include "SDL_ttf.h"
#include <GL/glu.h>
#include "renderer.h"
#include <fstream>


struct PrintLine {
	int	y;
	UInt	first;
	UInt	count;
	Float	spacing;
	
	PrintLine(int y, UInt first, UInt count, Float spacing) : y(y), first(first), count(count), spacing(spacing) {}
};


Font Font::defaultFont("../fonts/vera.ttf", 16);


Font::Font(const string& name, UInt size, UInt flags) :
	m_Name(name),
	m_Size(size),
	m_Flags(flags),
	m_FontData(NULL)
{

}


const string& Font::getName() const
{
	return m_Name;
}


UInt Font::getSize() const
{
	return m_Size;
}


UInt Font::getFlags() const
{
	return m_Flags;
}


UInt Font::getHeight()
{
	if (!m_FontData)
		m_FontData = &FontData::get(*this);
	
	return m_FontData->getHeight();
}


UInt Font::printText(int x, int y, UInt width, const wstring& text, FontAlignment alignment, int cursorpos)
{
	if (!m_FontData)
		m_FontData = &FontData::get(*this);

	if (!m_FontData->isLoaded())
		m_FontData->load();

	if (text.size() == 0) {
		m_FontData->printLine(x, y, width, L"", 0, cursorpos);
		return 0;
	}

	UInt cut_left = 0;
	UInt cut_right = 0;
	
	while ((cut_left < text.size()) && (text[cut_left] == ' '))
		++cut_left;
	
	wstring line = text.substr(cut_left);
	line = line.substr(0, m_FontData->getNumChars(width, line, (Float) 0));
	
	if ((line.size() + cut_left < text.size()) && (text[cut_left + line.size()] != ' ')) {
		while ((cut_right < line.size()) && (line[line.size() - 1 - cut_right] != ' '))
			++cut_right;
	}
	
	
	while ((cut_right < line.size()) && (line[line.size() - 1 - cut_right] == ' '))
		++cut_right;
	
	line = line.substr(0, line.size() - cut_right);
	
	Float spacing = (Float) 0;
	
	UInt pos = text.find('\n');
	if (pos <= cut_left) {
		return pos + 1;
	}
	if (pos - cut_left < line.size()) {
		line.erase(pos - cut_left, line.size());
		++cut_left;
	}
	
	
	UInt chars_width = m_FontData->getWidth(line);
	UInt total_width = chars_width + (UInt) (spacing * (line.size() - 1));
	
	switch (alignment) {
	case FA_LEFT:
		break;
		
	case FA_RIGHT:
		x += width - total_width;
		break;
		
	case FA_CENTER:
		x += (width - total_width) / 2;
		break;
		
	case FA_BLOCK:
		if (line.size() > 1) {
			Float new_spacing = (Float) (width - chars_width) / (line.size() - 1);
			if (new_spacing <= m_FontData->getHeight() / 4.0)
				spacing = new_spacing;
		}
		break;
	}
	
	
	int last = line.size();
	if (last < cursorpos) {
		line = text.substr(cut_left, cursorpos);
	}
	
	UInt printed = m_FontData->printLine(x, y, width, line, 0, cursorpos);

	return cut_left + printed;
}


UInt Font::countLines(UInt width, const wstring& text)
{
	if (!m_FontData)
		m_FontData = &FontData::get(*this);

	if (!m_FontData->isLoaded())
		m_FontData->load();

	wstring out = text;
	UInt count = 0;
		
	while (out.size() > 0) {
		UInt cut_left = 0;
		UInt cut_right = 0;
		
		while ((cut_left < out.size()) && (out[cut_left] == ' '))
			++cut_left;
		
		wstring line = out.substr(cut_left);
		line = line.substr(0, m_FontData->getNumChars(width, line, 0.0));
		
		if ((line.size() + cut_left < out.size()) && (out[cut_left + line.size()] != ' ')) {
			while ((cut_right < line.size()) && (line[line.size() - 1 - cut_right] != ' '))
				++cut_right;
		}
		
		
		while ((cut_right < line.size()) && (line[line.size() - 1 - cut_right] == ' '))
			++cut_right;
		
		line = line.substr(0, line.size() - cut_right);
		
		Float spacing = 0.0;
		
		UInt pos = out.find('\n');
		if (pos <= cut_left) {
			out = out.substr(pos + 1);
			continue;
		}
		if (pos - cut_left < line.size()) {
			line.erase(pos - cut_left, line.size());
			++cut_left;
		}
	
		UInt printed = m_FontData->printLine(0, 0, width, line, 0.0, -1, false);
		++count;
		
		out = out.substr(cut_left + printed);
		if (cut_left + printed == 0)
			break;
	}

	return count;
}


UInt Font::print(int x, int y, UInt w, UInt h, const wstring& text, PrintFlags flags, int cursor_pos)
{
	if (!m_FontData)
		m_FontData = &FontData::get(*this);

	if (!m_FontData->isLoaded())
		m_FontData->load();

	if ((flags & PF_WRAP_UP) == 0)
		return printNoWrap(x, y, w, text, flags, cursor_pos);
	
	wstring line = text;

	std::vector<PrintLine> lines;
	
	UInt count = 0;
	UInt num = 0;
	UInt total = 0;
	
	do {
		line = line.substr(num);
		num = m_FontData->getNumChars(w, line, 0.0);
		lines.push_back(PrintLine(y, total, num, 0.0));
		total += num;
		
		if ((flags & PF_WRAP_UP) == PF_WRAP_UP)
			y -= m_FontData->getHeight() + 1;
		else
			y += m_FontData->getHeight() + 1;
	} while ((num > 0) && (num < line.size()));
		
	for (UInt i = 0; i < lines.size(); ++i) {
		const PrintLine& pline = ((flags & PF_WRAP_UP) == PF_WRAP_UP) ?
			lines[lines.size() - i - 1] :
			lines[i];

		count += m_FontData->printLine(x, pline.y, w, text.substr(pline.first, pline.count), pline.spacing, cursor_pos);
	}

	return count;
}


UInt Font::printNoWrap(int x, int y, UInt w, const wstring& text, PrintFlags flags, int cursor_pos)
{
	if ((flags & PF_CENTER) == PF_CENTER) {
		x += ((int) w - m_FontData->getWidth(text, 0)) / 2;
	} else if ((flags & PF_RIGHT) == PF_RIGHT) {
		x += w - m_FontData->getWidth(text, 0);
	} else if ((UInt) cursor_pos <= text.size()) {
		UInt width = m_FontData->getWidth(text.substr(0, cursor_pos), 0);
		if (width > w)
			x = x + w - width;
	}
	
	return m_FontData->printLine(x, y, w, text, 0, cursor_pos);
}
