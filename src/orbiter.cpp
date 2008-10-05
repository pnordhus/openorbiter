#include "game.h"
#include "node.h"
#include "orbiter.h"
#include "scene.h"
#include "physics/circle.h"
#include <QBrush>
#include <QPen>


Orbiter::Orbiter(Player* player) :
	m_player(player),
	m_game(NULL),
	m_node(NULL),
	m_connected(false)
{
	const float radius = 0.5f; 
	
	m_graphicsItem = new QGraphicsEllipseItem(-radius, -radius, radius * 2.0f, radius * 2.0f);
	m_graphicsItem->setPen(QPen(Qt::NoPen));
	m_graphicsItem->setBrush(QBrush(color()));
	m_graphicsItem->setZValue(100);
	
	m_connectionLine = new QGraphicsLineItem;
	m_connectionLine->setZValue(5);
	m_connectionLine->setPen(QPen(Qt::white));
	m_connectionLine->hide();
	
	m_circle = new Circle(radius);
	connect(m_circle, SIGNAL(unlinked()), SLOT(disconnectNode()));
}


Orbiter::~Orbiter()
{
	delete m_connectionLine;
	delete m_circle;
	delete m_graphicsItem;
}


void Orbiter::enter(Game& game, World& world, Scene& scene)
{
	Q_ASSERT(m_game == NULL);
	
	m_game = &game;
	m_circle->setWorld(&world);
	
	if (m_graphicsItem->scene()) {
		m_graphicsItem->scene()->removeItem(m_graphicsItem);
		m_connectionLine->scene()->removeItem(m_connectionLine);
	}
	
	scene.addItem(m_graphicsItem);
	scene.addItem(m_connectionLine);
	
	
}


void Orbiter::leave()
{
	Q_ASSERT(m_game != NULL);
	
	m_circle->setWorld(NULL);
	
	if (m_graphicsItem->scene()) {
		m_graphicsItem->scene()->removeItem(m_graphicsItem);
		m_connectionLine->scene()->removeItem(m_connectionLine);
	}
	
	m_game = NULL;
}


void Orbiter::toggleConnect()
{
	if (m_node)
		disconnectNode();
	else
		connectNode();
}


void Orbiter::connectNode()
{
	m_connectionLine->setPen(QPen(Qt::darkRed));
	
	m_node = m_game->nearestNode(m_circle->position());
	m_connected = false;
}


void Orbiter::disconnectNode()
{
	m_connectionLine->hide();
	m_connected = false;
	m_node = NULL;
	m_circle->unlink();
}


void Orbiter::process()
{
	if (m_node && !m_connected) {
		m_connected = true;
		m_connectionLine->setPen(QPen(Qt::white));
		m_circle->link(m_node->position());
	}
}


Vector Orbiter::position() const
{
	return m_circle->position();
}


float Orbiter::radius() const
{
	return m_circle->radius();
}


void Orbiter::update()
{
	if (m_node) {
		m_connectionLine->setLine(m_circle->position().x, m_circle->position().y, m_node->position().x, m_node->position().y);
		m_connectionLine->show();
	}
	
	m_graphicsItem->setPos(m_circle->position().x, m_circle->position().y);
}


void Orbiter::setPosition(const Vector& pos)
{
	m_circle->setPosition(pos);
	m_graphicsItem->setPos(pos.x, pos.y);
}
