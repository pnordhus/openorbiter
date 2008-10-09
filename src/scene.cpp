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


#include "scene.h"
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPixmapCache>


Scene::Scene()
{
	m_field = addRect(0.0f, 0.0f, 40.0f, 30.0f, QPen(QColor(200, 0, 0), 0.2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin), QBrush(QColor(0,0,40)));
	
	m_text = addText("");
	m_text->setZValue(1000.0f);
	m_text->setDefaultTextColor(Qt::white);
	m_text->hide();
	
	m_textBackground = addRect(QRectF(), QPen(Qt::NoPen), QBrush(QColor(0,0,0,180)));
	m_textBackground->setZValue(900.0f);
	m_textBackground->hide();
}


Scene::~Scene()
{
	
}


void Scene::setField(float width, float height)
{
	// clear the pixmap cache to make sure all objects are rendered again
	QPixmapCache::clear();
	
	m_field->setRect(0.0f, 0.0f, width, height);
	setSceneRect(-0.2, -0.2, width + 0.4, height + 0.4);
	m_textBackground->setRect(sceneRect().adjusted(-1000.0f, -1000.0f, 1000.0f, 1000.0f));
	
	emit sizeChanged();
}


void Scene::showText(const QString& text)
{
	m_text->setTransform(QTransform());
	const float scale = sceneRect().height() / 300.0f;
 	m_text->scale(scale, scale);
	m_text->setTextWidth(sceneRect().width() / scale);
	m_text->setHtml(text);
	
	QRectF rect = m_text->sceneBoundingRect();
	QPointF pos;
	pos.setX((sceneRect().width() - rect.width()) / 2.0f);
	pos.setY((sceneRect().height() - rect.height()) / 2.0f);
	m_text->setPos(pos);
	
	m_text->show();	
	m_textBackground->show();
}


void Scene::hideText()
{
	m_text->hide();	
	m_textBackground->hide();
}
