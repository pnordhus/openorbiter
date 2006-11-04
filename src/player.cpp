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
#include "openorbiter.h"
#include "player.h"


#include <QDebug>


Player::Player(int id, const QString& name, const QColor& color, const QString& filename) :
	m_id(id),
	m_name(name),
	m_color(color),
	m_orbiter(*this, filename),
	m_isSelected(false)
{

}


void Player::setName(const QString& name)
{
	m_name = name;
}


void Player::setSelected(bool selected)
{
	m_isSelected = selected;
}


void Player::setKey(const QKeySequence& k)
{
	m_key = k;
}


void Player::startMatch()
{
	//qDebug() << "startMatch():" << m_name;

	m_matchStats.reset();
}


void Player::endMatch()
{
	//qDebug() << "endMatch():" << m_name;
}


void Player::startGame()
{
	//qDebug() << "startGame():" << m_name;

	m_orbiter.reset();
	m_gameStats.reset();

	m_orbiter.connectScene(g_openorbiter->graphicsScene());
}


void Player::endGame(bool won)
{
	m_orbiter.disconnectScene();

	//qDebug() << "endGame():" << m_name;
	if (won)
		m_matchStats.incWins();
	else
		m_matchStats.incLosses();
}


void Player::toggle()
{
	m_orbiter.toggle();
}
