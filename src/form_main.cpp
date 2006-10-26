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


#include "../build/ui_form_main.h"


#include "config.h"
#include "defs.h"
#include "game.h"
#include "form_creatematch.h"
#include "form_main.h"
#include "form_selectkey.h"
#include "form_settings.h"
#include "map.h"
#include "match.h"
#include "openorbiter.h"
#include "orbiter.h"
#include "player.h"


#include <cmath>
#include <QHeaderView>
#include <QShortcut>


#ifdef QT_MODULE_OPENGL
#  include <QGLWidget>
#endif


class PlayerShortcut : public QShortcut
{
public:
	PlayerShortcut(Player* player, QWidget* parent) : QShortcut(player->getKey(), parent) { Q_ASSERT(player); m_player = player; }

	Player*	getPlayer() { return m_player; }

private:
	Player*	m_player;
};


/****************************************************************************/


FormMain::FormMain(bool showStats) :
	QMainWindow(NULL),
	m_processTimer(this),
	m_updateTimer(this),
	m_lastWinner(NULL),
	m_modelStats(8, 0)
{
	m_window = new Ui::FormMain;

	m_window->setupUi(this);
	m_window->frameMapBack->setFormMain(this);

	connect(&m_processTimer, SIGNAL(timeout()), this, SLOT(process()));
	m_processTimer.start(20);

	connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateStats()));
	m_updateTimer.start(100);


	connect(m_window->buttonResume, SIGNAL(clicked()), this, SLOT(createMatch()));

	connect(m_window->menuGame, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));
	connect(m_window->menuSettings, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));
	connect(m_window->menuHelp, SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()));

	m_window->frameMap->setFocus(Qt::OtherFocusReason);

	m_modelStats.setHeaderData(0, Qt::Vertical, "Avg. Speed", Qt::DisplayRole);
	m_modelStats.setHeaderData(1, Qt::Vertical, "Frags", Qt::DisplayRole);
	m_modelStats.setHeaderData(2, Qt::Vertical, "Key", Qt::DisplayRole);
	m_modelStats.setHeaderData(3, Qt::Vertical, "Losses", Qt::DisplayRole);
	m_modelStats.setHeaderData(4, Qt::Vertical, "Play time", Qt::DisplayRole);
	m_modelStats.setHeaderData(5, Qt::Vertical, "Top speed", Qt::DisplayRole);
	m_modelStats.setHeaderData(6, Qt::Vertical, "Way", Qt::DisplayRole);
	m_modelStats.setHeaderData(7, Qt::Vertical, "Wins", Qt::DisplayRole);

	for (int i = 0; i < 7; i++) {
		QModelIndex index = m_modelStats.index(0, i);
		m_modelStats.setData(index, 0, Qt::DisplayRole);
	}

	m_window->tableStats->setModel(&m_modelStats);

	QHeaderView* header = m_window->tableStats->verticalHeader();
	for (int i = 0; i < header->length(); i++) {
		header->resizeSection(i, header->sectionSizeHint(i));
	}
	
	m_window->tableStats->horizontalHeader()->setClickable(false);
	m_window->tableStats->horizontalHeader()->setFocusPolicy(Qt::NoFocus);
    m_window->tableStats->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

	m_window->tableStats->verticalHeader()->setClickable(false);
	m_window->tableStats->verticalHeader()->setMovable(true);
	m_window->tableStats->verticalHeader()->setResizeMode(QHeaderView::Stretch);
	m_window->tableStats->verticalHeader()->setFocusPolicy(Qt::NoFocus);
    m_window->tableStats->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

	updatePlayers();

//	menuBar()->hide();
	connectActions();
	setShortcuts();
	m_window->actionShowStats->setChecked(showStats);

	connect(m_window->tableStats->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(statsContextMenu(const QPoint&)));
	connect(m_window->tableStats->verticalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(statsContextMenu(const QPoint&)));
	connect(m_window->tableStats, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(statsContextMenu(const QPoint&)));

	changeVisibleStats();
	updateStats();
	setMapColor();

	setWindowTitle(APP_NAME_VERSION);

#ifdef QT_MODULE_OPENGL
	qDebug() << "Using OpenGL";
	m_window->graphicsMap->setViewport(new QGLWidget);
