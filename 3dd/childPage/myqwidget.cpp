//#include "myqwidget.h"
#include "base_property_widget.h"
#include <QMetaType>

myqwidget::myqwidget(QWidget *parent) : QWidget(parent)
{
	qRegisterMetaType<QVector<int>>("QVector<int>");
	this->setMouseTracking(true);

	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, Qt::gray);
	this->setPalette(pal);
	this->setAutoFillBackground(true);
	//this->setStyleSheet("base_property_widget{border:1px solid rgb(179,216,247);}");
	setFocusPolicy(Qt::StrongFocus);
}

void myqwidget::paintEvent(QPaintEvent *event) 
{
	/*
	这是联络图类的绘制事件，当更新时，会更新界面，比如连接了两个控件，增加控件，点击按钮画线都会更新此界面。
	
	绘画逻辑是:
	先遍历保存的对应的第一个控件和第二个控件的位置和按钮序号。
	保存的按钮序号是为了知道是第一个控件的左边按钮连接了第二个控件的右边按钮
	或者是第一个控件的右边按钮连接了第二个控件的左边按钮。
	即对应的m_save_first_form1_button_number和m_save_second_form1_button_number列表(0和1分别对应左边按钮和右边按钮)。
	*/
	QPainter painter(this);
	painter.setPen(Qt::black);
	
		int x_1, y_1;
		int x_2, y_2;
		/*先遍历保存的所有的第一个控件和第二个控件的按钮坐标位置，然后连接画线*/
		for (int i = 0; i < m_save_all_first_form1.size(); i++)
		{
			if (m_save_first_form1_button_number[i] == 1)  //点击的第一个控件的按钮序号是1,即之前点击的第一个按钮是右边按钮
			{                                              //那么连接的第二个控件的按钮应该是左边按钮。
				x_1 = m_save_all_first_form1[i]->pos().x()-1;
				y_1 = m_save_all_first_form1[i]->pos().y() + m_save_all_first_form1[i]->height() / 2;
				x_2 = m_save_all_second_form1[i]->pos().x() + m_save_all_second_form1[i]->width();
				y_2 = m_save_all_second_form1[i]->pos().y() + m_save_all_second_form1[i]->height() / 2;
			//	printf("1	x1: %d, y1:%d\tx2: %d, y2:%d\n", x_1, y_1, x_2,y_2);
				if (y_1 == y_2)
					painter.drawLine(x_1, y_1, x_2, y_2);
				else
				{
					if (x_1 < x_2)
					{
						int half_y = (y_1 + y_2) / 2;
						painter.drawLine(x_1, y_1, x_1, half_y);
						painter.drawLine(x_1, half_y, x_2, half_y);
						painter.drawLine(x_2, half_y, x_2, y_2);
						//printf("%d,%d\n", m_save_first_form1_button_number[i], m_save_second_form1_button_number[i]);
					}
					else
					{
						int half_x = (x_1 + x_2) / 2;
						painter.drawLine(x_1, y_1, half_x, y_1);
						painter.drawLine(half_x, y_1, half_x, y_2);
						painter.drawLine(half_x, y_2, x_2, y_2);
						//printf("%d,%d\n", m_save_first_form1_button_number[i], m_save_second_form1_button_number[i]);
					}
				}
			}
			else if (m_save_first_form1_button_number[i] == 2)
			{
				x_1 = m_save_all_first_form1[i]->pos().x() + m_save_all_first_form1[i]->width();
				y_1 = m_save_all_first_form1[i]->pos().y() + m_save_all_first_form1[i]->height() / 2;
				x_2 = m_save_all_second_form1[i]->pos().x()-1;
				y_2 = m_save_all_second_form1[i]->pos().y() + m_save_all_second_form1[i]->height() / 2;
			//	printf("2	x1: %d, y1:%d\tx2: %d, y2:%d\n", x_1, y_1, x_2, y_2);

				if (y_1 == y_2)
					painter.drawLine(x_1, y_1, x_2, y_2);
				else
				{
					if (x_1 < x_2)
					{
						int half_x = (x_1 + x_2) / 2;
						painter.drawLine(x_1, y_1, half_x, y_1);
						painter.drawLine(half_x, y_1, half_x, y_2);
						painter.drawLine(half_x, y_2, x_2, y_2);
						//printf("%d,%d\n", m_save_first_form1_button_number[project_id][i], m_save_second_form1_button_number[project_id][i]);
					}
					else
					{
						int half_y = (y_1 + y_2) / 2;
						painter.drawLine(x_1, y_1, x_1, half_y);
						painter.drawLine(x_1, half_y, x_2, half_y);
						painter.drawLine(x_2, half_y, x_2, y_2);
						//printf("%d,%d\n", m_save_first_form1_button_number[i], m_save_second_form1_point[i]);
					}
				}
			}
		}
	//保存的控件遍历完成后，开始画已点击但还没保存的控件。
		if (m_first_form1_clicked == 0)  //没有控件按钮被点击，绘画事件到此结束。
			return;
		if (m_mouse_enter_form1_flag == 0) //处理鼠标进入控件内，线的颜色发生变化,0表示鼠标没有进入控件内
		{
			painter.setPen(Qt::blue);
		}
		else                            //鼠标移入到控件内
		{
			if (m_form1_left_button == 1)  //点击的第一个控件的按钮是右边按钮。
			{
				if(base_property_widget::first_widget_can_append_second_widget(m_record_second_form1, m_record_first_form1)==1)
					painter.setPen(Qt::yellow);
			}
			else
			{
				if(base_property_widget::first_widget_can_append_second_widget(m_record_first_form1, m_record_second_form1) == 1)
					painter.setPen(Qt::yellow);
			}
		}
		if (m_first_form1_y_position == m_second_form1_y_position)
			painter.drawLine(m_first_form1_x_position, m_first_form1_y_position, m_second_form1_x_position, m_second_form1_y_position);
		else
		{
			if (m_form1_left_button == 1)   //点击的是第一个按钮
			{
				if (m_first_form1_x_position < m_second_form1_x_position)
				{
					int half_y = (m_first_form1_y_position + m_second_form1_y_position) / 2;
					painter.drawLine(m_first_form1_x_position, m_first_form1_y_position, m_first_form1_x_position, half_y);
					painter.drawLine(m_first_form1_x_position, half_y, m_second_form1_x_position, half_y);
					painter.drawLine(m_second_form1_x_position, half_y, m_second_form1_x_position, m_second_form1_y_position);
				}
				else
				{
					int half_x = (m_first_form1_x_position + m_second_form1_x_position) / 2;
					painter.drawLine(m_first_form1_x_position, m_first_form1_y_position, half_x, m_first_form1_y_position);
					painter.drawLine(half_x, m_first_form1_y_position, half_x, m_second_form1_y_position);
					painter.drawLine(half_x, m_second_form1_y_position, m_second_form1_x_position, m_second_form1_y_position);
				}
			}
			else 
			{
				if (m_first_form1_x_position < m_second_form1_x_position)
				{
					int half_x = (m_first_form1_x_position + m_second_form1_x_position) / 2;
					painter.drawLine(m_first_form1_x_position, m_first_form1_y_position, half_x, m_first_form1_y_position);
					painter.drawLine(half_x, m_first_form1_y_position, half_x, m_second_form1_y_position);
					painter.drawLine(half_x, m_second_form1_y_position, m_second_form1_x_position, m_second_form1_y_position);
				}
				else
				{
					int half_y = (m_first_form1_y_position + m_second_form1_y_position) / 2;
					painter.drawLine(m_first_form1_x_position, m_first_form1_y_position, m_first_form1_x_position, half_y);
					painter.drawLine(m_first_form1_x_position, half_y, m_second_form1_x_position, half_y);
					painter.drawLine(m_second_form1_x_position, half_y, m_second_form1_x_position, m_second_form1_y_position);

				}

			}

		}
}

