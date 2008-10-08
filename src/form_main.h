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


#ifndef FORM_MAIN_H
#define FORM_MAIN_H


#include "scene.h"
#include <QMainWindow>


class MapDef;
class Match;
namespace Ui { class FormMain; }


class FormMain : public QMainWindow
{
	Q_OBJECT

public:
	FormMain();
	~FormMain();

public:
	void	show();

private slots:
	void	newMatch();
	void	showAbout();

protected:
	void	closeEvent(QCloseEvent*);

private:
	Ui::FormMain*	m_ui;
	
	Scene			m_scene;
	Match*			m_match;
	bool			m_firstShow;
	
	QList<MapDef>	m_maps;

#ifdef QT_SVG_LIB
	QAction*		m_actionSvg;
#endif
#ifdef QT_OPENGL_LIB
	QAction*		m_actionOpenGL;
#endif
};


#endif // FORM_MAIN_H
