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


#include "form_main.h"
#include "frame_mapback.h"
#include "match.h"
#include "openorbiter.h"
#include "player.h"


#include <QPainter>


FrameMapBack::FrameMapBack(QWidget* parent) :
	QWidget(parent),
	m_formMain(NULL)
{
	setAutoFillBackground(true);
	QPalette p = palette();
	p.setColor(QPalette::Window, QColor(0,0,0));
	setPalette(p);
}


void FrameMapBack::setFormMain(FormMain* form)
{
	m_formMain = form;
}



void FrameMapBack::resizeEvent(QResizeEvent*)
{
	Q_ASSERT(m_formMain);
	m_formMain->updateMapFrame();
}


void FrameMapBack::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);

	const Player* last = NULL;
	if (g_openorbiter->isRunning())
		last = g_openorbiter->match()->lastWinner();

	QColor color("black");
	if (last) {
		color = last->getColor();
	}

	QPen pen = painter.pen();
	pen.setStyle(Qt::SolidLine);
	pen.setCapStyle(Qt::SquareCap);
	pen.setJoinStyle(Qt::MiterJoin);
	pen.setColor(color);
	pen.setWidth(5);
	painter.setPen(pen);

	QRect rect = m_formMain->mapGeometry();
	rect.adjust(-10, -10, 10, 10);
	painter.drawRect(rect);
}
