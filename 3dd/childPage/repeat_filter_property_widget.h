#pragma once
#include "base_property_widget.h"

class repeat_filter_property_widget:public base_property_widget  //  重复过滤器	 类
{
	Q_OBJECT
public:
	repeat_filter_property_widget(int num, C_RepeatFilter* repeat_filter = nullptr);
	~repeat_filter_property_widget();
	C_RepeatFilter* return_repeat_filter_property_struct();
	virtual void delete_self();
private:
	custom_button_left *m_btn_left;
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	void m_update_widget_property();

private:
	C_RepeatFilter* m_return_repeat_filter_struct;
	QTreeWidget *m_normal_tree;
	QTreeWidgetItem *m_repeat_filter_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_data_error_treeitem;
	QTreeWidgetItem *m_x_error_treeitem;
	QTreeWidgetItem *m_y_error_treeitem;
	QTreeWidgetItem *m_z_error_treeitem;
	QLineEdit *m_x_error_edit;
	QLineEdit *m_y_error_edit;
	QLineEdit *m_z_error_edit;
signals:
	void sig_property_manager_dock_null();
	void sig_repeat_filter_to_axis();
	void sig_repeat_filter_to_border();
	void sig_repeat_filter_to_scatter();
	void sig_repeat_filter_to_transform();
	void sig_repeat_filter_to_exclusion_filter();
	void sig_repeat_filter_to_gridding();
	void sig_repeat_filter_to_repeat_filter();
	void sig_delete_repeat();
public slots:
private slots:
	void slot_repeat_filter_to_axis();
	void slot_repeat_filter_to_border();
	void slot_repeat_filter_to_scatter();
	void slot_repeat_filter_to_transform();
	void slot_repeat_filter_to_exclusion_filter();
	void slot_repeat_filter_to_gridding();
	void slot_repeat_filter_to_repeat_filter();
	void slot_x_error();
	void slot_y_error();
	void slot_z_error();

};