void myqwidget::delete_current_form_connection_node(base_property_widget* a)
{
	//获取右侧当前页面的id
	for (int i = 0; i < m_save_all_first_form1.size(); i++)
	{
		if (m_save_all_first_form1[i] == a || m_save_all_second_form1[i] == a)
		{
			if (m_save_all_first_form1[i] == a)
			{
				if (m_save_first_form1_button_number[i] == 1)
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(m_save_all_second_form1[i]->id_key, m_save_all_first_form1[i]->id_key);
					m_save_all_first_form1.erase(m_save_all_first_form1.begin() + i);
					m_save_first_form1_button_number.erase(m_save_first_form1_button_number.begin() + i);
					m_save_all_second_form1.erase(m_save_all_second_form1.begin() + i);
					m_save_second_form1_button_number.erase(m_save_second_form1_button_number.begin() + i);
					break;
				}
			}
			else if (m_save_all_second_form1[i] == a)
			{
				if (m_save_second_form1_button_number[i] == 1)
				{
					OperationalLayer::getOperationInstance().removeNodeConnection(m_save_all_first_form1[i]->id_key, m_save_all_second_form1[i]->id_key);
					m_save_all_first_form1.erase(m_save_all_first_form1.begin() + i);
					m_save_first_form1_button_number.erase(m_save_first_form1_button_number.begin() + i);
					m_save_all_second_form1.erase(m_save_all_second_form1.begin() + i);
					m_save_second_form1_button_number.erase(m_save_second_form1_button_number.begin() + i);
					break;
				}
			}
		}
	}
}

