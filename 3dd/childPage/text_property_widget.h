#pragma once
#include "base_property_widget.h"

class text_property_widget:public base_property_widget  //文本属性界面
{
	Q_OBJECT
public:
	text_property_widget(int num,G_Text* text=nullptr);
	~text_property_widget();
	G_Text* return_text_property_struct();
	virtual void delete_self();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	void m_update_widget_property();
private:
	QTreeWidget *m_normal_property; 
	QTreeWidgetItem *m_text;
	QTreeWidgetItem *m_text_treeitem;
	QTreeWidgetItem *m_font_treeitem;
	QTreeWidgetItem *m_font_size_treeitem;
	QTreeWidgetItem *m_color_treeitem;
	QTreeWidgetItem *m_position_treeitem;
	QTreeWidgetItem *m_x_treeitem;
	QTreeWidgetItem *m_y_treeitem;
	QTreeWidgetItem *m_z_treeitem;
	QLineEdit *m_text_edit;
	QComboBox *m_font;  //字体
	QSpinBox *m_font_size_spinbox;  //字号大小文本框
	QSlider *m_font_size_slider;  //字号大小滑动条
	QComboBox *m_color;
	QDoubleSpinBox *m_x_position_spin;
	QDoubleSpinBox *m_y_position_spin;
	QDoubleSpinBox *m_z_position_spin;
	G_Text *m_return_text_property;
signals:
	void sig_property_manager_dock_null();
	void sig_text_to_axis();
	void sig_text_to_border();
	void sig_new_project_update();
	void sig_delete_text();
public slots:
private slots:
	void slot_text_to_axis();
	void slot_text_to_border();
	void slot_text_edit();
	void slot_font();
	void slot_font_size_slider(int);
	void slot_font_size_spin(int);
	void slot_text_color();
	void slot_x_position_spin();
	//void slot_y_position_spin();
	//void slot_z_position_spin();
};
