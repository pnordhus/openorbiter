#ifndef MAP_H
#define MAP_H


#include "physics/vector.h"
#include <QList>
#include <QString>


class Map
{
public:
	Map(const QString& name, float width, float height);

public:
	const QString&	name() const { return m_name; }
	float			width() const { return m_width; }
	float			height() const { return m_height; }
	const QList<Vector>& nodes() const { return m_nodes; }
	const QList<Vector>& spawns() const { return m_spawns; }

public:
	void	addNode(const Vector& pos);
	void	addSpawn(const Vector& spawn);

private:
	QString			m_name;
	float			m_width;
	float			m_height;
	
	QList<Vector>	m_nodes;
	QList<Vector>	m_spawns;
};


#endif // MAP_H
