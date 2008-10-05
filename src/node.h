#ifndef NODE_H
#define NODE_H


#include "physics/vector.h"
#include <QGraphicsEllipseItem>


class Scene;


class Node
{
public:
	Node(Scene& scene);
	~Node();

public:
	void	setPosition(const Vector& pos);
	Vector	position() const;

private:
	QGraphicsEllipseItem*	m_item;
};


#endif // NODE_H
