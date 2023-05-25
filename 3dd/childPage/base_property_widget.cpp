#include "base_property_widget.h"
//#include "myqwidget.h"
//#include <iostream>

base_property_widget::base_property_widget(QWidget *parent, QString str) : QWidget(parent)
{
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	connect(this, SIGNAL(sig_vtk_camera_render()), receiver1, SLOT(slot_vtk_camera_render()));	// 
	connect(this, SIGNAL(sig_copy_node(uint16_t )), receiver1, SLOT(slot_copy_node(uint16_t)));	// 
	connect(this, SIGNAL(sig_save_data(uint16_t )), receiver1, SLOT(slot_save_data(uint16_t)));	// 
	connect(this, SIGNAL(sig_tab_update(uint16_t )), receiver1, SLOT(slot_tab_update(uint16_t)));	// 
	timer = new QTimer();
	timer->setInterval(300);
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_time_out()));
	connect(this, SIGNAL(sig_close_progress()), this, SLOT(slot_close_progress()));
	ia_tmr.setInterval(10);
}

void base_property_widget::create_2buttons(QWidget *parent, const QString label)
{
	m_form_check = new QCheckBox(parent);
	m_form_check->setFixedSize(20, 20);
	m_form_check->setChecked(true);
	connect(m_form_check, SIGNAL(stateChanged(int)), parent, SLOT(slot_checked_actor(int)));	// yzw 0608
	m_form_label = new QLabel(parent);
	m_btn_left = new custom_button_left(parent);
	m_btn_left->setObjectName("button" + QString::number(0));
	m_form_label->setText(label); m_label = label;
	m_form_label->adjustSize();
	m_btn_right = new custom_button_right(parent);
	m_btn_right->setObjectName("button" + QString::number(1));
	parent->setFixedHeight(30);
	parent->setFixedWidth(20 + 10 + 20 + 10 + m_form_label->width() + 10 + 20);
	//this->setWindowFlag(Qt::FramelessWindowHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	m_btn_left->move(0, parent->height() / 2 - 10);
	m_form_check->move(30, parent->height() / 2 - 10);
	m_form_label->move(60, parent->height() / 2 - m_form_label->height() / 2);
	m_btn_right->move(parent->width() - 20, parent->height() / 2 - 10);

	QPalette pal = parent->palette();
	pal.setColor(QPalette::Background, QColor(255, 255, 255, 255));
	parent->setPalette(pal);
	parent->setAutoFillBackground(true);

	connect(m_btn_left, SIGNAL(clicked(bool)), parent, SLOT(slot_ClickButtonDrawLine()));
	connect(m_btn_right, SIGNAL(clicked(bool)), parent, SLOT(slot_ClickButtonDrawLine()));
}

void base_property_widget::create_rbuttons(QWidget *parent, const QString label)
{
	m_form_check = new QCheckBox(parent);
	m_form_check->setFixedSize(20, 20);
	m_form_check->setChecked(true);
	connect(m_form_check, SIGNAL(stateChanged(int)), parent, SLOT(slot_checked_actor(int)));
	m_form_label = new QLabel(parent);
	m_form_label->setText(label); m_label = label;
	m_form_label->adjustSize();
	m_btn_right = new custom_button_right(parent);
	m_btn_right->setObjectName("button" + QString::number(1));
	parent->setFixedHeight(30);
	parent->setFixedWidth(10 + 20 + 10 + m_form_label->width() + 10 + 20);
	m_form_check->move(10, parent->height() / 2 - 10);
	m_form_label->move(40, parent->height() / 2 - m_form_label->height() / 2);
	m_btn_right->move(parent->width() - 20, parent->height() / 2 - 10);
	QPalette pal = parent->palette();
	pal.setColor(QPalette::Background, QColor(255, 255, 255, 255));
	parent->setPalette(pal);
	parent->setAutoFillBackground(true);
	connect(m_btn_right, SIGNAL(clicked(bool)), parent, SLOT(slot_ClickButtonDrawLine()));
}

void base_property_widget::checked(bool c)
{
	disconnect(m_form_check, 0,0,0);
	m_form_check->setChecked(c);
	connect(m_form_check, SIGNAL(stateChanged(int)),this, SLOT(slot_checked_actor(int)));
}

bool base_property_widget::close_progress()
{
	//std::thread::id thread_id = std::this_thread::get_id();
	emit sig_close_progress();
	return true;
}

void base_property_widget::end_progress()
{
}

void base_property_widget::running(bool r)
{
	if (r)
		;//timer->start();
	else {
		//timer->stop();
		//m_form_label->show();
		setLabel();
		end_progress();
	}
}

void base_property_widget::slot_close_progress()
{
	//std::thread::id thread_id = std::this_thread::get_id();
	running(false);
}

void base_property_widget::slot_time_out()
{
	if (m_form_label->isHidden())
		m_form_label->show();
	else
		m_form_label->hide();
}

void base_property_widget::setLabel(QString* label)
{
		m_form_label->setText(label?*label : m_label);
}

void base_property_widget::slot_ClickButtonDrawLine()
{
	myqwidget *p = (myqwidget*)parentWidget();
	QString btnName = QObject::sender()->objectName();
	int index = (btnName.mid(6, 1)).toInt();
	slot_draw_line(index);
}

