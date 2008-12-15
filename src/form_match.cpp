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


#include "form_match.h"
#include "map.h"
#include "mapdef.h"
#include "scene.h"
#include "ui_form_match.h"
#include <QMessageBox>
#include <QPainter>
#include <QSettings>


FormMatch::FormMatch(QWidget* parent) :
	QDialog(parent),
	m_map(NULL)
{
	m_ui = new Ui::FormMatch;
	m_ui->setupUi(this);
	m_ui->boxConditions->hide();
	
	connect(m_ui->btnStart,			SIGNAL(clicked()),		SLOT(start()));
	connect(m_ui->btnAdd,			SIGNAL(clicked()),		SLOT(addPlayer()));
	connect(m_ui->btnRemove,		SIGNAL(clicked()),		SLOT(removePlayer()));
	connect(m_ui->btnSelectAll,		SIGNAL(clicked()),		SLOT(selectAllMaps()));
	connect(m_ui->btnSelectNone,	SIGNAL(clicked()),		SLOT(deselectAllMaps()));
	connect(m_ui->comboDifficulty,	SIGNAL(currentIndexChanged(int)),		SLOT(difficultyChanged(int)));
	connect(&m_modelMaps,			SIGNAL(itemChanged(QStandardItem*)),	SLOT(mapChanged(QStandardItem*)));
	
	m_scene = new Scene;
	m_ui->view->setScene(m_scene);
	connect(m_scene, SIGNAL(sizeChanged()), m_ui->view, SLOT(sizeChanged()));
	
	m_ui->treePlayers->setModel(&m_modelPlayers);
	m_ui->treeMaps->setModel(&m_modelMaps);
	
	m_modelPlayers.setHorizontalHeaderLabels(QStringList() << tr("Name"));
	m_modelMaps.setHorizontalHeaderLabels(QStringList() << tr("Name"));
	
	m_colors.append(Qt::red);
	m_colors.append(Qt::green);
	m_colors.append(Qt::blue);
	m_colors.append(Qt::yellow);
	m_colors.append(Qt::cyan);
	m_colors.append(Qt::magenta);
	m_colors.append(Qt::lightGray);
	m_colors.append(Qt::darkGray);
	
	foreach (const QColor& color, m_colors) {
		QPixmap pixmap(16, 16);
		
		{
			QPainter painter(&pixmap);
			painter.fillRect(0, 0, 16, 16, color);
		}
		
		m_icons.append(QIcon(pixmap));
	}
	
	{
		QSettings s;
		{
			bool b;
			uint scale = s.value("mapscale").toUInt(&b);
			if (!b)
				scale = 50;
			m_ui->spinMapScale->setValue(scale);
		}
		
		QStringList players = s.value("players").toStringList();
		foreach (const QString& name, players)
			addPlayer(name);
		
		m_ui->comboDifficulty->setCurrentIndex(s.value("difficulty").toInt());
	}
	
	connect(m_ui->treePlayers->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		SLOT(playersSelectionChanged()));
	
	connect(m_ui->treeMaps->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		SLOT(mapsSelectionChanged(const QItemSelection&)));
}


FormMatch::~FormMatch()
{
	delete m_map;
	delete m_scene;
	delete m_ui;
}


void FormMatch::setMaps(const QList<MapDef>& maps)
{
	m_modelMaps.removeRows(0, m_modelMaps.rowCount());
	
	QSettings s;
	QStringList mapNames = s.value("maps").toStringList();
	
	foreach (const MapDef& map, maps) {
		QStandardItem* itemName = new QStandardItem(map.nameTranslated());
		itemName->setData(QVariant::fromValue((void*) &map));
		itemName->setCheckable(true);
		if (mapNames.contains(map.name())) {
			itemName->setCheckState(Qt::Checked);
			mapChanged(itemName);
		} else {
			itemName->setCheckState(Qt::Unchecked);
		}
		
		m_modelMaps.appendRow(QList<QStandardItem*>() << itemName);
	}
	
	m_ui->treeMaps->sortByColumn(0, Qt::AscendingOrder);
	difficultyChanged(m_ui->comboDifficulty->currentIndex());
}


void FormMatch::start()
{
	if (m_modelPlayers.rowCount() < 2) {
		QMessageBox::warning(this, "OpenOrbiter", tr("You have to add at least 2 players!"));
		return;
	}
	
	if (m_selectedMaps.size() < 1) {
		QMessageBox::warning(this, "OpenOrbiter", tr("You have to select at least 1 map!"));
		return;
	}
	
	QStringList names;
	for (int i = 0; i < m_modelPlayers.rowCount(); i++) {
		const QString name = m_modelPlayers.item(i)->text();
		
		if (name.isEmpty()) {
			QMessageBox::warning(this, "OpenOrbiter", tr("Please give each player a name!"));
			return;
		}
		
		if (names.contains(name)) {
			QMessageBox::warning(this, "OpenOrbiter", tr("There are at least two players named '%1'!\nPlease make sure all player names are unique.").arg(name));
			return;
		}
		
		names << name;
	}
	
	accept();
}


