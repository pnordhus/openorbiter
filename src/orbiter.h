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


#ifndef ORBITER_H
#define ORBITER_H


#include "player.h"
#include <QGraphicsEllipseItem>


class Circle;
class Game;
class Node;
class Scene;
class Vector;
class World;


class Orbiter : public QObject
{
	Q_OBJECT

public:
	Orbiter(Player* player);
	~Orbiter();

public:
	Player*			player() const { return m_player; }
	const QString&	name() const { return m_player->name(); }
	const QColor&	color() const { return m_player->color(); }
	int				key() const { return m_player->key(); }
	Vector			position() const;
	float			radius() const;

public:
	void	setPosition(const Vector& position);
	void	process();
	void	update();
	
	void	enter(Game& game, World& world, Scene& scene);
	void	leave();
	
	void	setKey(int key);
	void	toggleConnect();

private slots:
	void	connectNode();
	void	disconnectNode();

private:
	Player*					m_player;
	
	QGraphicsEllipseItem*	m_graphicsItem;
	QGraphicsLineItem*		m_connectionLine;
	Circle*					m_circle;
	Game*					m_game;
	Node*					m_node;
	bool					m_connected;
};


#endif // ORBITER_H
