#pragma once
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QVector>
#include <cmath>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <memory>
#include <QKeyEvent>
#include "Nodeparaters.h"

/*
这是联络图类，联络图上的控件都显示在此上面。

*/
class base_property_widget;
class myqwidget:public QWidget
{
	Q_OBJECT
public:
	explicit myqwidget(QWidget *parent = nullptr);
	~myqwidget();
	QPoint getMousePoint();

private:
	void mouseMoveEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *event);
	void contextMenuEvent(QContextMenuEvent *);
	void mousePressEvent(QMouseEvent *);
	QPoint m_mouse_pressPoint;  //记录鼠标按下的位置坐标，用于创建时控件摆放的位置  
	int m_first_form1_clicked = 0;  //记录第一个控件是否点击，默认为0，点击为1
	int m_first_form1_x_position, m_first_form1_y_position;  //记录第一个控件的x和y坐标
	int m_second_form1_x_position, m_second_form1_y_position; //记录第二个控件的x和y坐标 
	int m_mouse_enter_form1_flag = 0;  //鼠标移入控件标志，默认为0，鼠标移入控件内为1
	int m_form1_left_button = 0;   //按钮标志位，即第几个按钮被按下，左边按钮
	int m_form1_right_button = 0;   //按钮标志位，右边按钮
	QVector<QPoint> m_save_first_form1_point;//保存第一个控件的点坐标   
	QVector<QPoint> m_save_second_form1_point;//保存第二个控件的点坐标 
	QVector<base_property_widget *> m_save_all_first_form1;//保存记录所有的第一个控件 
	QVector<base_property_widget *> m_save_all_second_form1;//保存记录所有的第二个控件 
	QVector<base_property_widget *> m_record_all_form1;    //记录界面上所有控件 
	QVector<int> m_save_first_form1_button_number;  //保存第一个控件的按钮的序号 
	QVector<int> m_save_second_form1_button_number;  //保存第二个控件的按钮的序号
	base_property_widget *m_record_first_form1 = nullptr;   //记录第一个控件
	base_property_widget *m_record_second_form1 = nullptr;    //记录第二个控件
	base_property_widget *m_record_highlight_form1 = nullptr;  //记录界面上高亮显示的控件
	bool paste_flag = false;  //粘贴选项是否可用
	std::string paste_text="";
	QAction *paste_action;
public:
	void set_paste_flag(bool a);
	void set_paste_text(std::string);
	void send_signal_show_property_manager_message(QString,base_property_widget *);
	void send_signal_show_status_message(QString a);
	void delete_current_form_connection_node(base_property_widget *);
	int get_first_form1_clicked();//根据返回值来判断是否按下第一个按钮,0表示未按下，1按下。
	void set_first_form1_clicked(int);
	void set_m_form1_left_button(int data);
	int get_m_form1_left_button();
	void set_m_first_form1_x_position(int data);
	void set_m_first_form1_y_position(int data);
	void set_m_record_first_form1(base_property_widget *);
	void set_m_second_form1_x_position(int);
	void set_m_second_form1_y_position(int);
	void set_m_record_second_form1(base_property_widget *);
	int get_m_save_all_first_form1_size();
	base_property_widget*  get_form1_by_id(char id);// 由id取Form1
	base_property_widget *get_m_save_all_first_form1(int i); //得到m_save_all_first_form1中第i个Form1控件
	base_property_widget *get_m_save_all_second_form1(int); //同上
	base_property_widget *get_m_record_first_form1();  //返回m_record_first_form1控件
	base_property_widget *get_m_record_second_form1();  //返回m_record_second_form1控件
	int get_m_save_first_form1_button_number(int i);
	int get_m_save_second_form1_button_number(int i);
	void add_form1_to_m_save_all_first_form1(base_property_widget *);
	void add_form1_to_m_save_all_second_form1(base_property_widget *);
	void add_number_to_m_save_first_form1_button_number(int data);
	void add_number_to_m_save_second_form1_button_number(int data);
	base_property_widget *get_m_record_highlight_form1();  //返回界面上高亮显示的控件 
	void set_m_record_highlight_form1(base_property_widget *);     //设置界面上高亮显示的控件
	void dehighlight_form1();     // yzw 0702 高亮显示的控件 置为普通
	void add_form1_button_link(base_property_widget* form1);     // yzw 0706 优化按钮连接线关系
	void set_m_mouse_enter_form1_flag(int);
	int get_m_mouse_enter_form1_flag();
	int get_m_record_all_form1_size();   //返回界面上所有控件个数
	base_property_widget *get_m_record_all_form1(int);    //返回界面上第i个控件
	void add_form1_to_m_record_all_form1(base_property_widget *);  //添加界面控件
	void set_m_save_all_first_form1(int, base_property_widget *);
	void set_m_save_all_second_form1(int,base_property_widget *);
	void clear_m_save_all_first_form1();    //清空m_save_all_first_form1
	void clear_m_save_all_second_form1();   //清空 m_save_all_second_form1
	void clear_m_save_first_form1_button_number();   //清空m_save_first_form1_button_number;
	void clear_m_save_second_form1_button_number();    //同上;
	void paint_widget(base_property_widget*, int left_x, int left_y, int length, int width);
	void paint_widget(base_property_widget*, modelInfo mi);		// yzw 0704
	void delete_ith_element_from_first_form1(int i);  //从first_form1的vector中删除第i个元素
	void delete_ith_element_from_second_form1(int i);  //从second_form1的vector中删除第i个元素
	void delete_ith_element_from_first_number(int i);  //从first_nunber的vector中删除第i个元素
	void delete_ith_element_from_second_number(int i);  //从second_nunber的vector中删除第i个元素
	void delete_ith_element_form_all_form1(int i);
	int return_widget_type_num(std::string);
	int get_widget_name_num(std::string);
	void send_sig_manager_property_null();

signals:
	void sig_show_property_manager_message(QString,base_property_widget *);
	void sig_show_status_mess(QString a);
	void sig_load_data();
	void sig_paste_widget(std::string);
	void sig_text_widget();
	void sig_comment_widget();
	void sig_manager_property_null();
private slots:
	void slot_load_data();
	void slot_paste_widget();
	void slot_text_widget();
	void slot_comment_widget();
};

