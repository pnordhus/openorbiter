/***************************************************************************
 *   Copyright (C) 2008 Philipp Nordhus                                    *
 *   pnordhus@users.sourceforge.net                                        *
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


#include <QColor>


class Player
{
public:
	Player(const QString& name, const QColor& color);

public:
	const QString&	name() const { return m_name; }
	const QColor&	color() const { return m_color; }
	int				key() const { return m_key; }
	void			setKey(int key);
	void			incWins();
	uint			wins() const { return m_wins; }

private:
	QString	m_name;
	QColor	m_color;
	int		m_key;
	uint	m_wins;
};


#endif // PLAYER_H
