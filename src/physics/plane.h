#ifndef PLANE_H
#define PLANE_H


#include "object.h"


class Plane : public Object
{
public:
	Plane(const Vector& normal);

private:
	Vector	m_normal;
};


#endif // PLANE_H
