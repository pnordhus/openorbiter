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


#include "graphicsscene.h"


/****************************************************************************/


GraphicsScene::GraphicsScene()
{
	setBackgroundBrush(Qt::black);
	m_background.setPen(Qt::NoPen);
	m_background.setZValue(0.0f);
	addItem(&m_background);
}


/****************************************************************************/


void GraphicsScene::setBackgroundColor(const QColor& color)
{
	m_background.setBrush(color);
}


/****************************************************************************/


void GraphicsScene::setSize(float w, float h)
{
	m_background.setRect(0.0f, 0.0f, w, h);
	setSceneRect(0.0f, 0.0f, w, h);

	emit sizeChanged();
}
