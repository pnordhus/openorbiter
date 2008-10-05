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
#include "map.h"
#include "match.h"
#include "orbiter.h"
#include "ui_form_main.h"
#include <QSettings>


FormMain::FormMain() :
	m_match(NULL)
{
	m_ui = new Ui::FormMain;
	m_ui->setupUi(this);
	
	m_ui->view->setScene(&m_scene);
	
	connect(m_ui->actionNewMatch,		SIGNAL(triggered()),				SLOT(newMatch()));
	connect(m_ui->actionAntialiasing,	SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableAntiAliasing(bool)));
	connect(m_ui->actionOpenGL,			SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableGL(bool)));
	connect(m_ui->actionAboutQt,		SIGNAL(triggered()),	qApp,		SLOT(aboutQt()));
	
	QSettings s;
	restoreGeometry(s.value("geometry", QSize(400, 400)).toByteArray());
	
	{
		Map map("One Node to rule them all", 40.0f, 30.0f);
		
		map.addNode(Vector(20, 15));
		
		map.addSpawn(Vector(10, 2));
		map.addSpawn(Vector(15, 2));
		map.addSpawn(Vector(25, 2));
		map.addSpawn(Vector(30, 2));
		
		map.addSpawn(Vector(12.5, 4));
		map.addSpawn(Vector(17.5, 4));
		map.addSpawn(Vector(22.5, 4));
		map.addSpawn(Vector(27.5, 4));
		
		m_maps.append(map);
	}
	
	{
		Map map("Bermuda Triangle", 40.0f, 30.0f);
		
		map.addNode(Vector(20, 10));
		map.addNode(Vector(14.33, 20));
		map.addNode(Vector(25.77, 20));
		
		map.addSpawn(Vector(10, 2));
		map.addSpawn(Vector(15, 2));
		map.addSpawn(Vector(25, 2));
		map.addSpawn(Vector(30, 2));
		
		map.addSpawn(Vector(12.5, 4));
		map.addSpawn(Vector(17.5, 4));
		map.addSpawn(Vector(22.5, 4));
		map.addSpawn(Vector(27.5, 4));
		
		m_maps.append(map);
	}
}


FormMain::~FormMain()
{
	delete m_match;
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
		connect(m_ui->view, SIGNAL(resume()),			m_match,	SLOT(resume()));
		
		m_match->setPlayers(form.players());
		m_match->setMaps(form.maps());
		
		m_match->start();
	}
}
