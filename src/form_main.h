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


#ifndef OPENORBITER_FORM_MAIN_H
#define OPENORBITER_FORM_MAIN_H


#include "openorbiter.h"

#include <QMainWindow>
#include <QEvent>
#include <QTimer>
#include <QStandardItemModel>


namespace Ui {
	class FormMain;
}


class PlayerShortcut;


class FormMain : public QMainWindow
{
	Q_OBJECT

public:
	FormMain(bool showStats);
	~FormMain();

	void	updateMapFrame();
	void	updatePlayers();
	bool	statsShown() const;
	QRect	mapGeometry() const;

	void	setStatsVisibility(const QList<bool>& list);
	QList<bool>	getStatsVisibility() const;

private:
	void	setKeys();

public slots:
	void	createMatch();

private slots:
	void	resume();
	void	process();
	void	updateStats();
	void	input();
	void	toggleFullscreen();
	void	toggleStats(bool show);
	void	menuAboutToShow();

	void	changeVisibleStats();
	void	showAllStats();
	void	hideAllStats();

	void	statsContextMenu(const QPoint&);

	void	showPreferences();
	void	setMapColor();

private:
	void	updateStat(int row, int col, const QVariant& value, Qt::ItemDataRole role);
	void	resizeEvent(QResizeEvent*);

private:
	Ui::FormMain*	m_window;

	QTimer			m_processTimer;
	QTimer			m_updateTimer;

	QList<PlayerShortcut*>m_shortcuts;

	const Player*	m_lastWinner;
	QStandardItemModel	m_modelStats;
};


#endif
