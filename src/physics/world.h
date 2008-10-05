#ifndef WORLD_H
#define WORLD_H


#include "object.h"
#include "vector.h"
#include <QList>


class Circle;


class World
{
public:
	World();
	~World();

public:
	void	setGravity(const Vector& gravity);
	void	process(float time);

private:
	void	collide();
	void	collide(Object* obj1, Object* obj2);
	void	collide2(Circle* obj1, Circle* obj2);
	void	registerObject(Object* obj);
	void	unregisterObject(Object* obj);

private:
	QList<Object*>	m_objects;
	Vector			m_gravity;

friend class Object;
};


#endif // WORLD_H
