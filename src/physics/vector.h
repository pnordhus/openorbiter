#ifndef VECTOR_H
#define VECTOR_H


#include <cmath>


class Vector
{
public:
	Vector() : x(0.0), y(0.0) {}
	Vector(float x, float y) : x(x), y(y) {}
	explicit Vector(float angle) : x(std::cos(angle)), y(std::sin(angle)) {}

public:
	float angle() const
	{
		return atan2(y, x);
	}
	
	float lengthSquared() const
	{
		return x * x + y * y;
	}
	
	float length() const
	{
		return std::sqrt(lengthSquared());
	}
	
	void setLength(float newLen)
	{
		const float len = length();
		x *= newLen / len;
		y *= newLen / len;
	}
	
	Vector normalized() const
	{
		const float len = length();
		return Vector(x / len, y / len);
	}
	
	Vector& operator += (const Vector& rho)
	{
		x += rho.x;
		y += rho.y;
		return *this;
	}
	
	Vector operator + (const Vector& rho) const
	{
		return Vector(x + rho.x, y + rho.y);
	}
	
	Vector operator - (const Vector& rho) const
	{
		return Vector(x - rho.x, y - rho.y);
	}
	
	Vector operator * (float v) const
	{
		return Vector(x * v, y * v);
	}
	
	Vector operator - () const
	{
		return Vector(-x, -y);
	}
	
	float operator * (const Vector& rho) const
	{
		return cos(angle() - rho.angle());
	}

public:
	float	x;
	float	y;
};


#endif // VECTOR_H
