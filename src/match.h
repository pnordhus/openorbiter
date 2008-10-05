#ifndef MATCH_H
#define MATCH_H


#include <QColor>
#include <QQueue>
#include <QTime>
#include <QTimer>


class Game;
class Map;
class Orbiter;
class Player;
class Scene;


class Match : public QObject
{
	Q_OBJECT

public:
	Match(Scene& scene);
	~Match();

public:
	void	start();
	void	setPlayers(const QList<Player>& players);
	void	setMaps(const QList<Map>& maps);

public slots:
	void	keyPressed(int key);
	void	pause();
	void	resume();

private slots:
	void	process();

private:
	void	newGame();
	Map		nextMap();
	bool	requestKey();

private:
	QList<Player*>	m_players;
	QQueue<Map>		m_maps;
	
	Scene&			m_scene;
	Game*			m_game;
	QTimer			m_timer;
	QTime			m_time;
	bool			m_paused;
	bool			m_gameOver;
	QString			m_countdownText;
	float			m_countdown;
	int				m_countdownInt;
	
	QList<Player*>	m_needKey;
};


#endif // MATCH_H
