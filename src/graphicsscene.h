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


#ifndef OPENORBITER_GRAPHICSSCENE_H
#define OPENORBITER_GRAPHICSSCENE_H


class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GraphicsScene();

	void	setMapColor(const QColor& color);
	void	setSize(float w, float h);

signals:
	void	sizeChanged(float w, float h);

public slots:
	void	setStatusColor(const QColor& color);

private slots:
	void	togglePaused(bool);

private:
	void	updatePauseText();

private:
	QGraphicsRectItem	m_background;
	QGraphicsRectItem	m_backgroundStatus;
	QGraphicsRectItem	m_map;
	QGraphicsTextItem	m_pauseText;
	QGraphicsRectItem	m_pauseTextBackground;
};


#endif // OPENORBITER_GRAPHICSSCENE_H
