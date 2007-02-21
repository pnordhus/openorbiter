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


#include "ui_form_main.h"
#include "ui_form_creatematch.h"


#include "form_creatematch.h"
#include "form_selectmap.h"
#include "map.h"
#include "openorbiter.h"
#include "player.h"


FormCreateMatch::FormCreateMatch(QWidget* parent) :
	QDialog(parent)
{
	m_playerListModel.insertColumn(0);

	m_window = new Ui::FormCreateMatch;
	m_window->setupUi(this);

	connect(m_window->buttonMapSelect, SIGNAL(clicked()), this, SLOT(selectMap()));

	int current = 0;
	int x = 0;
	foreach (const Map* map, g_openorbiter->getMaps()) {
		m_window->comboMap->addItem(map->name());
		if (map == g_openorbiter->lastMap())
			current = x;
		x++;
	}

	m_window->comboMap->setCurrentIndex(current);

	m_playerListModel.insertRows(0, OpenOrbiter::MaxPlayers);

	for (unsigned int i = 0; i < OpenOrbiter::MaxPlayers; i++) {
		const Player& player = g_openorbiter->getPlayer(i);
		QModelIndex index = m_playerListModel.index(i, 0);
		m_playerListModel.setData(index, QVariant(player.getName()), Qt::DisplayRole);

		if (player.getSelected())
			m_playerListModel.setData(index, QVariant(Qt::Checked), Qt::CheckStateRole);
		else
			m_playerListModel.setData(index, QVariant(Qt::Unchecked), Qt::CheckStateRole);

		QPixmap p(10, 10);
		p.fill(player.getColor());

		m_playerListModel.setData(index, p, Qt::DecorationRole);
	}

	m_window->listPlayers->setModel(&m_playerListModel);
}


FormCreateMatch::~FormCreateMatch()
{
	delete m_window;
}


int FormCreateMatch::getMap() const
{
	return m_window->comboMap->currentIndex();
}


void FormCreateMatch::accept()
{
	int count = 0;
	for (int i = 0; i < m_playerListModel.rowCount(); i++) {
		QModelIndex index = m_playerListModel.index(i, 0);
		if (index.data(Qt::CheckStateRole) == Qt::Checked)
			count++;
	}

	if (count < 2) {
		QMessageBox::warning(this, tr("Too few players"), tr("You have to select at least two players."));
		return;
	}

	if (count > g_openorbiter->getMap(m_window->comboMap->currentIndex())->spawnPoints().size()) {
		QMessageBox::warning(this, tr("Too many players"), tr("You have to select more players than this map supports."));
		return;
	}

	// update names
	for (int i = 0; i < m_playerListModel.rowCount(); i++) {
		QModelIndex index = m_playerListModel.index(i, 0);
		Player& player = g_openorbiter->getPlayer(i);
		player.setName(index.data(Qt::DisplayRole).toString());
		player.setSelected(index.data(Qt::CheckStateRole) == Qt::Checked);
	}

	QDialog::accept(); // call parent accept to close dialog
}


void FormCreateMatch::selectMap()
{
	FormSelectMap form(this, m_window->comboMap->currentIndex());

	if (form.exec()) {
		m_window->comboMap->setCurrentIndex(form.getMap());
	}
}
