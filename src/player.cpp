#include "player.h"


Player::Player(const QString& name, const QColor& color) :
	m_name(name),
	m_color(color),
	m_key(0),
	m_wins(0)
{
	
}


void Player::setKey(int key)
{
	m_key = key;
}


void Player::incWins()
{
	m_wins++;
}
