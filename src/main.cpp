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


#include "form_main.h"
#include "openorbiter.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>


void printVersion()
{
	qWarning().nospace() << "OpenOrbiter 0.3 (" << __DATE__ << " " << __TIME__ << ")";
}


void printHelp(char* arg)
{
	qWarning() << "Usage:" << arg << "[OPTION]...";
	qWarning();
	qWarning() << "Option:";
	qWarning() << "  --reset     do not load config files";
	qWarning();
	qWarning() << "  --help      print this help and exit";
	qWarning() << "  --version   output version information and exit";
}


int main(int argc, char* argv[])
{
	bool loadConfig = true;
	int ret = EXIT_FAILURE;

	for (int i = 1; i < argc; i++) {
		if (QString(argv[i]) == "--reset") {
			loadConfig = false;
		} else if (QString(argv[i]) == "--help") {
			printHelp(argv[0]);
			return EXIT_SUCCESS;
		} else if (QString(argv[i]) == "--version") {
			printVersion();
			return EXIT_SUCCESS;
		} else {
			qWarning().nospace() << "Unknown option " << argv[i];
			qWarning();
			printHelp(argv[0]);
			return EXIT_FAILURE;
		}
	}

	srandom(time(NULL));

	OpenOrbiter::create();
	QApplication app(argc, argv);

	try {
		g_openorbiter->init(loadConfig);

		if (g_openorbiter->getMaps().size() == 0) {
			QMessageBox::critical(NULL, "No maps found", "Could not find any maps, this won't work, sorry!");
			OpenOrbiter::destroy();
			return EXIT_FAILURE;
		}

		int x, y, w, h;
		x = g_openorbiter->config().windowPosX();
		y = g_openorbiter->config().windowPosY();
		w = g_openorbiter->config().windowWidth();
		h = g_openorbiter->config().windowHeight();

		FormMain form(g_openorbiter->config().windowShowStats());
		form.resize(w, h);
		
		if (g_openorbiter->config().windowMaximized())
			form.setWindowState(form.windowState() | Qt::WindowMaximized);
			
		if (g_openorbiter->config().windowFullScreen())
			form.setWindowState(form.windowState() | Qt::WindowFullScreen);

		form.setStatsVisibility(g_openorbiter->config().statsVisibility());

		form.show();

		// this causese some troubles with fullscreen
		//form.createMatch();

		ret = app.exec();

		g_openorbiter->config().statsVisibility(form.getStatsVisibility());

		g_openorbiter->config().windowMaximized(form.isMaximized());
		g_openorbiter->config().windowFullScreen(form.isFullScreen());
		g_openorbiter->config().windowShowStats(form.statsShown());

		QRect geometry = form.normalGeometry();
		g_openorbiter->config().windowPosX(geometry.left());
		g_openorbiter->config().windowPosY(geometry.top());
		g_openorbiter->config().windowWidth(geometry.width());
		g_openorbiter->config().windowHeight(geometry.height());
	} catch (...) {
	
	}


	OpenOrbiter::destroy();

	return ret;
}
