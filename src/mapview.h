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


#ifndef OPENORBITER_MAPVIEW_H
#define OPENORBITER_MAPVIEW_H


#include "map.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>


class MapView : public QGraphicsView
{
public:
	MapView(QWidget*);

	void	setMap(const Map* map);
	void	setView(QGraphicsView*);
	void	updateSize();
	void	process();

private:
	void	resizeEvent(QResizeEvent*);

private:
	QGraphicsView*	m_view;
	QGraphicsScene	m_scene;
	const Map*		m_map;

	QGraphicsRectItem*		m_background;
};


#endif
