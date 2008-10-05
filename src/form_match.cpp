#include "form_match.h"
#include "ui_form_match.h"
#include <QMessageBox>
#include <QPainter>
#include <QSettings>


FormMatch::FormMatch(QWidget* parent) :
	QDialog(parent)
{
	m_ui = new Ui::FormMatch;
	m_ui->setupUi(this);
	
	connect(m_ui->btnStart,		SIGNAL(clicked()),		SLOT(start()));
	connect(m_ui->btnAdd,		SIGNAL(clicked()),		SLOT(addPlayer()));
	connect(m_ui->btnRemove,	SIGNAL(clicked()),		SLOT(removePlayer()));
	connect(&m_modelMaps,		SIGNAL(itemChanged(QStandardItem*)), SLOT(mapChanged(QStandardItem*)));
	
	m_ui->treePlayers->setModel(&m_modelPlayers);
	m_ui->treeMaps->setModel(&m_modelMaps);
	
	m_modelPlayers.setHorizontalHeaderLabels(QStringList() << "Name");
	m_modelMaps.setHorizontalHeaderLabels(QStringList() << "Name");
	
	m_colors.append(Qt::red);
	m_colors.append(Qt::green);
	m_colors.append(Qt::blue);
	m_colors.append(Qt::yellow);
	m_colors.append(Qt::cyan);
	m_colors.append(Qt::magenta);
	m_colors.append(Qt::darkYellow);
	m_colors.append(Qt::lightGray);
	
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
		QStringList players = s.value("players").toStringList();
		foreach (const QString& name, players)
			addPlayer(name);
	}
	
	connect(m_ui->treePlayers->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		SLOT(playersSelectionChanged()));
}


FormMatch::~FormMatch()
{
	delete m_ui;
}


void FormMatch::setMaps(const QList<Map>& maps)
{
	m_modelMaps.removeRows(0, m_modelMaps.rowCount());
	
	QSettings s;
	QStringList mapNames = s.value("maps").toStringList();
	
	foreach (const Map& map, maps) {
		QStandardItem* itemName = new QStandardItem(map.name());
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
}


void FormMatch::start()
{
	if (m_modelPlayers.rowCount() < 2) {
		QMessageBox::warning(this, "Unable to start match", "You have to add at least 2 players!");
		return;
	}
	
	if (m_selectedMaps.size() < 1) {
		QMessageBox::warning(this, "Unable to start match", "You have to select at least 1 map!");
		return;
	}
	
	QStringList names;
	for (int i = 0; i < m_modelPlayers.rowCount(); i++) {
		const QString name = m_modelPlayers.item(i)->text();
		
		if (name.isEmpty()) {
			QMessageBox::warning(this, "Unable to start match", "Please give each player a name!");
			return;
		}
		
		if (names.contains(name)) {
			QMessageBox::warning(this, "Unable to start match", QString("There are at least two players named '%1'!\nPlease make sure all player names are unique.").arg(name));
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
	foreach (const Map* map, m_selectedMaps)
		maps << map->name();
	
	QSettings s;
	s.setValue("players", players);
	s.setValue("maps", maps);
}


void FormMatch::addPlayer(QString name)
{
	Q_ASSERT(m_modelPlayers.rowCount() < 8);
	
	if (name.isEmpty()) {
		for (int i = 0; i < 8; i++) {
			name = QString("Player %1").arg(i + 1);
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


QList<Player> FormMatch::players() const
{
	QList<Player> players;
	
	for (int i = 0; i < m_modelPlayers.rowCount(); i++)
		players.append(Player(m_modelPlayers.item(i)->text(), m_colors[i]));
	
	return players;
}


QList<Map> FormMatch::maps() const
{
	QList<Map> maps;
	
	foreach (const Map* map, m_selectedMaps)
		maps.append(*map);
	
	return maps;
}


void FormMatch::mapChanged(QStandardItem* item)
{
	const Map* map = static_cast<const Map*>(item->data().value<void*>());
	if (item->checkState() == Qt::Checked)
		m_selectedMaps.insert(map);
	else
		m_selectedMaps.remove(map);
}
