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


#include "defs.h"
#include "form_main.h"
#include "form_match.h"
#include "mapdef.h"
#include "maploader.h"
#include "match.h"
#include "orbiter.h"
#include "rendermanager.h"
#include "ui_form_about.h"
#include "ui_form_main.h"
#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QTranslator>


FormMain::FormMain() :
	m_translator(NULL),
	m_match(NULL)
{
	m_ui = new Ui::FormMain;
	m_ui->setupUi(this);
	
	setWindowIcon(QIcon(":/orbiter-64.png"));
	
	{
		// language setup
		m_grpLanguages = new QActionGroup(this);
		m_grpLanguages->addAction(m_ui->actionLangEnglish);
		connect(m_ui->actionLangEnglish, SIGNAL(triggered()), SLOT(langChanged()));
		
		QSettings s;
		QString lang;
		if (s.contains("language"))
			lang = s.value("language").toString();
		else
			lang = QLocale::system().name().left(2);
		
		loadLanguage("de", "Deutsch", lang);
		
		if (m_translators.isEmpty())
			m_ui->menubar->removeAction(m_ui->menuLanguage->menuAction());
	}
	
	RenderManager::create();
	
	m_ui->view->setScene(&m_scene);
	connect(&m_scene, SIGNAL(sizeChanged()), m_ui->view, SLOT(sizeChanged()));
	
	connect(m_ui->actionNewMatch,		SIGNAL(triggered()),				SLOT(newMatch()));
	connect(m_ui->actionAntialiasing,	SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableAntiAliasing(bool)));
	connect(m_ui->actionAboutQt,		SIGNAL(triggered()),	qApp,		SLOT(aboutQt()));
	connect(m_ui->actionAbout,			SIGNAL(triggered()),				SLOT(showAbout()));
	
	QSettings s;
	m_ui->actionAntialiasing->setChecked(s.value("antialiasing", false).toBool());
	m_ui->actionAntialiasing->setWhatsThis(tr("Enable anti-aliasing. Improves quality."));
	m_ui->actionAntialiasing->setStatusTip(m_ui->actionAntialiasing->whatsThis());
	
#ifdef QT_SVG_LIB
	m_actionSvg = m_ui->menuView->addAction("");
	m_actionSvg->setText(tr("Pretty objects"));
	m_actionSvg->setWhatsThis(tr("Enable rendering of Scalable Vector Graphics (SVG). Improves quality."));
	m_actionSvg->setStatusTip(m_actionSvg->whatsThis());
	m_actionSvg->setCheckable(true);
	connect(m_actionSvg,				SIGNAL(toggled(bool)),	&RenderManager::get(), SLOT(enableSvg(bool)));
	m_actionSvg->setChecked(s.value("svg", true).toBool());
#endif
	
#ifdef QT_OPENGL_LIB
	m_actionOpenGL = m_ui->menuView->addAction("");
	m_actionOpenGL->setText(tr("Hardware acceleration"));
	m_actionOpenGL->setWhatsThis(tr("Enable OpenGL hardware acceleration. May improve speed."));
	m_actionOpenGL->setStatusTip(m_actionOpenGL->whatsThis());
	m_actionOpenGL->setCheckable(true);
	connect(m_actionOpenGL,				SIGNAL(toggled(bool)),	m_ui->view, SLOT(enableGL(bool)));
	m_actionOpenGL->setChecked(s.value("opengl", false).toBool());
#endif
	
	restoreGeometry(s.value("geometry", QSize(400, 400)).toByteArray());
	
	QDir dir(OO_DATADIR "/maps");
	QStringList files = dir.entryList(QStringList() << "*.xml", QDir::Files | QDir::Readable);
	foreach (const QString& file, files) {
		MapLoader loader;
		if (loader.loadMap(dir.absoluteFilePath(file)))
			m_maps.append(loader.map());
	}
	
	retranslate();
}


FormMain::~FormMain()
{
	delete m_match;
	
	RenderManager::destroy();
	delete m_ui;
}


void FormMain::loadLanguage(const QString& name, const QString& fullname, const QString& defaultLang)
{
	QTranslator* translator = new QTranslator(this);
	if (!translator->load(QString(OO_DATADIR "/translations/openorbiter_") + name)) {
		delete translator;
		return;
	}
	
	m_translators.insert(name, translator);
	
	QAction* action = m_ui->menuLanguage->addAction(fullname);
	m_grpLanguages->addAction(action);
	action->setCheckable(true);
	if (name == defaultLang) {
		Q_ASSERT(!m_translator);
		action->setChecked(true);
		m_translator = translator;
		qApp->installTranslator(translator);
		QSettings s;
		s.setValue("language", name);
	}
	
	action->setProperty("lang", name);
	connect(action, SIGNAL(triggered()), SLOT(langChanged()));
}


void FormMain::langChanged()
{
	if (m_translator) {
		qApp->removeTranslator(m_translator);
		m_translator = NULL;
	}
	
	QSettings s;
	if (sender() == m_ui->actionLangEnglish) {
		s.setValue("language", "en");
		return;
	}
	
	QString lang = sender()->property("lang").toString();
	QTranslator* translator = m_translators.value(lang);
	if (translator) {
		m_translator = translator;
		qApp->installTranslator(translator);
		s.setValue("language", lang);
	}
}


void FormMain::retranslate()
{
	setWindowTitle(OPENORBITER_VERSION_STRING);
	
#ifdef QT_SVG_LIB
	m_actionSvg->setText(tr("Pretty objects"));
	m_actionSvg->setWhatsThis(tr("Enable rendering of Scalable Vector Graphics (SVG). Improves quality."));
	m_actionSvg->setStatusTip(m_actionSvg->whatsThis());
#endif
	
#ifdef QT_OPENGL_LIB
	m_actionOpenGL->setText(tr("Hardware acceleration"));
	m_actionOpenGL->setWhatsThis(tr("Enable OpenGL hardware acceleration. May improve speed."));
	m_actionOpenGL->setStatusTip(m_actionOpenGL->whatsThis());
#endif
}


bool FormMain::event(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange) {
		m_ui->retranslateUi(this);
		retranslate();
	}
	
	return QMainWindow::event(e);
}


void FormMain::closeEvent(QCloseEvent* e)
{
	QSettings s;
	s.setValue("geometry", saveGeometry());
#ifdef QT_SVG_LIB
	s.setValue("svg", m_actionSvg->isChecked());
#endif
#ifdef QT_OPENGL_LIB
	s.setValue("opengl", m_actionOpenGL->isChecked());
#endif
	s.setValue("antialiasing", m_ui->actionAntialiasing->isChecked());
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
		
		m_match->setPlayers(form.players());
		m_match->setMaps(form.maps());
		
		m_match->start();
	}
}


void FormMain::showAbout()
{
	QDialog dlg(this);
	Ui::FormAbout about;
	about.setupUi(&dlg);
	about.lblVersion->setText(OPENORBITER_VERSION_STRING);
	about.lblBuildTime->setText(QString("Build date: %1 %2").arg(__DATE__).arg(__TIME__));
	dlg.exec();
}
