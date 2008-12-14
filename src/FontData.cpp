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


#include "FontData.h"
#include "Font.h"
#include <GL/gl.h>
#include "SDL_ttf.h"
#include <fstream>


class FontDataImpl : public FontData
{
private:
	struct CharInfo {
		float x1, y1, x2, y2;
		UInt w;
	};

public:
	FontDataImpl(string name, UInt size, UInt flags = FF_NORMAL);
	FontDataImpl(const Font& font);
	virtual ~FontDataImpl();
	bool		operator < (const FontDataImpl& font) const;
	
	UInt		getHeight() const;
	bool		isLoaded() const;

public:
	void		load();
	void		unload();

	UInt		getNumChars(UInt width, const wstring& text, Float spacing);
	UInt		getWidth(const wstring& text, UInt spacing = 0);
	UInt		printLine(int x, int y, UInt width, const wstring& line, Float spacing, int cursorpos = -1, bool draw = true);

private:
	void		beginPrint();
	void		printChar(int x, int y, const CharInfo& c);
	void		printCursor(int x, int y);
	void		endPrint();
	

public:
	static FontData&	get(const Font& font);
	static void		unloadAll();
	
private:
	string			m_Name;
	UInt			m_Size;
	UInt			m_Flags;
	CharInfo*		m_CharInfo;
	UInt			m_Texture;
	UInt			m_Height;
	bool			m_Loaded;
	unsigned char*		m_TexBuf;
	UInt			m_TexW;
	UInt			m_TexH;
	
	static const UInt FIRST_CHAR	= 32;
	static const UInt LAST_CHAR	= 511;
};


std::set<FontDataImpl> FontData::m_Fonts;



struct RGBA
{
	unsigned char r,g,b,a;	
};



FontDataImpl::FontDataImpl(string name, UInt size, UInt flags) :
	m_Name(name),
	m_Size(size),
	m_Flags(flags),
	m_CharInfo(NULL),
	m_Texture(0),
	m_Height(0),
	m_Loaded(false),
	m_TexBuf(NULL)
{

}


FontDataImpl::~FontDataImpl()
{
	unload();
	if (m_CharInfo)
		delete [] m_CharInfo;
	if (m_TexBuf)
		delete [] m_TexBuf;
}


FontDataImpl::FontDataImpl(const Font& font) :
	m_Name(font.getName()),
	m_Size(font.getSize()),
	m_Flags(font.getFlags()),
	m_CharInfo(NULL),
	m_Texture(0),
	m_Height(0),
	m_Loaded(false),
	m_TexBuf(NULL)
{
	
}


bool FontDataImpl::isLoaded() const
{
	return m_Loaded;
}