void base_property_widget::slot_draw_line(int index)
{
	/*
	这段代码是点击按钮画连接线的响应函数，Index是点击的按钮的序号，控件的左边按钮序号是0，右边控件的序号是1
	父控件的set_m_form1_left_button和set_m_form1_right_button函数分别保存点击的第一个控件和第二个控件的按钮序号
	父控件的set_m_first_form1_x_position,set_m_first_form1_y_position函数保存点击的第一个控件的x,y坐标。
	set_m_second_form1_x_position,set_m_second_form1_y_position保存点击的第二个控件的x,y坐标。
	父控件的set_first_form1_clicked(int)设置是否已经点击了第一个按钮，参数为1表示已经点击过第一个控件，0则相反。
	父控件的set_m_record_first_form()和set_m_record_second_form()分别保存点击的第一个控件和第二个控件。
	父控件的add_number_to_m_save_first_form1_button_number()和add_number_to_m_save_second_form1_button_number()
	分别将点击的第一个按钮和第二个按钮的序号保存到对应的list中。
	父控件的add_form1_to_m_save_all_first_form1和add_form1_to_m_save_all_second_form1分别将点击的第一个控件和第二个控件保存到对应的列表中。


	点击按钮画连接线的逻辑是:
	1：如果点击的第一个按钮的序号是0,即点击的第一个控件的左边按钮，那么连接第二个控件的按钮序号应该是1(即第二个控件的右边按钮)，否则连接无效
	2：如果点击的是第一个按钮的序号是1(即点击的是第一个按钮的右边按钮)，那么连接的第二个控件的按钮序号必须是0(即第二个控件的左边按钮)。
	
	*/
	uint16_t id_key1=0, id_key2=0;
	myqwidget *p = (myqwidget*)parentWidget();
	switch (index) 
	{
	case 0:           //点击输入按钮(第一个按钮)
		if (!p->get_first_form1_clicked())
		{
			p->set_m_form1_left_button(1); //即第一个按钮
			p->set_m_first_form1_x_position(this->pos().x()-1); //距离向左减一,避免线条与边界重合
			p->set_m_first_form1_y_position(this->pos().y() + this->height() / 2);
			p->set_m_record_first_form1(this);
			p->set_first_form1_clicked(1);
		}
		else
		{
			if (p->get_m_form1_left_button() == 1)   //两个都是输入按钮,无效
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			if (!first_widget_can_append_second_widget(p->get_m_record_first_form1(), this)) //两个控件连接类型不匹配,无效
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			p->set_m_second_form1_x_position(this->pos().x());
			p->set_m_second_form1_y_position(this->pos().y() + this->height() / 2);
			p->set_m_record_second_form1(this);
			int flag = 0;
			for(int i = 0; i < p->get_m_save_all_first_form1_size(); i++)
			{
				if (((p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1()) && (p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1())) || ((p->get_m_save_all_first_form1(i) == p->get_m_record_second_form1()) && (p->get_m_save_all_second_form1(i) == p->get_m_record_first_form1())))
				{	
					//重复连接即删除
					if (p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1() && p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1())
					{
						if (p->get_m_save_first_form1_button_number(i) == 2)
						{
							OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_first_form1(i)->id_key, p->get_m_save_all_second_form1(i)->id_key);
							p->delete_ith_element_from_first_form1(i);
							p->delete_ith_element_from_second_form1(i);
							p->delete_ith_element_from_first_number(i);
							p->delete_ith_element_from_second_number(i);
							remove_widget_suff_widget(p->get_m_record_first_form1(), p->get_m_record_second_form1()->id_key);
							p->get_m_record_second_form1()->m_pre_widget = nullptr;
							p->get_m_record_first_form1()->update();
							p->get_m_record_second_form1()->update();
						}
						else
						{
							//OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_second_form1(i)->id_key, p->get_m_save_all_first_form1(i)->id_key);
							QMessageBox::information(NULL, "Error", "Circular not allowed");
						}
					}
					else  
					{
						if (p->get_m_save_first_form1_button_number(i) == 2)
						{
							QMessageBox::information(NULL, "Error", "Circular not allowed");
						}
						else
						{
							OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_second_form1(i)->id_key, p->get_m_save_all_first_form1(i)->id_key);
							p->delete_ith_element_from_first_form1(i);
							p->delete_ith_element_from_second_form1(i);
							p->delete_ith_element_from_first_number(i);
							p->delete_ith_element_from_second_number(i);
							remove_widget_suff_widget(p->get_m_record_first_form1(), p->get_m_record_second_form1()->id_key);
							p->get_m_record_second_form1()->m_pre_widget = nullptr;
							p->get_m_record_second_form1()->update();
							p->get_m_record_first_form1()->update();
						}
					}
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			if (p->get_m_record_first_form1() != p->get_m_record_second_form1())
			{
				p->delete_current_form_connection_node(p->get_m_record_second_form1());
				p->add_form1_to_m_save_all_first_form1(p->get_m_record_first_form1());
				p->add_form1_to_m_save_all_second_form1(p->get_m_record_second_form1());
				p->add_number_to_m_save_first_form1_button_number(2);
				p->add_number_to_m_save_second_form1_button_number(1);
				p->get_m_record_first_form1()->m_suff_widget_list.push_back(p->get_m_record_second_form1());
				p->get_m_record_second_form1()->m_pre_widget = p->get_m_record_first_form1();
				OperationalLayer::getOperationInstance().setNodeConnection(p->get_m_record_first_form1()->id_key, p->get_m_record_second_form1()->id_key);
				id_key1 = p->get_m_record_first_form1()->id_key;
				id_key2 = p->get_m_record_second_form1()->id_key;
			}
			p->update();
			p->set_first_form1_clicked(0);
		}
		break;
	case  1:    //输出按钮
		if (!p->get_first_form1_clicked())
		{
			p->set_m_form1_left_button(2);     //即第2个按钮
			p->set_m_first_form1_x_position(this->pos().x() + this->width());
			p->set_m_first_form1_y_position(this->pos().y() + this->height() / 2);
			p->set_m_record_first_form1(this);
			p->set_first_form1_clicked(1);
		}
		else
		{
			if (p->get_m_form1_left_button() == 2) //两个都是输出按钮，无效
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			if (!first_widget_can_append_second_widget(this, p->get_m_record_first_form1())) //两个控件连接类型不匹配,无效
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			p->set_m_second_form1_x_position(this->pos().x() + this->width());
			p->set_m_second_form1_y_position(this->pos().y() + this->height() / 2);
			p->set_m_record_second_form1(this);
			int flag = 0;
			for (int i = 0; i < p->get_m_save_all_first_form1_size(); i++)
			{
				if (((p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1()) && (p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1())) || ((p->get_m_save_all_first_form1(i) == p->get_m_record_second_form1()) && (p->get_m_save_all_second_form1(i) == p->get_m_record_first_form1())))
				{
					if (p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1() && p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1())
					{
						
						if (p->get_m_save_first_form1_button_number(i) == 2)
						{
							//OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_first_form1(i)->id_key, p->get_m_save_all_second_form1(i)->id_key);
							QMessageBox::information(NULL, "Error", "Circular not allowed");
						}
						else
						{
							OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_second_form1(i)->id_key, p->get_m_save_all_first_form1(i)->id_key);
							p->delete_ith_element_from_first_form1(i);
							p->delete_ith_element_from_second_form1(i);
							p->delete_ith_element_from_first_number(i);
							p->delete_ith_element_from_second_number(i);
							remove_widget_suff_widget(p->get_m_record_second_form1(), p->get_m_record_first_form1()->id_key);
							p->get_m_record_first_form1()->m_pre_widget = nullptr;
							p->get_m_record_second_form1()->update();
							p->get_m_record_first_form1()->update();
						}
					}
					else 
					{
						if (p->get_m_save_first_form1_button_number(i) == 2)
						{
							OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_first_form1(i)->id_key, p->get_m_save_all_second_form1(i)->id_key);
							p->delete_ith_element_from_first_form1(i);
							p->delete_ith_element_from_second_form1(i);
							p->delete_ith_element_from_first_number(i);
							p->delete_ith_element_from_second_number(i);
							remove_widget_suff_widget(p->get_m_record_second_form1(), p->get_m_record_first_form1()->id_key);
							p->get_m_record_first_form1()->m_pre_widget = nullptr;
							p->get_m_record_first_form1()->update();
							p->get_m_record_second_form1()->update();
						}
						else
						{
							QMessageBox::information(NULL, "Error", "Circular not allowed");
						}
					}
					flag = 1;
					break;
				}
			}
			if (flag == 1)
			{
				p->set_first_form1_clicked(0);
				p->update();
				return;
			}
			if (p->get_m_record_first_form1() != p->get_m_record_second_form1())
			{
				p->delete_current_form_connection_node(p->get_m_record_first_form1());
				p->add_form1_to_m_save_all_first_form1(p->get_m_record_first_form1());
				p->add_form1_to_m_save_all_second_form1(p->get_m_record_second_form1());
				p->add_number_to_m_save_first_form1_button_number(1);
				p->add_number_to_m_save_second_form1_button_number(2);
				p->get_m_record_second_form1()->m_suff_widget_list.push_back(p->get_m_record_first_form1());
				p->get_m_record_first_form1()->m_pre_widget = p->get_m_record_second_form1();
				OperationalLayer::getOperationInstance().setNodeConnection(p->get_m_record_second_form1()->id_key, p->get_m_record_first_form1()->id_key);
				id_key1 = p->get_m_record_first_form1()->id_key;
				id_key2 = p->get_m_record_second_form1()->id_key;
			}
			p->update();
			p->set_first_form1_clicked(0);
		}
		break;
	}
	if (id_key1) {	// 连上了之后更新视图
		emit sig_tab_update(id_key1);
		emit sig_tab_update(id_key2);
	}
}

