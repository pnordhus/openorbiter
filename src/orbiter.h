#ifndef ORBITER_H
#define ORBITER_H


#include "player.h"
#include <QGraphicsEllipseItem>


class Circle;
class Game;
class Node;
class Scene;
class Vector;
class World;


class Orbiter : public QObject
{
	Q_OBJECT

public:
	Orbiter(Player* player);
	~Orbiter();

public:
	Player*			player() const { return m_player; }
	const QString&	name() const { return m_player->name(); }
	const QColor&	color() const { return m_player->color(); }
	int				key() const { return m_player->key(); }
	Vector			position() const;
	float			radius() const;

public:
	void	setPosition(const Vector& position);
	void	process();
	void	update();
	
	void	enter(Game& game, World& world, Scene& scene);
	void	leave();
	
	void	setKey(int key);
	void	toggleConnect();

private slots:
	void	connectNode();
	void	disconnectNode();

private:
	Player*					m_player;
	
	QGraphicsEllipseItem*	m_graphicsItem;
	QGraphicsLineItem*		m_connectionLine;
	Circle*					m_circle;
	Game*					m_game;
	Node*					m_node;
	bool					m_connected;
};


#endif // ORBITER_H
