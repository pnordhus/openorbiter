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


#ifndef OPENORBITER_ORBITER_H
#define OPENORBITER_ORBITER_H


#include "vector.h"


#include <QGraphicsEllipseItem>
#include <QRect>


class Node;
class Player;


class Orbiter
{
public:
	Orbiter(Player& player);

	void	reset();

	void	process(float time, const Vector& gravity);

	float	getRadius() const { return m_radius; }
	const Vector& getPosition() const { return m_position; }

	void	setPosition(const Vector& pos);

	void	setNode(const Node* node);
	bool	needsNode() const;
	bool	isConnected() const { return m_connected; }
	const Node* node() const { return m_connectionNode; }

	static void	collide(Orbiter&, Orbiter&);

	void	toggle();

	QGraphicsEllipseItem&	item() { return m_item; }

	void	connectScene(QGraphicsScene*);
	void	disconnectScene();

private:
	Player&	m_player;

	float	m_radius;
	float	m_mass;
	Vector	m_position;
	Vector	m_speed;
	float	m_nodeRadius;
	float	m_angle;
	float	m_angleSpeed;
	float	m_collisionTimer;

	bool	m_tryConnect;
	bool	m_connected;
	const Node*	m_connectionNode;

	QGraphicsEllipseItem	m_item;
	QGraphicsLineItem		m_line;
};


#endif
