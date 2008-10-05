#include "node.h"
#include "scene.h"


Node::Node(Scene& scene)
{
	const float radius = 0.5;
	m_item = scene.addEllipse(-radius / 2.0f, -radius / 2.0f, radius, radius);
	m_item->setPen(Qt::NoPen);
	m_item->setBrush(QBrush(Qt::white));
	m_item->setZValue(10);
}


Node::~Node()
{
	delete m_item;
}


void Node::setPosition(const Vector& pos)
{
	m_item->setPos(pos.x, pos.y);
}


Vector Node::position() const
{
	return Vector(m_item->pos().x(), m_item->pos().y());
}
