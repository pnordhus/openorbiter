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


#ifndef BOUNCER_H
#define BOUNCER_H


#include "physics/rect.h"
#include <QGraphicsRectItem>


class Scene;
class World;
class BouncerDef;


class Bouncer
{
public:
	Bouncer(Scene& scene, World* world);
	~Bouncer();

public:
	void	setDef(const BouncerDef& def);

private:
	void	updateItem();

private:
	QGraphicsRectItem*		m_item;
	QGraphicsEllipseItem*	m_itemCap1;
	QGraphicsEllipseItem*	m_itemCap2;
	Rect*					m_rect;

private:
	Q_DISABLE_COPY(Bouncer);
};


#endif // BOUNCER_H
