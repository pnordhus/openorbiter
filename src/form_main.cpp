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


#include "form_main.h"
#include "form_match.h"
#include "mapdef.h"
#include "maploader.h"
#include "match.h"
#include "orbiter.h"
#include "rendermanager.h"
#include "ui_form_main.h"
#include <QDir>
#include <QSettings>


FormMain::FormMain() :
	m_match(NULL)
{
	m_ui = new Ui::FormMain;
	m_ui->setupUi(this);
	
	setWindowTitle("OpenOrbiter 0.4-pre");
	
	RenderManager::create();
	
	m_ui->view->setScene(&m_scene);
	connect(&m_scene, SIGNAL(sizeChanged()), m_ui->view, SLOT(sizeChanged()));
	
	connect(m_ui->actionNewMatch,		SIGNAL(triggered()),				SLOT(newMatch()));
	connect(m_ui->actionAntialiasing,	SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableAntiAliasing(bool)));
	connect(m_ui->actionAboutQt,		SIGNAL(triggered()),	qApp,		SLOT(aboutQt()));
	
#ifdef QT_OPENGL_LIB
	QAction* actionOpenGL = m_ui->menuView->addAction("Enable OpenGL");
	actionOpenGL->setCheckable(true);
	connect(actionOpenGL,				SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableGL(bool)));
#endif
	
	QSettings s;
	restoreGeometry(s.value("geometry", QSize(400, 400)).toByteArray());
	
	QDir dir(OO_DATADIR "/maps");
	QStringList files = dir.entryList(QStringList() << "*.xml", QDir::Files | QDir::Readable);
	foreach (const QString& file, files) {
		MapLoader loader;
		if (loader.loadMap(dir.absoluteFilePath(file)))
			m_maps.append(loader.map());
	}
}


FormMain::~FormMain()
{
	delete m_match;
	
	RenderManager::destroy();
	delete m_ui;
}


void FormMain::closeEvent(QCloseEvent* e)
{
	QSettings s;
	s.setValue("geometry", saveGeometry());
	QMainWindow::closeEvent(e);
}


void FormMain::show()
{
	QMainWindow::show();
	newMatch();
}


void FormMain::newMatch()
{
	FormMatch form(this);
	form.setMaps(m_maps);
	if (form.exec() == QDialog::Accepted) {
		form.save();
		
		delete m_match;
		m_match = new Match(m_scene);
		connect(m_ui->view, SIGNAL(keyPressed(int)),	m_match,	SLOT(keyPressed(int)));
		connect(m_ui->view, SIGNAL(pause()),			m_match,	SLOT(pause()));
		
		m_match->setPlayers(form.players());
		m_match->setMaps(form.maps());
		
		m_match->start();
	}
}
