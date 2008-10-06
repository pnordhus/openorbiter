#include "bouncerdef.h"


BouncerDef::BouncerDef() :
	m_width(0.0f),
	m_boost(0.0f)
{
	
}


void BouncerDef::setPositions(const Vector& pos1, const Vector& pos2)
{
	m_position1 = pos1;
	m_position2 = pos2;
}


void BouncerDef::setWidth(float width)
{
	m_width = width;
}


void BouncerDef::setBoost(float boost)
{
	m_boost = boost;
}