void FontDataImpl::load()
{
	unload();

	if (!m_TexBuf) {
	
		TTF_Font* font = TTF_OpenFont(m_Name.c_str(), m_Size);
		if (!font) {
			m_TexW = 512;
			m_TexH = 512;
			m_TexBuf = new unsigned char[m_TexW * m_TexH];
			m_CharInfo = new CharInfo[LAST_CHAR - FIRST_CHAR + 1];
			
			ifstream file((m_Name + ".vtf").c_str(), ios::binary);
			file.read((char *) &m_Height, sizeof(UInt));
			file.read((char *) m_CharInfo, sizeof(CharInfo[LAST_CHAR - FIRST_CHAR + 1]));
			file.read((char *) m_TexBuf, m_TexW * m_TexH);	
		} else {
		
		SDL_Surface* newsur = SDL_CreateRGBSurface(SDL_SWSURFACE, 512, 512, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		
		std::cout << "Loading font: " << TTF_FontFaceFamilyName(font) << std::endl;
			
		m_TexW = 512;
		m_TexH = 512;
		
		GLint texSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		cout << "Max texture size: " << texSize << "x" << texSize << endl;
		
		SDL_Color white; white.r = 255; white.g = 255; white.b = 255;
		
		m_TexBuf = new unsigned char[m_TexW * m_TexH];
		memset(m_TexBuf, 0, sizeof(unsigned char[m_TexW * m_TexH]));
		
		if (!m_CharInfo) {
			m_CharInfo = new CharInfo[LAST_CHAR - FIRST_CHAR + 1];
			memset(m_CharInfo, 0, sizeof(CharInfo[LAST_CHAR - FIRST_CHAR + 1]));
		}
		
		UInt x = 1;
		UInt y = 1;
		
		UInt maxx = 1;
		
		SDL_Rect rect;
		rect.h = 0;
		UInt16 str[2]; str[1] = 0;
		for (UInt i = FIRST_CHAR; i <= LAST_CHAR; ++i) {
			str[0] = i;
			SDL_Surface* surface = TTF_RenderUNICODE_Blended(font, str, white);
			
			if (!surface)
				throw runtime_error(TTF_GetError());
			
			if (x + surface->w + 1 >= m_TexW) {
				x = 1;
				y += surface->h + 1;
			}
			
			if (x > maxx)
				maxx = x;
			
			rect.x = x;
			rect.y = y;
			rect.w = surface->w;
			rect.h = surface->h;
			
			SDL_Rect r2 = rect;
			
			for (int y = 0; y < rect.h; ++y) {
				for (int x = 0; x < rect.w; ++x) {
					UInt buf_index = (rect.y + y) * m_TexW + rect.x + x;
					UInt sur_index = (y * surface->w) + x;
					m_TexBuf[buf_index] = ((RGBA*) surface->pixels)[sur_index].a;
				}
			}
			
			m_CharInfo[i - FIRST_CHAR].x1 = (float) (rect.x) / (float) (m_TexW);
			m_CharInfo[i - FIRST_CHAR].y1 = (float) (rect.y) / (float) (m_TexH);
			m_CharInfo[i - FIRST_CHAR].x2 = (float) (rect.x + rect.w) / (float) (m_TexW);
			m_CharInfo[i - FIRST_CHAR].y2 = (float) (rect.y + rect.h) / (float) (m_TexH);
			m_CharInfo[i - FIRST_CHAR].w = rect.w;
			
			x += surface->w + 1;
	
			SDL_FreeSurface(surface);
		}
		
		cout << "Width: " << maxx + rect.w + 1 << ", Height: " << y + rect.h + 1 << endl;
		
		m_Height = rect.h;
		TTF_CloseFont(font);
		
		SDL_SaveBMP(newsur, "out.bmp");
		
		SDL_FreeSurface(newsur);
	
		ofstream file((m_Name + ".vtf").c_str(), ios::binary);
		file.write((char *) &m_Height, sizeof(UInt));
		file.write((char *) m_CharInfo, sizeof(CharInfo[LAST_CHAR - FIRST_CHAR + 1]));
		file.write((char *) m_TexBuf, m_TexW * m_TexH);
		}
	}
		
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_TexW, m_TexH, 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_TexBuf);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	


	m_Loaded = true;
}


void FontDataImpl::unload()
{
	if (m_Loaded) {
		glDeleteTextures(1, &m_Texture);
		m_Loaded = false;
	}
}

/*
void Font::load(int size) const
{
	string filename = m_Name;

	cout << "Loading font: " << filename << endl;
	TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
	if (!font)
		throw runtime_error(TTF_GetError());
	
	int tex_width = 512;
	int tex_height = 512;
	
	SDL_Color fg;
	fg.r = 255;
	fg.g = 255;
	fg.b = 255;
	
	CharSize* chars = m_CharSize;
	unsigned char* buf = new unsigned char[tex_width * tex_height];
	memset(buf, 0, sizeof(unsigned char[tex_width * tex_height]));

	int y = 1;
	int x = 1;
	int maxx = 0;
	int height = TTF_FontHeight(font);
	
	SDL_Rect rect;
	
	for (unsigned char c = 32; c < 127; ++c) {
		char str[2];
		str[0] = c;
		str[1] = 0;
	
		SDL_Surface* sur = TTF_RenderText_Blended(font, str, fg);

		if (!sur)
			assert(sur);
	
		if ((x + sur->w) >= tex_width) {
			x = 1;
			y += height + 1;
		}
		
		if (x + sur->w > maxx)
			maxx = x + sur->w;
		
		rect.x = x;
		rect.y = y;
		rect.w = sur->w;
		rect.h = sur->h;
		
		for (int y2 = 0; y2 < rect.h; ++y2)
			for (int x2 = 0; x2 < rect.w; ++x2) {
				buf[((rect.y + y2) * tex_width + rect.x + x2)] = ((RGBA*) sur->pixels)[y2 * sur->w + x2].a;
			}

		chars[c].x1 = (float) rect.x / tex_width;
		chars[c].y1 = (float) rect.y / tex_height;
		chars[c].x2 = (float) (rect.x + rect.w) / tex_width;
		chars[c].y2 = (float) (rect.y + rect.h) / tex_height;
		chars[c].w = rect.w;

		SDL_FreeSurface(sur);
		
		x += rect.w + 1;
	}
	m_Height = rect.h;
	
	
	std::cout << maxx << ", " << y + height << std::endl;
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex_width, tex_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buf);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glDisable(GL_TEXTURE_2D);
	
	
	delete [] buf;

	TTF_CloseFont(font);
}
*/

