#include "game.h"
#include "map.h"
#include "node.h"
#include "orbiter.h"
#include "player.h"
#include "scene.h"
#include <algorithm>
#include <limits>


Game::Game(Scene& scene, const Map& map, const QList<Player*>& players) :
	m_scene(scene)
{
	const float width = map.width();
	const float height = map.height();
	
	m_rect.setWidth(width);
	m_rect.setHeight(height);
	
	m_scene.setField(width, height);
	
	m_world.setGravity(Vector(0,10));
	
	foreach (const Vector& pos, map.nodes()) {
		Node* node = new Node(m_scene);
		node->setPosition(pos);
		m_nodes.append(node);
	}
	
	QList<Vector> spawns = map.spawns();
	std::random_shuffle(spawns.begin(), spawns.end());
	
	Q_ASSERT(spawns.size() >= players.size());
	
	foreach (Player* player, players) {
		Orbiter* orb = new Orbiter(player);
		m_orbiters.append(orb);
		orb->setPosition(spawns.takeFirst());
		orb->enter(*this, m_world, m_scene);
	}
	
	std::random_shuffle(m_orbiters.begin(), m_orbiters.end());
	std::random_shuffle(m_nodes.begin(), m_nodes.end());

}


Game::~Game()
{
	qDeleteAll(m_orbiters);
	qDeleteAll(m_nodes);
}


void Game::keyPressed(int key)
{
	foreach (Orbiter* orb, m_orbiters) {
		if (orb->key() == key) {
			orb->toggleConnect();
			break;
		}
	}
}


const Player* Game::process(float time)
{
	foreach (Orbiter* orb, m_orbiters)
		orb->process();
	
	m_world.process(time);
	
	for (int i = m_orbiters.size() - 1; i >= 0; i--) {
		Orbiter* orb = m_orbiters.at(i);
		if (!m_rect.adjusted(orb->radius(), orb->radius(), -orb->radius(), -orb->radius()).contains(orb->position().x, orb->position().y)) {
			delete orb;
			m_orbiters.removeAt(i);
			if (m_orbiters.size() == 1) {
				Player* winner = m_orbiters.first()->player();
				winner->incWins();
				return winner;
			}
		}
	}
	
	foreach (Orbiter* orb, m_orbiters)
		orb->update();
	
	return NULL;
}


Node* Game::nearestNode(const Vector& pos)
{
	float minDist = std::numeric_limits<float>::infinity();
	Node* nearestNode = NULL;
	
	foreach (Node* node, m_nodes) {
		const float dist = (node->position() - pos).lengthSquared();
		if (dist < minDist) {
			minDist = dist;
			nearestNode = node; 
		}
	}
	
	return nearestNode;
}
