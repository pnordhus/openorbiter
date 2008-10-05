#ifndef FORM_MAIN_H
#define FORM_MAIN_H


#include "scene.h"
#include <QMainWindow>


class Map;
class Match;
namespace Ui { class FormMain; }


class FormMain : public QMainWindow
{
	Q_OBJECT

public:
	FormMain();
	~FormMain();

public:
	void	show();

private slots:
	void	newMatch();

protected:
	void	closeEvent(QCloseEvent*);

private:
	Ui::FormMain*	m_ui;
	
	Scene			m_scene;
	Match*			m_match;
	bool			m_firstShow;
	
	QList<Map>		m_maps;
};


#endif // FORM_MAIN_H
