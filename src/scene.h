#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>


class Scene : public QGraphicsScene
{
	Q_OBJECT

public:
	Scene();
	~Scene();

public:
	void	setField(float width, float height);
	void	showText(const QString& text);
	void	hideText();

private:
	QGraphicsRectItem*	m_field;
	QGraphicsTextItem*	m_text;
	QGraphicsRectItem*	m_textBackground;
};


#endif // SCENE_H
