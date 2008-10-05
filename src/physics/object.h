#ifndef OBJECT_H
#define OBJECT_H


#include "vector.h"
#include <QObject>


class World;


class Object : public QObject
{
	Q_OBJECT

public:
	enum Type { Circle, Plane };

public:
	Type			type() const { return m_type; }
	const Vector&	position() const { return m_position; }
	const Vector&	speed() const { return m_speed; }
	float			mass() const { return m_mass; }
	void			setPosition(const Vector& pos);
	void			accelerate(const Vector& acc);
	void			accelerate(float acc);
	void			move(float time);
	void			move(const Vector& dir);
	void			setWorld(World* world);
	void			link(const Vector& pos);
	void			unlink();
	bool			isLinked() const { return m_linked; }

signals:
	void			unlinked();

protected:
	Object(Type type);
	virtual ~Object();

private:
	const Type	m_type;
	World*		m_world;
	Vector		m_position;
	Vector		m_speed;
	float		m_mass;
	Vector		m_linkPos;
	bool		m_linked;
	float		m_linkSpeed;
	float		m_linkRadius;

Q_DISABLE_COPY(Object);
};


#endif // OBJECT_H