#endif

	m_window->graphicsMap->setScene(g_openorbiter->graphicsScene());
}


/****************************************************************************/


FormMain::~FormMain()
{
	m_updateTimer.stop();
	m_processTimer.stop();

	foreach (QShortcut* sc, m_shortcuts)
		delete sc;
	delete m_window;
}


/****************************************************************************/


void FormMain::connectActions()
{
	connect(m_window->actionNewMatch,			SIGNAL(activated()),	this,	SLOT(createMatch()));
	connect(m_window->actionFullScreenMode,		SIGNAL(activated()),	this,	SLOT(toggleFullscreen()));
	connect(m_window->actionPreferences,		SIGNAL(activated()),	this,	SLOT(showPreferences()));
	connect(m_window->actionShowAll,			SIGNAL(activated()),	this,	SLOT(showAllStats()));
	connect(m_window->actionShowNone,			SIGNAL(activated()),	this,	SLOT(hideAllStats()));

	connect(m_window->actionShowAverageSpeed,	SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowFrags,			SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowKey,			SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowLosses,			SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowPlayTime,		SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowTopSpeed,		SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowWay,			SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
	connect(m_window->actionShowWins,			SIGNAL(activated()),	this,	SLOT(changeVisibleStats()));
}


/****************************************************************************/


void FormMain::setShortcuts()
{
	m_window->actionNewMatch->setShortcut(Qt::CTRL + Qt::Key_N);
	m_window->actionQuit->setShortcut(Qt::CTRL + Qt::Key_Q);
	m_window->actionFullScreenMode->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_F);

	m_window->actionShowStats->setShortcut(Qt::CTRL + Qt::Key_S);
	m_window->actionShowStats->setShortcutContext(Qt::ApplicationShortcut);
}


/****************************************************************************/


void FormMain::updateMapFrame()
{
	m_window->frameMap->recalcSize();
	m_window->frameMapBack->update();
}


/****************************************************************************/


void FormMain::createMatch()
{
	FormCreateMatch dialog(this);
	if (dialog.exec()) {
		updatePlayers();

		FormSelectKey keySelect(this);
		if (keySelect.exec()) {
			m_window->labelMapName->setText(g_openorbiter->getMap(dialog.getMap())->name());
			m_window->buttonResume->setText(QApplication::translate("FormMain", "Start", 0, QApplication::UnicodeUTF8));
			disconnect(m_window->buttonResume, SIGNAL(clicked()), this, SLOT(createMatch()));
			disconnect(m_window->buttonResume, SIGNAL(clicked()), this, SLOT(resume()));
			connect(m_window->buttonResume, SIGNAL(clicked()), this, SLOT(resume()));
			m_window->buttonResume->show();
			m_window->frameMap->unsetCursor();

			Match match(g_openorbiter->getMap(dialog.getMap()));
			g_openorbiter->startMatch(match);
//			m_window->graphicsMap->setMap(g_openorbiter->getMap(dialog.getMap()));
			setKeys();
			updateMapFrame();
		}
	}
}


/****************************************************************************/


void FormMain::updatePlayers()
{
	QList<Player*> players;
	for (int i = 0; i < OpenOrbiter::MaxPlayers; i++) {
		Player& p = g_openorbiter->getPlayer(i);
		if (p.getSelected())
			players.append(&p);
	}

	int num = m_modelStats.columnCount() - players.size();
	if (num < 0) {
		m_modelStats.insertColumns(0, -num);
	} else if (num > 0) {
		m_modelStats.removeColumns(0, num);
	}

	int x = 0;
	foreach (Player* player, players) {
		m_modelStats.setHeaderData(x, Qt::Horizontal, player->getName(), Qt::DisplayRole);
		++x;
	}

	m_window->tableStats->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}


/****************************************************************************/


void FormMain::resume()
{
	if (g_openorbiter->isRunning()) {
		m_window->frameMap->setCursor(Qt::BlankCursor);
		m_window->buttonResume->hide();
		m_window->buttonResume->setText(QApplication::translate("FormMain", "Resume", 0, QApplication::UnicodeUTF8));
		g_openorbiter->resume();
	}
}


/****************************************************************************/


