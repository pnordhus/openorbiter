#ifndef PLAYER_H
#define PLAYER_H


#include <QColor>
#include <QString>


class Player
{
public:
	Player(const QString& name, const QColor& color);

public:
	const QString&	name() const { return m_name; }
	const QColor&	color() const { return m_color; }
	int				key() const { return m_key; }
	void			setKey(int key);
	void			incWins();
	uint			wins() const { return m_wins; }

private:
	QString	m_name;
	QColor	m_color;
	int		m_key;
	uint	m_wins;
};


#endif // PLAYER_H
