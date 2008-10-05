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


#include "view.h"
#include <QKeyEvent>
#include <QGLWidget>


View::View(QWidget* parent) :
	QGraphicsView(parent),
	m_gl(false),
	m_antiAliasing(false)
{
	
}


void View::enableGL(bool enable)
{
	switchSettings(enable, m_antiAliasing);
}


void View::enableAntiAliasing(bool enable)
{
	switchSettings(m_gl, enable);
}


void View::switchSettings(bool gl, bool antiAliasing)
{
	if (gl) {
		if (antiAliasing)
			setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
		else
			setViewport(new QGLWidget);
	} else {
		setViewport(new QWidget);
		
		QPainter::RenderHints hints = renderHints();
		
		if (antiAliasing)
			hints |= QPainter::Antialiasing;
		else
			hints &= ~QPainter::Antialiasing;
		
		setRenderHints(hints);
	}
	
	m_antiAliasing = antiAliasing;
	m_gl = gl;
}


void View::resizeEvent(QResizeEvent* e)
{
	fitInView(sceneRect().adjusted(-0.1, -0.1, 0.1, 0.1), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(e);
}


void View::focusInEvent(QFocusEvent* e)
{
	QGraphicsView::focusInEvent(e);
	//grabKeyboard();
	//emit resume();
}


void View::focusOutEvent(QFocusEvent* e)
{
	emit pause();
	//releaseKeyboard();
	QGraphicsView::focusOutEvent(e);
}


void View::keyPressEvent(QKeyEvent* e)
{
	emit keyPressed(e->key());
}