void FormMain::process()
{

	//qDebug() << m_model->headerData(0, Qt::Vertical, Qt::SizeHintRole);

	if (!g_openorbiter->isRunning())
		return;

	g_openorbiter->process();

	if (g_openorbiter->isPaused())
		return;

//	m_window->graphicsMap->process();
	m_window->frameMap->process();
}


/****************************************************************************/


void FormMain::updateStat(int row, int col, const QVariant& value, Qt::ItemDataRole role)
{
	QModelIndex index = m_modelStats.index(row, col);
	if (m_modelStats.data(index, role) != value)
		m_modelStats.setData(index, value, role);
}


/****************************************************************************/


void FormMain::updateStats()
{
	int count = 0;
	foreach (Player* player, g_openorbiter->selectedPlayers())  {
		QColor color = player->getColor();
		color.setAlpha(75);

		for (int i = 0; i < 8; i++) {
			updateStat(i, count, Qt::AlignCenter, Qt::TextAlignmentRole);
			updateStat(i, count, color, Qt::BackgroundColorRole);
		}

		updateStat(0, count, QString::number(player->matchStats().speed(), 'f', 1), Qt::DisplayRole);
		updateStat(1, count, player->matchStats().frags(), Qt::DisplayRole);
		updateStat(2, count, player->getKey().toString(), Qt::DisplayRole);
		updateStat(3, count, player->matchStats().losses(), Qt::DisplayRole);
		updateStat(4, count, QString::number(player->matchStats().time(), 'f', 1), Qt::DisplayRole);
		updateStat(5, count, QString::number(player->matchStats().topSpeed(), 'f', 1), Qt::DisplayRole);
		updateStat(6, count, QString::number(player->matchStats().way(), 'f', 1), Qt::DisplayRole);
		updateStat(7, count, player->matchStats().wins(), Qt::DisplayRole);

		count++;
	}

	if (!g_openorbiter->isRunning()) {
		m_window->labelMatchTime->setText("0.0s");
		m_window->labelGameTime->setText("0.0s");
		return;
	}

	const Player* last = g_openorbiter->match()->lastWinner();
	if (last != m_lastWinner)	{
		m_lastWinner = last;
		m_window->frameMapBack->update();
	}

	m_window->labelMatchTime->setText(QString("%1s").arg(g_openorbiter->match()->time(), 0, 'f', 1));
	m_window->labelGameTime->setText(QString("%1s").arg(g_openorbiter->match()->game()->time(), 0, 'f', 1));

	if (!g_openorbiter->isPaused() && !isActiveWindow()) {
		g_openorbiter->pause();
		m_window->buttonResume->show();
		m_window->frameMap->unsetCursor();
	}
}


/****************************************************************************/


void FormMain::setKeys()
{
	foreach (QShortcut* sc, m_shortcuts)
		delete sc;

	m_shortcuts.clear();

	foreach (Player* player, g_openorbiter->selectedPlayers()) {
		PlayerShortcut* psc = new PlayerShortcut(player, this);
		connect(psc, SIGNAL(activated()), this, SLOT(input()));
		m_shortcuts.append(psc);
	}
}


/****************************************************************************/


void FormMain::input()
{
	PlayerShortcut* sc = dynamic_cast<PlayerShortcut*>(sender());
	if (!sc)
		return;

	if (g_openorbiter->isRunning() && !g_openorbiter->isPaused())
		sc->getPlayer()->toggle();
}


/****************************************************************************/


void FormMain::toggleFullscreen()
{
	setWindowState(windowState() ^ Qt::WindowFullScreen);
}


/****************************************************************************/


void FormMain::resizeEvent(QResizeEvent*)
{
	m_window->actionFullScreenMode->setChecked(isFullScreen());
}


/****************************************************************************/


bool FormMain::isStatsShown() const
{
	return m_window->actionShowStats->isChecked();
}


/****************************************************************************/


void FormMain::menuAboutToShow()
{
	if (g_openorbiter->isRunning() && !g_openorbiter->isPaused()) {
		g_openorbiter->pause();
		m_window->buttonResume->show();
		m_window->frameMap->unsetCursor();
	}
}


/****************************************************************************/


QRect FormMain::mapGeometry() const
{
	return m_window->frameMap->geometry();
}


/****************************************************************************/


