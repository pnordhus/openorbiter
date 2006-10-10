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


#ifndef OPENORBITER_MATCHSTATS_H
#define OPENORBITER_MATCHSTATS_H


#include <QtGlobal>


class MatchStats
{
public:
	MatchStats();

	void	reset();

	void	incLosses();
	void	incWins();
	void	incWay(float way);
	void	incTime(float time);
	void	checkTopSpeed(float speed);

	int		frags() const { return m_frags; }
	uint	losses() const { return m_losses; }
	uint	wins() const { return m_wins; }
	float	way() const { return m_way; }
	float	speed() const { if (m_time == 0.0f) return 0.0f; return m_way / m_time; }
	float	topSpeed() const { return m_topSpeed; }
	float	time() const { return m_time; }

private:
	int		m_frags;
	uint	m_losses;
	uint	m_wins;
	float	m_way;
	float	m_time;
	float	m_topSpeed;
};


#endif
