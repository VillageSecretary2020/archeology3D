#pragma once
#include <QPushButton>
class custom_button_right :public QPushButton
{
public:
	custom_button_right(QWidget*parent = 0);
	void paintEvent(QPaintEvent *);

};