void FormMain::changeVisibleStats()
{
	QHeaderView* header = m_window->tableStats->verticalHeader();
	header->setSectionHidden(0, !m_window->actionShowAverageSpeed->isChecked());
	header->setSectionHidden(1, !m_window->actionShowFrags->isChecked());
	header->setSectionHidden(2, !m_window->actionShowKey->isChecked());
	header->setSectionHidden(3, !m_window->actionShowLosses->isChecked());
	header->setSectionHidden(4, !m_window->actionShowPlayTime->isChecked());
	header->setSectionHidden(5, !m_window->actionShowTopSpeed->isChecked());
	header->setSectionHidden(6, !m_window->actionShowWay->isChecked());
	header->setSectionHidden(7, !m_window->actionShowWins->isChecked());

	int h = m_window->tableStats->frameSize().height() - m_window->tableStats->contentsRect().height();
	h += (header->count() - header->hiddenSectionCount() + 1) * header->sizeHint().height();
	m_window->tableStats->setFixedHeight(h);
}


/****************************************************************************/


void FormMain::showAllStats()
{
	m_window->actionShowAverageSpeed->setChecked(true);
	m_window->actionShowFrags->setChecked(true);
	m_window->actionShowKey->setChecked(true);
	m_window->actionShowLosses->setChecked(true);
	m_window->actionShowPlayTime->setChecked(true);
	m_window->actionShowTopSpeed->setChecked(true);
	m_window->actionShowWay->setChecked(true);
	m_window->actionShowWins->setChecked(true);

	changeVisibleStats();
}


/****************************************************************************/


void FormMain::hideAllStats()
{
	m_window->actionShowAverageSpeed->setChecked(false);
	m_window->actionShowFrags->setChecked(false);
	m_window->actionShowKey->setChecked(false);
	m_window->actionShowLosses->setChecked(false);
	m_window->actionShowPlayTime->setChecked(false);
	m_window->actionShowTopSpeed->setChecked(false);
	m_window->actionShowWay->setChecked(false);
	m_window->actionShowWins->setChecked(false);

	changeVisibleStats();
}


/****************************************************************************/


void FormMain::setStatsShown(const StringBoolMap& map)
{
	m_window->actionShowAverageSpeed->setChecked(map.value("avgSpeed"));
	m_window->actionShowFrags->setChecked(map.value("frags"));
	m_window->actionShowKey->setChecked(map.value("key"));
	m_window->actionShowLosses->setChecked(map.value("losses"));
	m_window->actionShowPlayTime->setChecked(map.value("playTime"));
	m_window->actionShowTopSpeed->setChecked(map.value("topSpeed"));
	m_window->actionShowWay->setChecked(map.value("way"));
	m_window->actionShowWins->setChecked(map.value("wins"));

	changeVisibleStats();
}


/****************************************************************************/


StringBoolMap FormMain::statsShown() const
{
	StringBoolMap ret;

	ret.insert("avgSpeed", m_window->actionShowAverageSpeed->isChecked());
	ret.insert("frags", m_window->actionShowFrags->isChecked());
	ret.insert("key", m_window->actionShowKey->isChecked());
	ret.insert("losses", m_window->actionShowLosses->isChecked());
	ret.insert("playTime", m_window->actionShowPlayTime->isChecked());
	ret.insert("topSpeed", m_window->actionShowTopSpeed->isChecked());
	ret.insert("way", m_window->actionShowWay->isChecked());
	ret.insert("wins", m_window->actionShowWins->isChecked());

	return ret;
}


/****************************************************************************/


void FormMain::statsContextMenu(const QPoint& p)
{
	menuAboutToShow();

	QTableView* t = dynamic_cast<QTableView*>(sender());
	if (t) {
		m_window->menuStatistics->popup(t->viewport()->mapToGlobal(p));
	} else {
		QWidget* w = dynamic_cast<QWidget*>(sender());
		m_window->menuStatistics->popup(w->mapToGlobal(p));
	}
}


/****************************************************************************/


void FormMain::showPreferences()
{
	FormSettings dlg(this);
	if (dlg.exec()) {
		setMapColor();
	}
}


/****************************************************************************/


void FormMain::setMapColor()
{
	QPalette p = m_window->frameMap->palette();
	p.setColor(QPalette::Background, g_config.mapColor());
	m_window->frameMap->setPalette(p);
}
