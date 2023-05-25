#pragma once
#include <QPushButton>
class custom_button_left :public QPushButton
{
public:
	custom_button_left(QWidget*parent = 0);
	void paintEvent(QPaintEvent *);

};

