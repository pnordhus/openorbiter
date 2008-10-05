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


#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H


#include <QGraphicsItem>
#include <QSvgRenderer>


class RenderManager
{
public:
	QGraphicsItem*	createNodeItem(float radius);
	QGraphicsItem*	createOrbiterItem(float radius, const QColor& color);

public:
	static void	create();
	static void	destroy();
	static RenderManager& get() { Q_ASSERT(m_singleton); return *m_singleton; }

private:
	RenderManager();
	~RenderManager();

private:
	void	loadRenderer(const QString& name, const QColor& color);

private:
	QSvgRenderer*					m_rendererNode;
	QMap<QString, QSvgRenderer*>	m_rendererOrbiter;

private:
	static RenderManager*	m_singleton;

private:
	Q_DISABLE_COPY(RenderManager);
};


#endif // RENDERMANAGER_H