void myqwidget::mouseMoveEvent(QMouseEvent *e)
{
	//if (m_first_form1_clicked == 1)
	//{
		m_second_form1_x_position = e->x();
		m_second_form1_y_position = e->y();
		update();
	//}
}

void myqwidget::mousePressEvent(QMouseEvent *event) 
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_first_form1_clicked)  //有按钮被点击
		{
			if (!m_mouse_enter_form1_flag) //鼠标没有进入控件内
			{
				m_first_form1_clicked = 0;
				this->update();
			}
		}
	}
	if (event->button() == Qt::RightButton)
	{
		m_mouse_pressPoint = event->pos();
	}
}

QPoint myqwidget::getMousePoint()
{
	return m_mouse_pressPoint;
}


void myqwidget::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_C))
	{
		if (!m_record_highlight_form1)
			return;
		else
			this->paste_text = m_record_highlight_form1->NodeType;
	}
	else if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_V))
	{
		if (this->paste_text=="")
			return;
		else 
		{
			emit sig_paste_widget(this->paste_text);
		}
	}
	else if ((event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_Delete) || (event->key() == Qt::Key_Delete))
	{
		if (!m_record_highlight_form1)
			return;
		else
			m_record_highlight_form1->delete_self();// slot_delete_self_widget();
	}
}

base_property_widget*  myqwidget::get_form1_by_id(char id)
{
	for (int i = 0; i < m_record_all_form1.size(); i++)
	{
		char key = m_record_all_form1[i]->id_key;
		if (key == id) 
			return  m_record_all_form1[i];
	}
	return nullptr;
}

int myqwidget::get_first_form1_clicked()  //判断是否点击了第一个控件按钮
{
	return m_first_form1_clicked;
}

void myqwidget::set_first_form1_clicked(int data)  
{
	m_first_form1_clicked = data;
}

void myqwidget::set_m_form1_left_button(int data) //设置点击的第一个控件的按钮序号
{
	m_form1_left_button = data;
}

int myqwidget::get_m_form1_left_button()
{
	return m_form1_left_button;
}

void myqwidget::set_m_first_form1_x_position(int data)
{
	m_first_form1_x_position = data;

}

void myqwidget::set_m_first_form1_y_position(int data)
{
	m_first_form1_y_position = data;

}
void myqwidget::set_m_record_first_form1(base_property_widget* form1)
{
	m_record_first_form1 = form1;
}

void myqwidget::set_m_second_form1_x_position(int data)
{
	m_second_form1_x_position = data;
}

void myqwidget::set_m_second_form1_y_position(int data)
{
	m_second_form1_y_position = data;
}

void myqwidget::set_m_record_second_form1(base_property_widget* form1)
{
	m_record_second_form1 = form1;
}

int myqwidget::get_m_save_all_first_form1_size()
{
	return m_save_all_first_form1.size();
}

base_property_widget* myqwidget::get_m_save_all_first_form1(int i)
{
	return m_save_all_first_form1[i];
}

base_property_widget* myqwidget::get_m_save_all_second_form1(int i)
{
	return m_save_all_second_form1[i];
}

base_property_widget* myqwidget::get_m_record_first_form1()
{
	return m_record_first_form1;

}

base_property_widget* myqwidget::get_m_record_second_form1()
{
	return m_record_second_form1;
}