void base_property_widget::slot_connect_input()
{
	myqwidget *p = (myqwidget*)parentWidget();
	slot_draw_line(0);
	p->update();
}

void base_property_widget::slot_connect_output()
{
	myqwidget *p = (myqwidget*)parentWidget();
	slot_draw_line(1);
	p->update();
}

void base_property_widget::mousePressEvent(QMouseEvent *event) {
	myqwidget  *p = (myqwidget*)parentWidget();
	if (event->button() == Qt::LeftButton)
	{
			this->raise();
			this->m_mouse_pressPoint = event->pos();
	}
#if 0
	if (p->get_m_record_highlight_form1())
	{
		QPalette pal = p->get_m_record_highlight_form1()->palette();
		//pal.setColor(QPalette::Background, QColor(160, 160, 160, 255));
		pal.setColor(QPalette::Background, QColor(255, 255, 255, 255));
		p->get_m_record_highlight_form1()->setPalette(pal);
		p->get_m_record_highlight_form1()->setAutoFillBackground(true);
	}
	p->set_m_record_highlight_form1(this);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, QColor(0, 255, 160, 255));
	this->setPalette(pal);
	this->setAutoFillBackground(true);
#else
	show_highlight(p, this);
#endif

	highlight1(true);
	p->send_signal_show_property_manager_message(this->m_form_label->text(), this);
}

