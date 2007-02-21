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


#ifndef OPENORBITER_FORM_SELECTKEY_H
#define OPENORBITER_FORM_SELECTKEY_H


namespace Ui {
	class FormSelectKey;
}


class Player;


class FormSelectKey : public QDialog
{
	Q_OBJECT

public:
	FormSelectKey(QWidget* parent);
	~FormSelectKey();

private slots:
	void	nextPlayer();

private:
	void	keyPressEvent(QKeyEvent*);
	void	timerEvent(QTimerEvent*);
	void	showEvent(QShowEvent*);
	void	hideEvent(QShowEvent*);

private:
	Ui::FormSelectKey*	m_window;
	QList<Player*>		m_players;
	QList<int>			m_keys;
	int					m_currentPlayer;
	QTimer				m_timer;
};


#endif
