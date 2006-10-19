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


#include "frame_map.h"
#include "openorbiter.h"
#include "orbiter.h"

#include <QPainter>
#include <QDebug>


FrameMap::FrameMap(QWidget* parent) :
	QWidget(parent)
{
	setAutoFillBackground(true);
}


void FrameMap::process()
{
	QRegion newRegion;
	foreach (Player* player, g_openorbiter->game()->players()) {
		QRect rect = g_openorbiter->game()->drawingRect(player->getOrbiter(), width());
		newRegion += rect.adjusted(-2, -2, 2, 2);
	}

	m_updateRegion += newRegion;
	update(m_updateRegion);
	m_updateRegion = QRegion();
}


void FrameMap::paintEvent(QPaintEvent*)
{
	if (!g_openorbiter->isRunning())
		return;

	QPainter painter(this);
	//qDebug() << e->region();
	painter.setBrush(Qt::SolidPattern);
	//painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);	

	QPen oldPen = painter.pen();
	QPen pen = oldPen;
	pen.setStyle(Qt::SolidLine);
	pen.setCapStyle(Qt::SquareCap);
	pen.setJoinStyle(Qt::MiterJoin);
	pen.setWidth(2);

	foreach (Player* player, g_openorbiter->game()->players()) {
		const Orbiter& orb = player->getOrbiter();
		const Node* node = orb.node();
		if (!node)
			continue;

		QPoint posOrb = g_openorbiter->game()->drawingPos(orb.getPosition(), width());
		QPoint posNode = g_openorbiter->game()->drawingPos(*node, width());

		if (orb.isConnected())
			pen.setColor("white");
		else
			pen.setColor("red");

		painter.setPen(pen);

		painter.drawLine(posOrb, posNode);
		QRect rect;
		rect.setLeft  (posOrb.x() <  posNode.x() ? posOrb.x() : posNode.x());
		rect.setRight (posOrb.x() >= posNode.x() ? posOrb.x() : posNode.x());
		rect.setTop   (posOrb.y() <  posNode.y() ? posOrb.y() : posNode.y());
		rect.setBottom(posOrb.y() >= posNode.y() ? posOrb.y() : posNode.y());

		m_updateRegion += rect.adjusted(-2, -2, 2, 2);
	}

	painter.setPen(oldPen);
	painter.setPen(QColor("black"));

	foreach (const Node& node, g_openorbiter->game()->getNodes()) {
		QRect rect = g_openorbiter->game()->drawingRect(node, width());
		painter.setBrush(node.getColor());
		painter.drawEllipse(rect);
		m_updateRegion += rect.adjusted(-2, -2, 2, 2);
	}

	foreach (Player* player, g_openorbiter->game()->players()) {
		const Orbiter& orb = player->getOrbiter();
		painter.setBrush(player->getColor());
		QRect rect = g_openorbiter->game()->drawingRect(orb, width());
		painter.drawEllipse(rect);
		m_updateRegion += rect.adjusted(-2, -2, 2, 2);
	}
}


void FrameMap::recalcSize()
{
	const qreal w = parentWidget()->width();
	const qreal h = parentWidget()->height();

	qreal ratio;
	if (g_openorbiter->isRunning())
		ratio = g_openorbiter->game()->getMap().getAspectRatio();
	else
		ratio = 2.0 / 1.0;

	int newWidth, newHeight;
	int newX, newY;

	if ((w - 40.0) > ratio * (h - 40.0)) {
		newWidth = int(ratio * (h - 40.0));
		newHeight = int(h - 40.0);
		newX = int((w - newWidth) / 2);
		newY = 20;
	} else {
		newWidth = int(w - 40.0);
		newHeight = int((w - 40.0) / ratio);
		newX = 20;
		newY = int((h - newHeight) / 2);
	}

	resize(newWidth, newHeight);
	move(newX, newY);

	if (g_openorbiter->isRunning()) {
		//repaint();
	}
}


void FrameMap::showEvent(QShowEvent *)
{
	//recalcSize();
}


void FrameMap::focusOutEvent(QFocusEvent*)
{
	//g_openorbiter->pause();
}