void base_property_widget::mouseMoveEvent(QMouseEvent *event)
{
	myqwidget  *p = (myqwidget*)parentWidget();
	if (event->buttons() == Qt::LeftButton) {
		//printf("drag	x1: %d, y1:%d\n", event->x(), event->y());
		this->move(this->mapToParent(event->pos() - this->m_mouse_pressPoint));

		//防止按钮移出父窗口		
		if (this->mapToParent(this->rect().topLeft()).x() <= 0) {
			this->move(0, this->pos().y());
		}
		if (this->mapToParent(this->rect().bottomRight()).x() >= this->parentWidget()->rect().width()) {
			this->move(this->parentWidget()->rect().width() - this->width(), this->pos().y());
		}
		if (this->mapToParent(this->rect().topLeft()).y() <= 0) {
			this->move(this->pos().x(), 0);
		}
		if (this->mapToParent(this->rect().bottomRight()).y() >= this->parentWidget()->rect().height()) {
			this->move(this->pos().x(), this->parentWidget()->rect().height() - this->height());
		}
		m_update_base_info();
	}
	p->update();
}


void base_property_widget::enterEvent(QEvent *) //鼠标移动到控件上事件，主要实现鼠标移动到控件上产生吸附的功能
{    
	myqwidget  *p = (myqwidget*)parentWidget();
	if (p->get_first_form1_clicked() == 1)
	{
		p->set_m_record_second_form1(this);
		for (int i = 0; i < p->get_m_save_all_first_form1_size(); i++)
		{
			if ((p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1() && p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1()) || (p->get_m_save_all_first_form1(i) == p->get_m_record_second_form1() && p->get_m_save_all_second_form1(i) == p->get_m_record_first_form1()))
			{
				if (p->get_m_save_all_first_form1(i) == p->get_m_record_first_form1() && p->get_m_save_all_second_form1(i) == p->get_m_record_second_form1())
				{
					if (p->get_m_save_first_form1_button_number(i) == p->get_m_form1_left_button())
						break;
					else
					{
						p->update();
						return;
					}
				}
				else
				{
					if (p->get_m_save_first_form1_button_number(i) != p->get_m_form1_left_button())
						break;
					else
					{
						p->update();
						return;
					}
				}
			}
		}
		p->set_m_mouse_enter_form1_flag(1);
		if (p->get_m_form1_left_button() == 1)
		{
			//不能连接的，不会自动吸附
			if (!first_widget_can_append_second_widget(this, p->get_m_record_first_form1()))
			{
				p->update();
				return;
			}
			p->set_m_second_form1_x_position(this->pos().x() + this->width());
			p->set_m_second_form1_y_position(this->pos().y() + this->height() / 2);
		}
		else
		{
			if (!first_widget_can_append_second_widget(p->get_m_record_first_form1(), this))
			{
				p->update();
				return;
			}
			p->set_m_second_form1_x_position(this->pos().x()-1); //位置减1，避免线条与边界重合
			p->set_m_second_form1_y_position(this->pos().y() + this->height() / 2);
		}
		p->update();
	}
}

void base_property_widget::leaveEvent(QEvent *)
{
	myqwidget *p = (myqwidget*)parentWidget();
	if (p->get_m_mouse_enter_form1_flag() == 1)
		p->set_m_mouse_enter_form1_flag(0);
}

