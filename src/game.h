#ifndef GAME_H
#define GAME_H


#include "physics/world.h"
#include <QList>
#include <QObject>
#include <QRectF>


class Map;
class Node;
class Orbiter;
class Player;
class Scene;


class Game : public QObject
{
	Q_OBJECT

public:
	Game(Scene& scene, const Map& map, const QList<Player*>& players);
	~Game();

public:
	Node*			nearestNode(const Vector& pos);
	void			keyPressed(int key);

public slots:
	const Player*	process(float time);

private:
	World			m_world;
	Scene&			m_scene;
	
	QList<Orbiter*>	m_orbiters;
	QList<Node*>	m_nodes;
	QRectF			m_rect;
};


#endif // GAME_H
