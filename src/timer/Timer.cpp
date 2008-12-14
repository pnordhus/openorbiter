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


#include "Timer.h"


using namespace Mercury;


double		Timer::m_Time = 0.0;
const double&	Timer::frameTime = Timer::m_Time;
bool		Timer::m_Inited = false;


#ifdef _WIN32


LARGE_INTEGER	Timer::m_Freq;
LARGE_INTEGER	Timer::m_TimeVal;


inline void Timer::init()
{
        QueryPerformanceFrequency(&m_Freq);
        QueryPerformanceCounter(&m_TimeVal);
	m_Inited = true;
}


inline void Timer::updateTime()
{
	QueryPerformanceCounter(&m_TimeVal);
}


inline double Timer::getTime()
{
	return (double) m_TimeVal.QuadPart / (double) m_Freq.QuadPart;
}


inline void Timer::sleep(UInt msecs)
{
	Sleep(msecs);
}


#else


struct timeval	Timer::m_TimeVal;
struct timezone	Timer::m_TimeZone;


inline void Timer::init()
{
        updateTime();
	m_Inited = true;
}


inline void Timer::updateTime()
{
        gettimeofday(&m_TimeVal, &m_TimeZone);
}


inline double Timer::getTime()
{
	return (double) m_TimeVal.tv_sec + (double) m_TimeVal.tv_usec / (1000000.0);
}


inline void Timer::sleep(UInt msecs)
{
	usleep(msecs * 1000);
}


#endif


void Timer::update(double max_fps)
{
	if (!m_Inited)
		init();

        double tmp1, tmp2;

        tmp1 = getTime();
	updateTime();
        tmp2 = getTime();
        
	m_Time = (double) (tmp2 - tmp1);
	
	double frtime = (1.0 / max_fps) - m_Time;
	int rtime = (int) (frtime * 1000.0);
	
	if (rtime <= 0)
                return;

	sleep(rtime);
	
	updateTime();
        tmp2 = getTime();
        
	m_Time = (double) (tmp2 - tmp1);
}