void base_property_widget::contextMenuEvent(QContextMenuEvent *event) 
{
#if 0
	QMenu   *menu = new QMenu(this);
	QMenu  *out_put = menu->addMenu(tr("graphic output"));
	out_put->addAction(tr("clipboard"), this, SLOT(create_cut()));
	out_put->addAction(tr("axis"), this, SLOT(slot_create_axis()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_create_border()));
	QMenu  *normal = menu->addMenu(tr("normal"));
	normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input module(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
		//menu->addAction(new QAction(tr("connect input module"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output module(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output module"), this));
	menu->addAction(new QAction(tr("save data"), this));
	menu->addAction(new QAction(tr("copy"), this));
	//menu->addAction(new QAction(tr("delete"), this，SLOT(delete_self_widget()));
	menu->addAction(tr("delete"), this, SLOT(slot_delete_self_widget()));
	menu->addAction(tr("rename"), this, SLOT(slot_rename_control()));
	menu->move(cursor().pos());
	menu->show();
#endif
}

int base_property_widget::isvalid(base_property_widget *a, int x1, int y1, int x2, int y2)
{
	int right_x = a->pos().x() + a->width();
	int right_y = a->pos().y() + 40;
	int left_x = a->pos().x();
	int left_y = a->pos().y();
	if (right_x <= x2 && right_x >= x1 && right_y <= y2 && right_y >= y1)
		return 1;
	if (left_x >= x1 && left_x <= x2 && left_y >= y1 && left_y <= y2)
		return 1;
	if (right_x >= x1 && right_x <= x2 && left_y >= y1 && left_y <= y2)
		return 1;
	if (left_x >= x1 && left_x <= x2 && right_y >= y1 && right_y <= y2)
		return 1;
	return 0;
}

void base_property_widget::layout1(int *x1, int *y1, int *x2, int *y2)
{
	myqwidget *p = (myqwidget *)parentWidget();
	int i = 0;
	while (i < p->get_m_record_all_form1_size())
	{
		if (isvalid(p->get_m_record_all_form1(i), *x1, *y1, *x2, *y2))
		{
			*y1 += 42;
			*y2 += 42;
			i = 0;
		}
		else
			i++;
	}
}

void base_property_widget::form_layout(myqwidget *m_myqwidget, base_property_widget *new1)
{
	int x1, y1, x2, y2;
	base_property_widget *pre = m_myqwidget->get_m_record_highlight_form1();
	if (pre)
	{
		//int x1 = m_myqwidget->width() - 6 - new1->width();
		//int y1 = m_myqwidget->get_m_record_highlight_form1()->pos().y() - 40;
		 x1 = pre->pos().x() + pre->width()+20;// m_myqwidget->width() - 6 - new1->width();
		 y1 = pre->pos().y() + 40;//  m_myqwidget->get_m_record_highlight_form1()->pos().y() - 40;
		 x2 = x1 + new1->width();
		 y2 = y1 + 40;
		if (y1 <= 0)
			y1 = 0;
		if (x2 > m_myqwidget->width())
			x1 = m_myqwidget->width() - new1->width() - 5;
		//new1->layout1(&x1, &y1, &x2, &y2);
		//new1->setGeometry(x1, y1, new1->width(), new1->height());
	}
	else
	{
		 x1 = m_myqwidget->getMousePoint().x();// m_myqwidget->width() - 6 - new1->width();
		 y1 = m_myqwidget->getMousePoint().y();// 0;
		 x2 = x1 + new1->width();
		 y2 = y1 + 40;
	}
	new1->layout1(&x1, &y1, &x2, &y2);
	new1->setGeometry(x1, y1, new1->width(), new1->height());
}

void base_property_widget::show_highlight(myqwidget *m_myqwidget, base_property_widget *new1)
{
	base_property_widget *widget = m_myqwidget->get_m_record_highlight_form1();
	if (widget == new1)
		return;
	if (widget)
	{
		QPalette pal = widget->palette();
		//pal.setColor(QPalette::Background, QColor(160, 160, 160, 255));
		pal.setColor(QPalette::Background, QColor(255, 255, 255, 255));
		widget->setPalette(pal);
		widget->setAutoFillBackground(true);
		widget->update();
		widget->highlight1(false);
	}
	if(new1){
		m_myqwidget->set_m_record_highlight_form1(new1);
		QPalette pal = new1->palette();
		pal.setColor(QPalette::Background, QColor(0, 255, 160, 255));
		new1->setPalette(pal);
		new1->setAutoFillBackground(true);
		new1->update();
	}
}

void base_property_widget::highlight1(bool hl)
{/*
	QPalette pal = palette();
	pal.setColor(QPalette::Background, hl? QColor(0, 255, 160, 255):(255, 255, 255, 255));
	setPalette(pal);
	setAutoFillBackground(true);*/
	uint16_t key = id_key;
	int8_t projectId = getProjectId(key);
	//int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	node->setFocused(hl);
}

QTabWidget* base_property_widget::return_m_form1_tab()
{
	return m_form1_tab;
}

void base_property_widget::slot_checked_actor(int ns) //  0719
{
	myqwidget  *p = (myqwidget*)parentWidget();
	show_highlight(p, this);
	p->send_signal_show_property_manager_message(this->m_form_label->text(), this); 	// 显示属性页
	uint16_t key = id_key;
	int8_t projectId = getProjectId(key);
	//int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	node->setFocused(true);
	node->setChecked(ns); // 0719 Node->setChecked(ns); 
	emit sig_vtk_camera_render();
}

void base_property_widget::set_color_bar_actor(bool ns) //  0810
{
	myqwidget  *p = (myqwidget*)parentWidget();
	show_highlight(p, this);
	p->send_signal_show_property_manager_message(this->m_form_label->text(), this); 	// 显示属性页
	uint16_t key = id_key;
	int8_t projectId = getProjectId(key);
	//int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<GraphNode> gnode = std::dynamic_pointer_cast<GraphNode> (project->getSpecifiedNode(getNodeId(key)));
	gnode->setColorBarOn(ns); // 0810
	emit sig_vtk_camera_render();
}

void base_property_widget::update_display(std::shared_ptr<SuperNodeStruct> st)
{
	myqwidget  *p = (myqwidget*)parentWidget();
	show_highlight(p, this);
	p->send_signal_show_property_manager_message(this->m_form_label->text(), this); 	// 显示属性页
	uint16_t key = id_key;
	int8_t projectId = getProjectId(key);
	//int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<GraphNode> gnode = std::dynamic_pointer_cast<GraphNode> (project->getSpecifiedNode(getNodeId(key)));
	gnode->updateDisplay(st); // 0810
	emit sig_vtk_camera_render();	;
}

void base_property_widget::slot_cut_plane_property_changed(Child_CutingPlane*, int type)
{
}

// get name, and pos from widget
void base_property_widget::getBaseStruct()
{
	baseStruct->m_name = get_label_text();
	baseStruct->m_info.m_leftTopPosX = pos().x();
	baseStruct->m_info.m_leftTopPosY = pos().y();
	baseStruct->m_info.m_length = width();
	baseStruct->m_info.m_width = height();
	baseStruct->widget = this;
}

 void base_property_widget::m_update_base_info() //0718
{
	getBaseStruct();
	std::shared_ptr<SuperNodeStruct>ptr(std::make_shared<SuperNodeStruct>());
	*(ptr.get()) = *baseStruct;

	OperationalLayer::getOperationInstance().modifyNodeBaseInfo(this->id_key, ptr);
}

 void base_property_widget::base_info_to_widget()
 {
	 m_input_treeitem->setText(1, QString::fromStdString(baseStruct->m_putIn));
 }

 void base_property_widget::data_count_to_widget(int dc, int fc)
 {
	 if(m_input_data_count_treeitem)
		m_input_data_count_treeitem->setText(1, QString::number(dc));
	 if(m_field_index_treeitem)
		 m_field_index_treeitem->setText(1, QString::number(fc));
 }

 void base_property_widget::m_update_widget_property()
 {
	 std::shared_ptr<SuperNodeStruct>ptr(std::make_shared<SuperNodeStruct>());
	  *(ptr.get()) = *baseStruct;

	 OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	 emit sig_vtk_camera_render();
 }

void base_property_widget::slot_rename_control()
{
	int form_width = this->width();
	form_width = form_width - this->m_form_label->width();
	//yzw 0614 QString dlgtitle = QString::fromLocal8Bit("命名");
	//QString txtLabel = "";
	QString defaultInput = this->m_form_label->text();
	QLineEdit::EchoMode echoMode = QLineEdit::Normal;
	bool ok = false;
	/*QString text = QInputDialog::getText(this, STRING_TITLE_RENAME, STRING_LABEL_RENAME, echoMode, defaultInput, &ok);*/
	QInputDialog* dlg = new QInputDialog();
	dlg->setInputMode(QInputDialog::TextInput);
	dlg->setCancelButtonText(tr(u8"取消"));
	dlg->setOkButtonText(tr(u8"确定"));
	dlg->setLabelText(tr(u8"新名字"));
	dlg->setTextValue(defaultInput);
	dlg->setWindowTitle(tr(u8"节点改名"));
	ok= dlg->exec(); 
	QString text = dlg->textValue();
	delete dlg;
	if (ok && !text.isEmpty())
	{
		if (text == this->m_form_label->text())
			return;
		this->m_form_label->setText(text);
		this->m_form_label->adjustSize();
		this->setFixedWidth(form_width+this->m_form_label->width());
		m_btn_right->move(this->width() - 20, this->height() / 2 - 10);
		this->update();
		m_update_base_info();
	} 
}

void base_property_widget::slot_copy_widget()
{
	myqwidget *p = (myqwidget*)parentWidget();
	p->set_paste_flag(true);
	p->set_paste_text(this->NodeType);
	emit sig_copy_node( id_key);
}

void base_property_widget::slot_save_widget()
{
	emit sig_save_data( id_key);	;
}

void base_property_widget::delete_self()
{
}

void base_property_widget::slot_delete_self_widget()  //删除自身控件(删除自己)，鼠标右键中的删除功能
{
	myqwidget *p = (myqwidget*)parentWidget();
	int count_ = 0;
	while (count_ < p->get_m_save_all_first_form1_size())
	{
		if (p->get_m_save_all_first_form1(count_) == this || p->get_m_save_all_second_form1(count_) == this)
		{
			if (p->get_m_save_first_form1_button_number(count_) == 2)
			{
				if (p->get_m_save_all_first_form1(count_) == this)
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_first_form1(count_)->id_key, p->get_m_save_all_second_form1(count_)->id_key);
					p->get_m_save_all_second_form1(count_)->m_pre_widget = nullptr;
					p->get_m_save_all_second_form1(count_)->update();
				}
				else
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_first_form1(count_)->id_key, p->get_m_save_all_second_form1(count_)->id_key);
					remove_widget_suff_widget(p->get_m_save_all_first_form1(count_), p->get_m_save_all_second_form1(count_)->id_key);
					p->get_m_save_all_first_form1(count_)->update();
				}
			}
			else
			{
				if (p->get_m_save_all_first_form1(count_) == this)
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_second_form1(count_)->id_key, p->get_m_save_all_first_form1(count_)->id_key);
					remove_widget_suff_widget(p->get_m_save_all_second_form1(count_), p->get_m_save_all_first_form1(count_)->id_key);
					p->get_m_save_all_second_form1(count_)->update();
				}
				else
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(p->get_m_save_all_second_form1(count_)->id_key, p->get_m_save_all_first_form1(count_)->id_key);
					p->get_m_save_all_first_form1(count_)->m_pre_widget = nullptr;
					p->get_m_save_all_first_form1(count_)->update();
				}
			}
			p->delete_ith_element_from_first_form1(count_);
			p->delete_ith_element_from_second_form1(count_);
			p->delete_ith_element_from_first_number(count_);
			p->delete_ith_element_from_second_number(count_);
		}
		else
			count_++;
	}
	p->update();

	//此处删除该控件的右侧vtk图形以及孩子节点的vtk图形(前提是:该控件有数据)
	//if (this->data_full)
	//{
	//	remove_actor(this);
	//	recursion_delete_actor(this);
	//}
	for (int i = 0; i < p->get_m_record_all_form1_size(); i++)
	{
		if (p->get_m_record_all_form1(i)->id_key == this->id_key)
		{
			p->send_sig_manager_property_null();
			p->get_m_record_all_form1(i)->setVisible(false);
			//p->get_m_record_all_form1(i)->setParent(NULL);
			OperationalLayer::getOperationInstance().deleteNodeFromProject(this->id_key);
			p->delete_ith_element_form_all_form1(i);
			break;
		}
	}
	p->update();
	p->set_m_record_highlight_form1(nullptr);
}

