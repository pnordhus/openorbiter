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


#include "form_settings.h"
#include "openorbiter.h"
#include "../build/ui_form_settings.h"
#include <QColorDialog>


FormSettings::FormSettings(QWidget* parent) :
	QDialog(parent)
{
	m_window = new Ui::FormSettings;
	m_window->setupUi(this);

	connect(m_window->buttonSelectColor, SIGNAL(clicked()), this, SLOT(showColorDialog()));

	setColor(g_openorbiter->config().mapColor());
	m_window->frameColor->setAutoFillBackground(true);
}


FormSettings::~FormSettings()
{
	delete m_window;
}


void FormSettings::setColor(const QColor& color)
{
	QPalette p = m_window->frameColor->palette();
	p.setColor(QPalette::Window, color);
	m_window->frameColor->setPalette(p);
}


void FormSettings::accept()
{
	g_openorbiter->config().setMapColor(m_window->frameColor->palette().color(QPalette::Window));
	QDialog::accept();
}


void FormSettings::showColorDialog()
{
	QColor ret = QColorDialog::getColor(m_window->frameColor->palette().color(QPalette::Window));
	if (ret.isValid())
		setColor(ret);
}
