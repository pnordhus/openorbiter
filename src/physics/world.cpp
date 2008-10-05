#include "circle.h"
#include "world.h"


World::World()
{
	
}


World::~World()
{
	Q_ASSERT(m_objects.isEmpty());
}


void World::setGravity(const Vector& gravity)
{
	m_gravity = gravity;
}


void World::process(float time)
{
	const uint iterations = time / 0.01;
	const float stepSize = time / iterations;
	
	for (uint i = 0; i < iterations; i++) {
		foreach (Object* obj, m_objects) {
			if (obj->isLinked()) {
				obj->accelerate(stepSize);
			} else {
				// gravity only applies to non-linked objects
				obj->accelerate(m_gravity * stepSize);
			}
			
			obj->move(stepSize);
		}
		
		collide();
	}
}


void World::collide()
{
	for (int i = 0; i + 1 < m_objects.size(); i++) {
		for (int j = i + 1; j < m_objects.size(); j++) {
			collide(m_objects[i], m_objects[j]);
		}
	}
}


void World::collide(Object* obj1, Object* obj2)
{
	if ((obj1->type() == Object::Circle) && (obj2->type() == Object::Circle)) {
		collide2(static_cast<Circle*>(obj1), static_cast<Circle*>(obj2));
		return;
	}
	
	Q_ASSERT(false);
}


void World::collide2(Circle* obj1, Circle* obj2)
{
	const float dist = (obj1->position() - obj2->position()).length();
	if (dist >= obj1->radius() + obj2->radius())
		return;
	
	obj1->unlink();
	obj2->unlink();
	
	const Vector to1 = (obj1->position() - obj2->position()).normalized();
	const Vector to2 = -to1;
	
	const Vector speed1 = obj1->speed();
	const Vector speed2 = obj2->speed();
	
	const float mass1 = obj1->mass();
	const float mass2 = obj2->mass();
	const float massSum = mass1 + mass2;
	
	float impulseTransferToFactor1 = ((mass1 - mass2) * speed1.length() + 2.0f * mass2 * speed2.length()) / massSum;
	float impulseTransferToFactor2 = ((mass2 - mass1) * speed2.length() + 2.0f * mass1 * speed1.length()) / massSum;
	
	const float angleAffectorTo1 = to1 * speed2;
	const float angleAffectorTo2 = to2 * speed1;
	
	impulseTransferToFactor1 = angleAffectorTo1 * impulseTransferToFactor1;
	impulseTransferToFactor2 = angleAffectorTo2 * impulseTransferToFactor2;
	
	const Vector impulseTransferTo1 = to1 * impulseTransferToFactor1;
	const Vector impulseTransferTo2 = to2 * impulseTransferToFactor2;
	
	obj1->accelerate(impulseTransferTo1 - impulseTransferTo2);
	obj2->accelerate(impulseTransferTo2 - impulseTransferTo1);
	
	const float move = (obj1->radius() + obj2->radius() - dist) / 2.0f;
	obj1->move(to1 * move);
	obj2->move(to2 * move);
}


void World::registerObject(Object* obj)
{
	m_objects.append(obj);
}


void World::unregisterObject(Object* obj)
{
	m_objects.removeAll(obj);
}
