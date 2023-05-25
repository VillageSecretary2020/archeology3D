#pragma once
#include "base_property_widget.h"

class subset_property_widget:public base_property_widget  // 子集  类
{
	Q_OBJECT
public:
	subset_property_widget::subset_property_widget(int num, SuperNodeStruct* param=nullptr);
	~subset_property_widget();

	SuperNodeStruct* return_property_struct();
	virtual void delete_self();
	void showInputResolution(int resolution[3]);
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);

	void m_update_widget_property();
private:
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_subset_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_resolution_treeitem;
	QTreeWidgetItem *m_output_component_treeitem;
	QTreeWidgetItem *m_data_component_treeitem;
	QLineEdit *m_input_resolution_edit;

	QTreeWidget *m_geometry;
	QTreeWidgetItem *m_geometry_treeitem;
	QTreeWidgetItem *m_input_calculate_treeitem;
	QTreeWidgetItem *m_x_limit_treeitem;
	QTreeWidgetItem *m_x_start_node_treeitem;
	QTreeWidgetItem *m_x_end_node_treeitem;
	QTreeWidgetItem *m_x_step_treeitem;
	QTreeWidgetItem *m_y_limit_treeitem;
	QTreeWidgetItem *m_y_start_node_treeitem;
	QTreeWidgetItem *m_y_end_node_treeitem;
	QTreeWidgetItem *m_y_step_treeitem;
	QTreeWidgetItem *m_z_limit_treeitem;
	QTreeWidgetItem *m_z_start_node_treeitem;
	QTreeWidgetItem *m_z_end_node_treeitem;
	QTreeWidgetItem *m_z_step_treeitem;
	QCheckBox *m_input_calculate_check;
	QSpinBox *m_x_start_node_spinbox;
	QSpinBox *m_x_end_node_spinbox;
	QSpinBox *m_x_step_spinbox;
	QSpinBox *m_y_start_node_spinbox;
	QSpinBox *m_y_end_node_spinbox;
	QSpinBox *m_y_step_spinbox;
	QSpinBox *m_z_start_node_spinbox;
	QSpinBox *m_z_end_node_spinbox;
	QSpinBox *m_z_step_spinbox;
signals:
	void sig_property_manager_dock_null();
	void sig_subset_to_axis();
	void sig_subset_to_border();
	void sig_delete_ubset();
public slots:
private slots:
	void slot_update_widget_property();
	void slot_subset_to_axis();
	void slot_subset_to_border();
	void slot_input_calculate();
	void slot_x_start_node();
	void slot_x_end_node();
	void slot_x_step();
	void slot_y_start_node();
	void slot_y_end_node();
	void slot_y_step();
	void slot_z_start_node();
	void slot_z_end_node();
	void slot_z_step();
};
