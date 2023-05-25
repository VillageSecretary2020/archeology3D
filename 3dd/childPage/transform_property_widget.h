#pragma once
#include "base_property_widget.h"

class transform_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	transform_property_widget(int,C_Conversion* a=nullptr);
	~transform_property_widget();

	virtual void delete_self();
	C_Conversion *return_transform_property_struct();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	C_Conversion *m_return_transform_struct;
	void m_update_widget_property();
private:
	//常规属性
	QTreeWidget *m_normal_tree;
	QTreeWidgetItem *m_translate_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_origin_treeitem;
	QComboBox *m_origin_combox;
	QTreeWidgetItem *m_input_x_treeitem;
	QTreeWidgetItem *m_input_y_treeitem;
	QTreeWidgetItem *m_input_z_treeitem;
	QLineEdit *m_translale_x_edit;
	QLineEdit *m_translale_y_edit;
	QLineEdit *m_translale_z_edit;
	//变换属性
	QTreeWidget *m_transform_tree;
	QTreeWidgetItem *m_scale_factor_treeitem;
	QTreeWidgetItem *m_scale_x_treeitem;
	QTreeWidgetItem *m_scale_y_treeitem;
	QTreeWidgetItem *m_scale_z_treeitem;
	QLineEdit *m_scale_x_edit;
	QLineEdit *m_scale_y_edit;
	QLineEdit *m_scale_z_edit;

	QTreeWidgetItem *m_rotate_treeitem;
	QTreeWidgetItem *m_rotate_x_treeitem;
	QTreeWidgetItem *m_rotate_y_treeitem;
	QTreeWidgetItem *m_rotate_z_treeitem;
	QLineEdit *m_rotate_x_edit;
	QLineEdit *m_rotate_y_edit;
	QLineEdit *m_rotate_z_edit;

	QTreeWidgetItem *m_angle_treeitem;
	QDoubleSpinBox *m_angle_spin;
	QSlider *m_angle_slider;

	QTreeWidgetItem *m_transform_treeitem;
	QTreeWidgetItem *m_transform_x_treeitem;
	QTreeWidgetItem *m_transform_y_treeitem;
	QTreeWidgetItem *m_transform_z_treeitem;
	QLineEdit *m_transform_x_edit;
	QLineEdit *m_transform_y_edit;
	QLineEdit *m_transform_z_edit;

signals:
	void sig_property_manager_dock_null();
	void sig_transform_to_slice();
	void sig_transform_to_shape_render();
	void sig_transform_to_scatter();
	void sig_transform_to_orthogonal();
	void sig_transform_to_isoline();
	void sig_transform_to_isosurface();
	void sig_transform_to_surface();
	void sig_transform_to_axis();
	void sig_transform_to_border();
	void sig_transform_to_transform();
	void sig_transform_to_subset();
	void sig_transform_to_exclusion_filter();
	void sig_transform_to_gridding();
	void sig_transform_to_repeat_filter();
	void sig_delete_transform();
private slots:
	void slot_transform_to_slice();
	void slot_transform_to_shape_render();
	void slot_transform_to_scatter();
	void slot_transform_to_orthogonal();
	void slot_transform_to_isoline();
	void slot_transform_to_isosurface();
	void slot_transform_to_surface();
	void slot_transform_to_axis();
	void slot_transform_to_border();
	void slot_transform_to_transform();
	void slot_transform_to_subset();
	void slot_transform_to_exclusion_filter();
	void slot_create_gridding();
	void slot_create_repeat_filter();
	void slot_origin_combox();
	void slot_scale_x();
	void slot_scale_y();
	void slot_scale_z();
	void slot_rotate_x();
	void slot_rotate_y();
	void slot_rotate_z();
	void slot_angle_slider(double);
	void slot_angle_spin(int);
	void slot_transform_x();
	void slot_transform_y();
	void slot_transform_z();
};
