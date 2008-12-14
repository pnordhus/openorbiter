/***************************************************************************
 *   Copyright (C) 2004 by Philipp Nordhus                                 *
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


#ifndef MERCURY_TIMER_H
#define MERCURY_TIMER_H


#include "common/Types.h"


#ifdef _WIN32
#  include <windows.h>
#else
#  include <sys/time.h>
#  include <unistd.h>
#endif


//! The Mercury game engine
namespace Mercury
{
	//! Calculate time between two frames.
	/*! This class calculates the time one frame took for
	 *  processing and renderering. Optionally you can specify
	 *  a maximum value for frames per seconds.
	 */
	class Timer
	{
	public:
		//! Set frameTime to time since last update.
		/*!
		 * \param max_fps Sleep if fps would be greater than this.
		 */
		static void		update(double max_fps);
		
	private:
		inline static void	init();
		inline static void	updateTime();
		inline static double	getTime();
		inline static void	sleep(UInt);
		
	public:
		//! Time since last update.
		static const double&	frameTime;
		
	private:
		static double		m_Time;
		static bool		m_Inited;

#ifdef _WIN32
        	static LARGE_INTEGER	m_TimeVal;
        	static LARGE_INTEGER	m_Freq;
#else
	        static struct timeval	m_TimeVal;
        	static struct timezone	m_TimeZone;
#endif
	};
	
}


#endif