bool FontDataImpl::operator < (const FontDataImpl& font) const
{
	if (m_Name != font.m_Name)
		return m_Name < font.m_Name;
	
	if (m_Size != font.m_Size)
		return m_Size < font.m_Size;
	
	return m_Flags < font.m_Flags;
}


UInt FontDataImpl::getHeight() const
{
	return m_Height;
}


UInt FontDataImpl::getNumChars(UInt width, const wstring& text, Float spacing)
{
	Float x = 0;
	for (UInt i = 0; i < text.size(); ++i) {
		const CharInfo& c = m_CharInfo[(UInt) text[i] - FIRST_CHAR];
		
		if (x + c.w >= width)
			return i;
		
		x += c.w + spacing;
	}
	
	return text.size();
}


UInt FontDataImpl::getWidth(const wstring& text, UInt spacing)
{
	UInt width = 0;
	for (UInt i = 0; i < text.size(); ++i) {
		const CharInfo& c = m_CharInfo[(UInt) text[i] - FIRST_CHAR];
		width += c.w + spacing;
	}
	return width;
}


UInt FontDataImpl::printLine(int x, int y, UInt width, const wstring& line, Float spacing, int cursorpos, bool draw)
{
	if (draw)
		beginPrint();
	
	int rx = (int) x;
	
	UInt i;
	for (i = 0; i < line.size(); ++i) {
		const CharInfo& c = m_CharInfo[(UInt) line[i] - FIRST_CHAR];
		//if ((int) rx + c.w > x + width)
		//	break;
		if (draw) {
			printChar((int) rx, y, c);
			if (cursorpos == (int) i)
				printCursor((int) rx - 1, y);
		}
		
		rx += c.w;
		rx += spacing;
	}
	
	if (draw) {
		if (cursorpos == (int) line.size())
			printCursor((int) rx - 1, y);

		endPrint();
	}
	
	return i;
}


void FontDataImpl::beginPrint()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glDisable(GL_DEPTH_TEST);
	
	glBegin(GL_QUADS);
}


void FontDataImpl::printChar(int x, int y, const CharInfo& c)
{
	if ((x + (int) c.w) < -1)
		return;
	
	int w = c.w;
	int h = getHeight();
	
	glTexCoord2f(c.x2, c.y1);
	glVertex2i(x + w, y);
	glTexCoord2f(c.x1, c.y1);
	glVertex2i(x, y);
	glTexCoord2f(c.x1, c.y2);
	glVertex2i(x, y + h);
	glTexCoord2f(c.x2, c.y2);
	glVertex2i(x + w, y + h);
	
	/*
	glTexCoord2f(1.0, 0.0);
	glVertex2i(m_TexW / 2, 0);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex2i(0, m_TexH / 2);
	glTexCoord2f(1.0, 1.0);
	glVertex2i(m_TexW / 2, m_TexH / 2);
	*/
	//glRasterPos2i(100, 100);
}


void FontDataImpl::printCursor(int x, int y)
{
	if (x < -1)
		return;

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x, y + getHeight());
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
}


void FontDataImpl::endPrint()
{
	glEnd();
	glPopMatrix();
	glPopAttrib();
}


FontData& FontData::get(const Font& font)
{
	return const_cast<FontDataImpl&>(*m_Fonts.insert(m_Fonts.end(), FontDataImpl(font)));
}


void FontData::unloadAll()
{
	for (std::set<FontDataImpl>::iterator it = m_Fonts.begin(); it != m_Fonts.end(); ++it) {
		const_cast<FontDataImpl&>(*it).unload();
	}
}