int base_property_widget::first_widget_can_append_second_widget(base_property_widget*a, base_property_widget*b)
{
	for (int i = 0; i < a->m_can_append_widget.size(); i++)
	{
		if (a->m_can_append_widget[i] == b->NodeType)
			return 1;
	}
	return 0;
}

void base_property_widget::remove_widget_suff_widget(base_property_widget*a, uint16_t key_id)
{
	for (int i = 0; i < a->m_suff_widget_list.size(); i++)
	{
		if (a->m_suff_widget_list[i]->id_key == key_id)
		{
			a->m_suff_widget_list.erase(a->m_suff_widget_list.begin() + i);
			break;
		}
	}
}

std::string base_property_widget::get_label_text()
{
	std::string str= m_form_label->text().toStdString();
	return str;
}

void base_property_widget::color_choose(QComboBox* combox)
{
	//QPixmap pix(16, 16);
	//QPainter painter(&pix);
	//painter.fillRect(0, 0, 16, 16, Qt::red);
	QStringList colorNameList = QColor::colorNames();
	QString colorName;
	foreach(colorName, colorNameList)
	{
		QPixmap pix_color(20, 20);
		pix_color.fill(QColor(colorName));
		combox->addItem(QIcon(pix_color), colorName);
	}
}

Child_colorRGB name2crgb(QString t)
{
	QColor color = QColor(t);
	Child_colorRGB c = { (uint16_t)color.red() ,  (uint16_t)color.green(),  (uint16_t)color.blue() };
	return c;
}