void FormMatch::save()
{
	QStringList players;
	for (int i = 0; i < m_modelPlayers.rowCount(); i++)
		players << m_modelPlayers.item(i)->text();
	
	QStringList maps;
	for (int i = 0; i < m_modelMaps.rowCount(); i++) {
		QStandardItem* item = m_modelMaps.item(i, 0);
		if (item->checkState() == Qt::Checked)
			maps << static_cast<const MapDef*>(item->data().value<void*>())->name();
	}
	
	QSettings s;
	s.setValue("mapscale", m_ui->spinMapScale->value());
	s.setValue("players", players);
	s.setValue("maps", maps);
	s.setValue("difficulty", m_ui->comboDifficulty->currentIndex());
}


void FormMatch::addPlayer(QString name)
{
	Q_ASSERT(m_modelPlayers.rowCount() < 8);
	
	if (name.isEmpty()) {
		for (int i = 0; i < 8; i++) {
			name = tr("Player %1").arg(i + 1);
			bool found = false;
			for (int j = 0; j < m_modelPlayers.rowCount(); j++) {
				if (m_modelPlayers.item(j)->text() == name) {
					found = true;
					break;
				}
			}
			if (!found)
				break;
		}
	}
	
	QStandardItem* itemName = new QStandardItem(name);
	itemName->setIcon(m_icons[m_modelPlayers.rowCount()]);
	m_modelPlayers.appendRow(QList<QStandardItem*>() << itemName);
	
	if (m_modelPlayers.rowCount() == 8)
		m_ui->btnAdd->setEnabled(false);
}


void FormMatch::removePlayer()
{
	QItemSelectionModel* selection = m_ui->treePlayers->selectionModel();
	for (int i = 0; i < m_modelPlayers.rowCount(); i++) {
		if (selection->isRowSelected(i, QModelIndex()))
			m_modelPlayers.removeRow(i);
	}
	
	for (int i = 0; i < m_modelPlayers.rowCount(); i++) {
		m_modelPlayers.item(i)->setIcon(m_icons[i]);
	}
	
	if (m_modelPlayers.rowCount() < 8)
		m_ui->btnAdd->setEnabled(true);
}


void FormMatch::playersSelectionChanged()
{
	m_ui->btnRemove->setEnabled(m_ui->treePlayers->selectionModel()->hasSelection());
}


void FormMatch::mapsSelectionChanged(const QItemSelection& sel)
{
	delete m_map;
	m_map = NULL;
	
	QModelIndexList list = sel.indexes();
	if (!list.isEmpty()) {
		QStandardItem* item = m_modelMaps.itemFromIndex(list.first());
		const MapDef* def = static_cast<const MapDef*>(item->data().value<void*>());
		m_map = new Map(*def, *m_scene);
		m_ui->lblMapName->setText(def->nameTranslated());
		m_ui->lblMapAuthor->setText(def->author());
		m_ui->lblMapDifficulty->setText(def->difficultyString());
		m_ui->lblMapNodes->setText(QString::number(def->nodes().size()));
	}
}


QList<Player> FormMatch::players() const
{
	QList<Player> players;
	
	for (int i = 0; i < m_modelPlayers.rowCount(); i++)
		players.append(Player(m_modelPlayers.item(i)->text(), m_colors[i]));
	
	return players;
}


QList<MapDef> FormMatch::maps() const
{
	QList<MapDef> maps;
	
	foreach (const MapDef* map, m_selectedMaps) {
		maps.append(*map);
		maps.last().setScale(float(m_ui->spinMapScale->value()) / 100.0f);
	}
	
	return maps;
}


void FormMatch::mapChanged(QStandardItem* item)
{
	const MapDef* map = static_cast<const MapDef*>(item->data().value<void*>());
	if ((item->checkState() == Qt::Checked) && (!m_ui->treeMaps->isRowHidden(item->row(), QModelIndex())))
		m_selectedMaps.insert(map);
	else
		m_selectedMaps.remove(map);
}


void FormMatch::selectAllMaps()
{
	for (int i = 0; i < m_modelMaps.rowCount(); i++)
		m_modelMaps.item(i)->setCheckState(Qt::Checked);
}


void FormMatch::deselectAllMaps()
{
	for (int i = 0; i < m_modelMaps.rowCount(); i++)
		m_modelMaps.item(i)->setCheckState(Qt::Unchecked);
}


void FormMatch::difficultyChanged(int diff)
{
	for (int i = 0; i < m_modelMaps.rowCount(); i++) {
		QStandardItem* item = m_modelMaps.item(i, 0);
		const MapDef* map = static_cast<const MapDef*>(item->data().value<void*>());
		if ((diff == 0) || (map->difficulty() == MapDef::Difficulty(diff - 1))) {
			m_ui->treeMaps->setRowHidden(i, QModelIndex(), false);
		} else {
			m_ui->treeMaps->setRowHidden(i, QModelIndex(), true);
		}
		mapChanged(item);
	}
}
