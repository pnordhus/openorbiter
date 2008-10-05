#include "scene.h"
#include <QGraphicsRectItem>
#include <QGraphicsView>


Scene::Scene()
{
	m_field = addRect(0.0f, 0.0f, 40.0f, 30.0f, QPen(QColor(200, 0, 0), 0.2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin), QBrush(QColor(0,0,40)));
	
	m_text = addText("");
	m_text->setZValue(1000.0f);
	m_text->setDefaultTextColor(Qt::white);
	m_text->hide();
	
	m_textBackground = addRect(QRectF(), QPen(Qt::NoPen), QBrush(QColor(0,0,0,180)));
	m_textBackground->setZValue(900.0f);
	m_textBackground->hide();
}


Scene::~Scene()
{
	
}


void Scene::setField(float width, float height)
{
	m_field->setRect(0.0f, 0.0f, width, height);
	setSceneRect(-0.2, -0.2, width + 0.4, height + 0.4);
}


void Scene::showText(const QString& text)
{
	m_text->setTransform(QTransform());
	m_text->scale(0.08, 0.08);
	m_text->setTextWidth(sceneRect().width() / 0.08);
	m_text->setHtml(text);
	
	QRectF rect = m_text->sceneBoundingRect();
	QPointF pos;
	pos.setX((sceneRect().width() - rect.width()) / 2.0f);
	pos.setY((sceneRect().height() - rect.height()) / 2.0f);
	m_text->setPos(pos);
	
	rect = m_text->sceneBoundingRect();
	rect.adjust(-sceneRect().width(), 0, sceneRect().width(), 0);
	m_textBackground->setRect(rect);

	m_text->show();	
	m_textBackground->show();
}


void Scene::hideText()
{
	m_text->hide();	
	m_textBackground->hide();
}
