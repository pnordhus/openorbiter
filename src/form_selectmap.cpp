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


#include "form_selectmap.h"
#include "../build/ui_form_selectmap.h"
#include "openorbiter.h"


FormSelectMap::FormSelectMap(QWidget* parent, int currentMap) :
	QDialog(parent)
{
	m_window = new Ui::FormSelectMap;
	m_window->setupUi(this);

	foreach (const Map* map, g_openorbiter->getMaps()) {
		const QString& name = map->name();
		m_window->listMaps->addItem(name);
	}

	connect(m_window->listMaps, SIGNAL(currentRowChanged(int)), this, SLOT(mapSelected(int)));

	m_window->listMaps->setCurrentRow(currentMap);
}


FormSelectMap::~FormSelectMap()
{
	delete m_window;
}


int FormSelectMap::getMap() const
{
	return m_window->listMaps->currentRow();
}


void FormSelectMap::mapSelected(int index)
{
	const Map* map = g_openorbiter->getMap(index);
	
	m_window->labelName->setText(map->name());
	m_window->labelNodeCount->setText(QString::number(map->nodes().size()));
	m_window->labelSpawnPoints->setText(QString::number(map->spawnPoints().size()));

	const QString& auth = map->author();
	if (auth.isEmpty())
		m_window->labelAuthor->setText(tr("n/a"));
	else
		m_window->labelAuthor->setText(auth);

	const QString& desc = map->description();
	if (desc.isEmpty())
		m_window->textDescription->setHtml(tr("n/a"));
	else
		m_window->textDescription->setHtml(desc);
}
