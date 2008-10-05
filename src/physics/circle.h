#ifndef CIRCLE_H
#define CIRCLE_H


#include "object.h"


class Circle : public Object
{
public:
	Circle(float radius);

public:
	float	radius() const { return m_radius; }

private:
	float	m_radius;
};


#endif // CIRCLE_H