void myqwidget::add_form1_to_m_save_all_first_form1(base_property_widget* form1)
{
	m_save_all_first_form1.push_back(form1);
}

void myqwidget::add_form1_to_m_save_all_second_form1(base_property_widget* form1)
{
	m_save_all_second_form1.push_back(form1);
}

void myqwidget::add_number_to_m_save_first_form1_button_number(int data)
{
	m_save_first_form1_button_number.push_back(data);
}

void myqwidget::add_number_to_m_save_second_form1_button_number(int data)
{
	m_save_second_form1_button_number.push_back(data);
}

base_property_widget* myqwidget::get_m_record_highlight_form1()
{
	return m_record_highlight_form1;
}

void myqwidget::set_m_record_highlight_form1(base_property_widget* form1)
{
	m_record_highlight_form1 = form1;
}

    // yzw 0702 高亮显示的控件 置为普通
void myqwidget::dehighlight_form1()
{
	if (m_record_highlight_form1) {
		QPalette pal = m_record_highlight_form1->palette();
		pal.setColor(QPalette::Background, QColor(255, 255,  255));
		m_record_highlight_form1->setPalette(pal);
		m_record_highlight_form1->setAutoFillBackground(true);
	}
}

void myqwidget::add_form1_button_link(base_property_widget* form1)
{
	if (m_record_highlight_form1)
	{
		if (base_property_widget::first_widget_can_append_second_widget(m_record_highlight_form1, form1))
		{
			add_form1_to_m_save_all_first_form1(m_record_highlight_form1);
			add_form1_to_m_save_all_second_form1(form1);
			add_number_to_m_save_first_form1_button_number(2);
			add_number_to_m_save_second_form1_button_number(1);
			m_record_highlight_form1->m_suff_widget_list.push_back(form1);
			form1->m_pre_widget = m_record_highlight_form1;
			OperationalLayer::getOperationInstance().setNodeConnection(m_record_highlight_form1->id_key, form1->id_key);
			//网格化控件加入，不用管前节点是否有数据，直接加入就行。只有点击网格化控件的开始网格化按钮，才做后续变换
			//if (current_lianluotu->get_m_record_highlight_form1()->data_full)
			//{
				//new1->data_full = true;
				//vtkSmartPointer <vtkProp> pointActor = OperationalLayer::getOperationInstance().getActorFromProject(new1->id_key >> 8);
				//current_project->m_vtkrender->ResetCamera();
			//}
			form1->baseStruct->m_putIn = m_record_highlight_form1->get_label_text();// 0802 
			form1->base_info_to_widget();
		}
	}
	form1->show_highlight(this, form1);
		update();
}

void  myqwidget::set_m_mouse_enter_form1_flag(int data)
{
	m_mouse_enter_form1_flag = data;
}
int myqwidget::get_m_mouse_enter_form1_flag()
{
	return m_mouse_enter_form1_flag;
}

int myqwidget::get_m_record_all_form1_size()
{
	return m_record_all_form1.size();
}

base_property_widget* myqwidget::get_m_record_all_form1(int i)
{
	return m_record_all_form1[i];
}

void myqwidget::add_form1_to_m_record_all_form1(base_property_widget* form1)
{
	m_record_all_form1.push_back(form1);
}

void myqwidget::set_m_save_all_first_form1(int i, base_property_widget* form1)
{
	m_save_all_first_form1[i] = form1;
}

void myqwidget::set_m_save_all_second_form1(int i, base_property_widget* form1)
{
	m_save_all_second_form1[i] = form1;
}

void myqwidget::clear_m_save_all_first_form1()
{
	m_save_all_first_form1.clear();
}

void myqwidget::clear_m_save_all_second_form1()
{
	m_save_all_second_form1.clear();

}

void  myqwidget::clear_m_save_first_form1_button_number()
{
	m_save_first_form1_button_number.clear();
}

void  myqwidget::clear_m_save_second_form1_button_number()
{
	m_save_second_form1_button_number.clear();
}

void myqwidget::delete_ith_element_from_first_form1(int i)
{
	m_save_all_first_form1.erase(m_save_all_first_form1.begin() + i);
}

void myqwidget::delete_ith_element_from_second_form1(int i)
{
	m_save_all_second_form1.erase(m_save_all_second_form1.begin() + i);
}

