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


#include "config.h"
#include "graphicsview.h"


/****************************************************************************/


GraphicsView::GraphicsView(QWidget* parent) :
	QGraphicsView(parent)
{
#ifdef BUILD_OPENGL
	QGLFormat f;
	f.setSwapInterval(1);
	f.setSampleBuffers(0);
	QGLFormat::setDefaultFormat(f);

	configChanged("useOpenGL");

	connect(g_config, SIGNAL(changed(const QString&)), this, SLOT(configChanged(const QString&)));
#endif
}


/****************************************************************************/


void GraphicsView::resize(float w, float h)
{
	w = float(width() - 10) / w;
	h = float(height() - 10) / h;

	float r = qMin(w, h);

	QMatrix mat;
	mat.scale(r, r);
	setMatrix(mat);
}


/****************************************************************************/


#ifdef BUILD_OPENGL


void GraphicsView::configChanged(const QString& name)
{
	if (name != "useOpenGL")
		return;

	if (g_config->getBool("useOpenGL"))
		setViewport(new QGLWidget);
	else
		setViewport(new QWidget);
}


#endif


/****************************************************************************/


void GraphicsView::resizeEvent(QResizeEvent* e)
{
	Q_ASSERT(scene());

	const QRectF& rect = scene()->sceneRect();
	resize(rect.width(), rect.height());
	QGraphicsView::resizeEvent(e);
}