QString crgb2name(Child_colorRGB crgb){
	QStringList colorNameList = QColor::colorNames();
	QString colorName;
	foreach(colorName, colorNameList)
	{
		QColor color = QColor(crgb.m_r, crgb.m_g, crgb.m_b);
		if (color == QColor(colorName))
			return colorName;
	}
	return "";
}

// https://blog.csdn.net/BaiJing1999/article/details/102158097
// http://www.ncl.ucar.edu/Document/Graphics/color_table_gallery.shtml
// https://www.schemecolor.com/
void base_property_widget::gradient_color(QComboBox* combox)
{
	QPixmap pix0(100, 20);
	QPainter painter0(&pix0);
	QLinearGradient linear0(0, 0, 100, 20);
	linear0.setColorAt(0.14, QColor(153, 102, 252));
	linear0.setColorAt(0.28, QColor(58, 39, 255));
	linear0.setColorAt(0.42, QColor(0, 255, 0));
	linear0.setColorAt(0.56, QColor(255, 255, 0));
	linear0.setColorAt(0.70, QColor(255, 220, 0));
	linear0.setColorAt(0.84, QColor(255, 102, 0));
	linear0.setColorAt(1.0, QColor(255, 0, 0));
	painter0.setBrush(QBrush(linear0));
	painter0.drawRect(pix0.rect());
	combox->addItem(QIcon(pix0), "custom");

	combox->setIconSize(QSize(150, 20));
	QPixmap pix(100,20);
	QPainter painter(&pix);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QLinearGradient linear(0, 0, 100, 20);
	linear.setColorAt(0.2, Qt::white);
	linear.setColorAt(0.4, Qt::red);
	linear.setColorAt(0.6, Qt::green);
	linear.setColorAt(1.0, Qt::black);
	painter.setBrush(QBrush(linear));
	painter.drawRect(pix.rect());
	combox->addItem(QIcon(pix),"white-black");

	QPixmap pix1(100, 20);
	QPainter painter1(&pix1);
	QLinearGradient linear1(0, 0, 100, 20);
	linear1.setColorAt(0.3, Qt::red);
	linear1.setColorAt(0.5, Qt::yellow);
	linear1.setColorAt(0.7, Qt::green);
	linear1.setColorAt(1.0, Qt::blue);
	painter1.setBrush(QBrush(linear1));
	painter1.drawRect(pix1.rect());
	combox->addItem(QIcon(pix1), "red-blue");

	QPixmap pix2(100, 20);
	QPainter painter2(&pix2);
	QLinearGradient linear2(0, 0, 100, 20);
	linear2.setColorAt(0.2, QColor("seagreen"));
	linear2.setColorAt(0.4, QColor("gold"));
	linear2.setColorAt(0.7, QColor("beige"));
	linear2.setColorAt(1.0, QColor("mediumblue"));
	painter2.setBrush(QBrush(linear2));
	painter2.drawRect(pix2.rect());
	combox->addItem(QIcon(pix2), "seagreen-mediumblue");

	QPixmap pix3(100, 20);
	QPainter painter3(&pix3);
	QLinearGradient linear3(0, 0, 100, 20);
	linear3.setColorAt(0.14, QColor("red"));
	linear3.setColorAt(0.28, QColor("orange"));
	linear3.setColorAt(0.42, QColor("yellow"));
	linear3.setColorAt(0.56, QColor("green"));
	linear3.setColorAt(0.70, QColor("darkcyan"));
	linear3.setColorAt(0.84, QColor("blue"));
	linear3.setColorAt(1.0, QColor("purple"));
	painter3.setBrush(QBrush(linear3));
	painter3.drawRect(pix3.rect());
	combox->addItem(QIcon(pix3), "red-purple");

	QPixmap pix4(100, 20);
	QPainter painter4(&pix4);
	QLinearGradient linear4(0, 0, 100, 20);
	linear4.setColorAt(0.2, QColor("blue"));
	linear4.setColorAt(0.28, QColor("black"));
	linear4.setColorAt(0.32, QColor("green"));
	linear4.setColorAt(0.56, QColor("yellow"));
	linear4.setColorAt(0.70, QColor("orange"));
	linear4.setColorAt(1.0, QColor("red"));
	painter4.setBrush(QBrush(linear4));
	painter4.drawRect(pix4.rect());
	combox->addItem(QIcon(pix4), "blue-red");

	QPixmap pix5(100, 20);
	QPainter painter5(&pix5);
	QLinearGradient linear5(0, 0, 100, 20);
	linear5.setColorAt(0.2, QColor(195, 20, 24));
	linear5.setColorAt(0.28, QColor(247, 106, 14));
	linear5.setColorAt(0.32, QColor(234, 205, 30));
	linear5.setColorAt(0.56, QColor(102, 171, 9));
	linear5.setColorAt(0.70, QColor(19, 125, 224));
	linear5.setColorAt(1.0, QColor(136, 9, 181));
	painter5.setBrush(QBrush(linear5));
	painter5.drawRect(pix5.rect());
	combox->addItem(QIcon(pix5), "prism");

	QPixmap pix6(100, 20);
	QPainter painter6(&pix6);
	QLinearGradient linear6(0, 0, 100, 20);
	linear6.setColorAt(0.2, QColor(51, 51, 51));
	linear6.setColorAt(0.28, QColor(71, 71, 71));
	linear6.setColorAt(0.32, QColor(82, 82, 82));
	linear6.setColorAt(0.56, QColor(143, 143, 143));
	linear6.setColorAt(0.70, QColor(178, 178, 178));
	linear6.setColorAt(1.0, QColor(255, 255, 255));
	painter6.setBrush(QBrush(linear6));
	painter6.drawRect(pix6.rect());
	combox->addItem(QIcon(pix6), "gray");
}


//该函数主要用来连接两个控件时调用，同时要递归添加actor(前提是:控件的前节点有数据,否则不调用)
void base_property_widget::recursion_add_actor(base_property_widget *new1)
{

		for (int i = 0; i < new1->m_suff_widget_list.size(); i++)
		{
			//add_actor(new1->m_suff_widget_list[i]); 将当前孩子节点控件图形添加到右侧界面
			recursion_add_actor(new1->m_suff_widget_list[i]);
		}
}

//该函数主要是断开两个控件时调用，同时要递归删除该控件的孩子控件的图像(即右侧vtk图像)
void base_property_widget::recursion_delete_actor(base_property_widget *new1)
{
	for (int i = 0; i < new1->m_suff_widget_list.size(); i++)
	{
		//remove_actor(new1->m_suff_widget_list[i]);  将当前孩子控件图形从右侧界面移除
		recursion_delete_actor(new1->m_suff_widget_list[i]);
	}
}
