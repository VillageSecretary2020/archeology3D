#pragma once
#include "base_property_widget.h"

class exclusion_filter_property_widget:public base_property_widget  //排除过滤器  类
{
	Q_OBJECT
public:
	exclusion_filter_property_widget(int num, C_EliminationFiler* m_exclusion_filter=nullptr);
	~exclusion_filter_property_widget();
	C_EliminationFiler* return_exclusion_filter_property_struct();
	virtual void delete_self();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	C_EliminationFiler* m_return_exclusion_filter_struct;
	void m_update_widget_property();
private:
	QTreeWidget* m_normal_tree;
	QTreeWidgetItem* m_exclusion_filter_treeitem;
	//	QTreeWidgetItem* m_input_treeitem;
	QTreeWidgetItem* m_filter_str_treeitem;
	QLineEdit *m_filter_str_edit;
signals:
	void sig_property_manager_dock_null();
	void sig_exclusion_filter_to_axis();
	void sig_exclusion_filter_to_border();
	void sig_exclusion_filter_to_scatter();
	void sig_exclusion_filter_to_transform();
	void sig_exclusion_filter_to_exclusion_filter();
	void sig_exclusion_filter_to_gridding();
	void sig_exclusion_filter_to_repeat_filter();
	void sig_delete_exclusion();
public slots:
private slots:
	void slot_exclusion_filter_to_axis();
	void slot_exclusion_filter_to_border();
	void slot_exclusion_filter_to_scatter();
	void slot_exclusion_filter_to_transform();
	void slot_exclusion_filter_to_exclusion_filter();
	void slot_exclusion_filter_to_gridding();
	void slot_exclusion_filter_to_repeat_filter();
	void slot_filter_str();
};
