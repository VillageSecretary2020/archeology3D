#pragma once
#include "base_property_widget.h"
#include"color_scale_property.h"

class surface_render_property_widget:public base_property_widget   //表面渲染  类
{
	Q_OBJECT
public:
	surface_render_property_widget(int num,G_SurfaceRender* surface_render=nullptr);
	~surface_render_property_widget();
	G_SurfaceRender *return_surface_render_property_struct();
	virtual void delete_self();
	void m_update_widget_property();
	static void xyz_range(int*a, void*p);
	static void update_property(int num_blocks, float average_blocks, float volume, void* widget);
private:
	void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	G_SurfaceRender *m_return_surface_render_struct = nullptr;
	custom_button_left *m_btn_left;
	//custom_button_right *m_btn_right;
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_surface_render_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_component_treeitem;
	QTreeWidgetItem *m_block_number_treeitem;
	QTreeWidgetItem *m_average_block_treeitem;
	QTreeWidgetItem *m_calculate_volume_treeitem;
	QTreeWidgetItem *m_volume_treeitem;
	QLineEdit *m_volume_edit;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_show_border_treeitem;
	QTreeWidgetItem *m_border_color_treeitem;
	QTreeWidgetItem *m_gradient_chro_treeitem;
	QLineEdit *m_block_number_edit;
	QLineEdit *m_average_block_edit;
	QCheckBox *m_calculate_volume;
	QCheckBox *m_show_border;
	QComboBox *m_border_color;
	QComboBox *m_gradient_chro;
	QTreeWidget *m_geometry_property;
	QTreeWidgetItem *m_geometry_property_treeitem;
	QTreeWidgetItem *m_x_coordinate_limit_treeitem;
	QTreeWidgetItem *m_x_start_node_treeitem;
	QTreeWidgetItem *m_x_end_node_treeitem;
	QTreeWidgetItem *m_x_single_slice_treeitem;
	QTreeWidgetItem *m_y_coordinate_limit_treeitem;
	QTreeWidgetItem *m_y_start_node_treeitem;
	QTreeWidgetItem *m_y_end_node_treeitem;
	QTreeWidgetItem *m_y_single_slice_treeitem;
	QTreeWidgetItem *m_z_coordinate_limit_treeitem;
	QTreeWidgetItem *m_z_start_node_treeitem;
	QTreeWidgetItem *m_z_end_node_treeitem;
	QTreeWidgetItem *m_z_single_slice_treeitem;
	QSpinBox *m_x_start_node_spin;
	QSlider *m_x_start_node_slider;
	QSpinBox *m_x_end_node_spin;
	QSlider *m_x_end_node_slider;
	QCheckBox *m_x_single_slice;
	QSpinBox *m_y_start_node_spin;
	QSlider *m_y_start_node_slider;
	QSpinBox *m_y_end_node_spin;
	QSlider *m_y_end_node_slider;
	QCheckBox *m_y_single_slice;
	QSpinBox *m_z_start_node_spin;
	QSlider *m_z_start_node_slider;
	QSpinBox *m_z_end_node_spin;
	QSlider *m_z_end_node_slider;
	QCheckBox *m_z_single_slice;
	color_scale_property *m_color_scale;
signals:
	void sig_property_manager_dock_null();
	void sig_delete_surface_render();
public slots:
private slots:
	void slot_color_scale_property_changed(Child_ColorBar*, int type);
	void slot_block_number_edit();
	void slot_average_block_edit();
	void slot_calculate_volume();
	void slot_show_border();
	void slot_border_color();
	void slot_gradient_color();
	void slot_x_start_slider(int);
	void slot_x_start_spin(int);
	void slot_x_end_slider(int);
	void slot_x_end_spin(int);
	void slot_x_single_slice();
	void slot_y_start_slider(int);
	void slot_y_start_spin(int);
	void slot_y_end_slider(int);
	void slot_y_end_spin(int);
	void slot_y_single_slice();
	void slot_z_start_slider(int);
	void slot_z_start_spin(int);
	void slot_z_end_slider(int);
	void slot_z_end_spin(int);
	void slot_z_single_slice();
};
