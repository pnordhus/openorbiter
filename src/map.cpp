#include "map.h"


Map::Map(const QString& name, float width, float height) :
	m_name(name),
	m_width(width),
	m_height(height)
{
	
}


void Map::addNode(const Vector& pos)
{
	m_nodes.append(pos);
}


void Map::addSpawn(const Vector& spawn)
{
	m_spawns.append(spawn);
}
