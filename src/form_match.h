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


#ifndef FORM_MATCH_H
#define FORM_MATCH_H


#include "map.h"
#include "player.h"
#include <QDialog>
#include <QSet>
#include <QStandardItemModel>


namespace Ui { class FormMatch; }


class FormMatch : public QDialog
{
	Q_OBJECT

public:
	FormMatch(QWidget* parent);
	~FormMatch();

public:
	void			setMaps(const QList<Map>& maps);
	QList<Player>	players() const;
	QList<Map>		maps() const;

public slots:
	void			save();

private slots:
	void	start();
	void	addPlayer(QString name = QString());
	void	removePlayer();
	void	playersSelectionChanged();
	void	mapChanged(QStandardItem* item);

private:
	Ui::FormMatch*		m_ui;
	QStandardItemModel	m_modelPlayers;
	QStandardItemModel	m_modelMaps;
	
	QList<QColor>		m_colors;
	QList<QIcon>		m_icons;
	
	QSet<const Map*>	m_selectedMaps;
};


#endif // FORM_MATCH_H
