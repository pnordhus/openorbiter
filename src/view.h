#ifndef VIEW_H
#define VIEW_H


#include <QGraphicsView>


class View : public QGraphicsView
{
	Q_OBJECT

public:
	View(QWidget* parent);

signals:
	void	keyPressed(int key);
	void	pause();
	void	resume();

public slots:
	void	enableGL(bool enable);
	void	enableAntiAliasing(bool enable);
	void	switchSettings(bool gl, bool antiAliasing);

protected:
	void	resizeEvent(QResizeEvent*);
	void	keyPressEvent(QKeyEvent*);
	void	focusInEvent(QFocusEvent*);
	void	focusOutEvent(QFocusEvent*);

private:
	bool	m_gl;
	bool	m_antiAliasing;
};


#endif // VIEW_H
