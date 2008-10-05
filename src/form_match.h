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