void myqwidget::delete_ith_element_from_first_number(int i)
{
	m_save_first_form1_button_number.erase(m_save_first_form1_button_number.begin() + i);
}

void myqwidget::delete_ith_element_from_second_number(int i)
{
	m_save_second_form1_button_number.erase(m_save_second_form1_button_number.begin() + i);
}

int myqwidget::get_m_save_first_form1_button_number(int i)
{
	return m_save_first_form1_button_number[i];
}

int myqwidget::get_m_save_second_form1_button_number(int i)
{
	return m_save_second_form1_button_number[i];
}

void myqwidget::delete_ith_element_form_all_form1(int i)
{
	m_record_all_form1.erase(m_record_all_form1.begin() + i);
}

void myqwidget::set_paste_flag(bool a)
{
	paste_flag = a;
}

void myqwidget::contextMenuEvent(QContextMenuEvent  *event) {
	QMenu   *menu = new QMenu(this);
	menu->addAction(STRING_ITEM_OPEN_SOURCE, this,SLOT(slot_load_data()));
	paste_action = new QAction(this);
	paste_action->setText(STRING_ITEM_PASTE);
	menu->addAction(paste_action);
	//if (!paste_flag)
	{
		std::shared_ptr<TiXmlNode> nx= OperationalLayer::getCopyNodeXml();
		paste_action->setEnabled(nx.get());
	}
	//else
	{
		//paste_action->setEnabled(true);
	}
	connect(paste_action, SIGNAL(triggered()), this, SLOT(slot_paste_widget()));
	menu->addSeparator();
	menu->addAction(STRING_ITEM_TEXT, this, SLOT(slot_text_widget()));
	menu->addAction(STRING_ITEM_COMMENT, this, SLOT(slot_comment_widget()));
	menu->move(cursor().pos());
	menu->show();
}

void myqwidget::set_paste_text(std::string a)
{
	paste_text = a;
}

void myqwidget::slot_load_data()
{
	emit sig_load_data();
}

void myqwidget::slot_paste_widget()
{
	emit sig_paste_widget(this->paste_text);
}

void myqwidget::slot_text_widget()
{
	emit sig_text_widget();
}

void myqwidget::slot_comment_widget()
{
	emit sig_comment_widget();
}


void myqwidget::send_signal_show_property_manager_message(QString a, base_property_widget *b)
{
	emit  sig_show_property_manager_message(a, b);
}

void myqwidget::send_signal_show_status_message(QString a)
{
	emit sig_show_status_mess(a);
}

void myqwidget::paint_widget(base_property_widget *a, int left_x, int left_y, int length, int width)
{
	a->setParent(this);
	a->setGeometry(left_x, left_y, length, width);
	a->show();
}

// yzw 0704
void myqwidget::paint_widget(base_property_widget* a, modelInfo mi)
{
	a->setParent(this);
	a->setGeometry(mi.m_leftTopPosX , mi.m_leftTopPosY, mi.m_length, mi.m_width);
	a->show();
}

int myqwidget::get_widget_name_num(std::string a)
{
	int num = 0;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] >= '0' && a[i] <= '9')
		{
			num = num * 10 + a[i] - '0';
		}
	}
	if (num == 0)
		return 1;
	return num;
}

int myqwidget::return_widget_type_num(std::string a)
{
	int num = 0;
	std::vector<int>temp_vector;	// yzw 0615 此类名称中的数字
	for (int i = 0; i < m_record_all_form1.size(); i++)
	{
		if (m_record_all_form1[i]->NodeType == a)
		{
			std::string name = m_record_all_form1[i]->get_label_text();
			temp_vector.push_back(get_widget_name_num(name));
			num++;
		}
	}
	if (!num)
		return 0;
	sort(temp_vector.begin(), temp_vector.end());
	num = 0;		// yzw 0615 找到未被使用的数字
	while (num < temp_vector.size())
	{
		if (temp_vector[num] == num+1)
		{
			num++;
			continue;
		}
		else
		{
			break;//return num;
		}
	}
	//if (num == temp_vector.size())
		return num;
}

void myqwidget::send_sig_manager_property_null()
{
	emit sig_manager_property_null();
}

myqwidget::~myqwidget()
{

}
