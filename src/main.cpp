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


#include "config.h"
#include "defs.h"
#include "form_main.h"
#include "openorbiter.h"


#include <QApplication>
#include <QMessageBox>
#include <QDebug>


/****************************************************************************/


static void printVersion()
{
	qWarning().nospace() << APP_NAME_VERSION_BUILD;
}


/****************************************************************************/


static void printHelp(char* arg)
{
	qWarning() << "Usage:" << arg << "[OPTION]...";
	qWarning();
	qWarning() << "Option:";
	qWarning() << "  --reset     do not load config files";
	qWarning();
	qWarning() << "  --help      print this help and exit";
	qWarning() << "  --version   output version information and exit";
}


/****************************************************************************/


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

	qsrand(time(NULL));

	QApplication app(argc, argv);


	Config::create();
	OpenOrbiter::create();

	g_openorbiter->init(loadConfig);

	if (g_openorbiter->getMaps().size() == 0) {
		QMessageBox::critical(NULL, "No maps found", "Could not find any maps, this won't work, sorry!");
		OpenOrbiter::destroy();
		return EXIT_FAILURE;
	}

	QRect g = g_config->getRect("windowGeometry");

	FormMain form(g_config->getBool("windowShowStats"));
	form.resize(g.width(), g.height());
	
	if (g_config->getBool("windowMaximized"))
		form.setWindowState(form.windowState() | Qt::WindowMaximized);
		
	if (g_config->getBool("windowFullScreen"))
		form.setWindowState(form.windowState() | Qt::WindowFullScreen);

	form.setStatsShown(g_config->statsShown());

	form.show();

	// this causese some troubles with fullscreen
	//form.createMatch();

	ret = app.exec();

	g_config->setStatsShown(form.statsShown());

	g_config->set("windowMaximized",	form.isMaximized());
	g_config->set("windowFullScreen",	form.isFullScreen());
	g_config->set("windowShowStats",	form.isStatsShown());

	g_config->set("windowGeometry", form.normalGeometry());

	OpenOrbiter::destroy();
	Config::destroy();

	return ret;
}
