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


#ifndef OPENORBITER_DEFS_H
#define OPENORBITER_DEFS_H


const quint8	APP_VERSION_MAJOR = 0;
const quint8	APP_VERSION_MINOR = 3;
const quint8	APP_VERSION_MICRO = 0;

/* set this to true for final release builds */
const bool		APP_VERSION_FINAL = false;


/****************************************************************************/


const QString APP_BUILD_TIME = QString(__DATE__) + " (" + __TIME__ + ")";

const QString APP_NAME = "OpenOrbiter";
const QString APP_NAME_VERSION_SHORT = QString("%1 %2.%3.%4%5")
	.arg(APP_NAME)
	.arg(APP_VERSION_MAJOR)
	.arg(APP_VERSION_MINOR)
	.arg(APP_VERSION_MICRO)
	.arg(APP_VERSION_FINAL ? "" : "-pre");
const QString APP_NAME_VERSION_BUILD = APP_NAME_VERSION_SHORT + " - " + APP_BUILD_TIME;

#ifndef QT_NO_DEBUG
const QString APP_NAME_VERSION = APP_NAME_VERSION_BUILD;
#else
const QString APP_NAME_VERSION = APP_NAME_VERSION_SHORT;
#endif


#endif // OPENORBITER_DEFS_H
