#include "plane.h"


Plane::Plane(const Vector& normal) :
	Object(Object::Plane),
	m_normal(normal)
{
	
}
