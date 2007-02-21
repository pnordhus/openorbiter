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


#include "ui_form_selectkey.h"


#include "form_selectkey.h"
#include "openorbiter.h"
#include "player.h"


FormSelectKey::FormSelectKey(QWidget* parent) :
	QDialog(parent)
{
	m_window = new Ui::FormSelectKey;
	m_window->setupUi(this);

	m_players = g_openorbiter->selectedPlayers();

	m_currentPlayer = -1;
	nextPlayer();

	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(nextPlayer()));
}


FormSelectKey::~FormSelectKey()
{
	delete m_window;
}


void FormSelectKey::nextPlayer()
{
	m_currentPlayer++;
	if (m_currentPlayer == m_players.size()) {
		for (int i = 0; i < m_players.size(); i++) {
			m_players.value(i)->setKey(m_keys.value(i));
		}
		accept();
		return;
	}

	m_window->labelName->setText(m_players.value(m_currentPlayer)->getName());
	m_window->labelText->setText(tr("Please press your key"));
}


void FormSelectKey::keyPressEvent(QKeyEvent* k)
{
	if (m_timer.isActive()) {
		m_timer.stop();
		nextPlayer();
	}

	int key = k->key();

	foreach (int oldKey, m_keys) {
		if (oldKey == key) {
			m_window->labelText->setText(tr("Invalid key! Someone is using this key already."));
			return;
		}
	}

	if ((k->modifiers() == Qt::NoModifier) && (key != Qt::Key_Space)) {
		m_keys.append(key);
		m_window->labelText->setText(tr("Key set to: ") + QKeySequence(key).toString());
		m_timer.start(1000);
	} else {
		m_window->labelText->setText(tr("Invalid key! No modifier keys allowed."));
	}
}


void FormSelectKey::timerEvent(QTimerEvent*)
{

}


void FormSelectKey::showEvent(QShowEvent*)
{
	grabKeyboard();
}


void FormSelectKey::hideEvent(QShowEvent*)
{
	releaseKeyboard();
}
