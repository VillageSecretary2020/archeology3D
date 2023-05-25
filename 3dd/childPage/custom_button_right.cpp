#include "custom_button_right.h"
#include <QPainter>
custom_button_right::custom_button_right(QWidget*parent) :QPushButton(parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint);
	setFixedSize(20, 20);
	setAttribute(Qt::WA_TranslucentBackground);
}

void custom_button_right::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setPen(Qt::black);
	p.setBrush(Qt::yellow);
	static const QPointF points[5] = {
		QPointF(0,0),
		QPointF(10, 0),
		QPointF(20,10),
		QPointF(10,20),
		QPointF(0,20)
	};
	p.drawConvexPolygon(points, 5);

}