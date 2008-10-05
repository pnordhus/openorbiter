#include "object.h"
#include "world.h"


Object::Object(Type type) :
	m_type(type),
	m_world(NULL),
	m_mass(1.0f),
	m_linked(false)
{
	
}


Object::~Object()
{
	if (m_world)
		m_world->unregisterObject(this);
}


void Object::setWorld(World* world)
{
	if (m_world)
		m_world->unregisterObject(this);
	
	m_world = world;
	
	if (m_world)
		m_world->registerObject(this);
}


void Object::setPosition(const Vector& pos)
{
	m_position = pos;
}


void Object::accelerate(const Vector& acc)
{
	Q_ASSERT(!m_linked);
	
	m_speed += acc;
}


void Object::accelerate(float acc)
{
	Q_ASSERT(m_linked);
	
	if (m_linkSpeed > 0.0f)
		m_linkSpeed += acc;
	else
		m_linkSpeed -= acc;
}


void Object::move(float time)
{
	if (m_linked) {
		const float d = m_linkSpeed * time;
		const float angle = d / m_linkRadius;
		const Vector newDir((m_position - m_linkPos).angle() + angle);
		m_position = m_linkPos + newDir * m_linkRadius;
	} else {
		m_position += m_speed * time;
	}
}


void Object::move(const Vector& dir)
{
	m_position += dir;
}


void Object::link(const Vector& pos)
{
	m_linkPos = pos;
	m_linkRadius = (m_position - pos).length(); 
	
	const float factor = std::sin(m_speed.angle() - (m_position - pos).angle());
	m_linkSpeed = m_speed.length() * factor;
	
	m_linked = true;
}


void Object::unlink()
{
	if (!m_linked)
		return;
	
	m_linked = false;
	
	const Vector center = m_linkPos - m_position;
	if (m_linkSpeed > 0.0f) {
		m_speed.x = center.y;
		m_speed.y = -center.x;
		m_speed.setLength(m_linkSpeed);
	} else {
		m_speed.x = -center.y;
		m_speed.y = center.x;
		m_speed.setLength(-m_linkSpeed);
	}
	
	emit unlinked();
}
