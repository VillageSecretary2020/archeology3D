// shape_render_property_widget.h

#pragma once
#include "base_property_widget.h"
#include "color_scale_property.h"

class shape_render_property_widget:public base_property_widget   //形体渲染  类
{
	Q_OBJECT
public:
	shape_render_property_widget(int num,G_VolumeRender *volume=nullptr);
	~shape_render_property_widget();
	G_VolumeRender *return_shape_render_struct();
	virtual void delete_self();
private:
	G_VolumeRender *m_return_shape_render_property_struct;
	//Child2DTexture two_texture;
	//Child3DTexture three_texture;
	void m_update_widget_property();
	void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	void slot_shape_to_axis();
	void slot_shape_to_border();
private:
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_shape_render_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_component_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_render_method_treeitem;
	//QTreeWidgetItem *m_slice_number_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QTreeWidgetItem *m_interpolation_treeitem;
	QTreeWidgetItem *m_render_mode_treeitem;
	QTreeWidgetItem *m_sample_distance_treeitem;
	QTreeWidgetItem *m_averange_range_treeitem;
	QTreeWidgetItem *m_color_type_treeitem;
	QTreeWidgetItem *m_gradient_chro_treeitem;
	QTreeWidgetItem *m_opacity_treeitem;
	//QComboBox *m_render_method;
	QComboBox *m_composition_color_method;
	QComboBox *m_interpolation_method;
	QComboBox *m_render_mode_combox;
	QDoubleSpinBox *m_sample_distance_spin;
	QLineEdit *m_range_0; QLineEdit *m_range_1;
	QComboBox *m_color_type_combox;
	QComboBox *m_gradient_combox;
	QDoubleSpinBox *m_opacity_spinbox;
	QSlider *m_opacity_slider;
	color_scale_property *m_color_scale;
	//QSpinBox *m_slice_number_spin;
	//QSlider *m_slice_number_slider;

signals:
	void sig_property_manager_dock_null();
	void sig_shape_to_axis();
	void sig_shape_to_border();
	void sig_delete_shape_render();	// yzw 0526

private slots:
	void slot_render_mode();
	void slot_sample_distance();
	void slot_range_min();
	void slot_range_max();
	void slot_colot_type();

	void slot_color_scale_property_changed(Child_ColorBar*, int type);

	void slot_composition_color_method();
	void slot_interpolation_method();
	void m_opacity_slider_setValue(double);
	void m_opacity_spin_setValue(int);
	void slot_show_color_scale(int);
	void slot_place_direction(int);
	void slot_m_x_position_slider(double);
	void slot_m_x_position_spin(int);
	void slot_m_y_position_slider(double);
	void slot_m_y_position_spin(int);
	void slot_m_width_slider(double);
	void slot_m_width_spin(int);
	void slot_m_length_slider(double);
	void slot_m_length_spin(int);
	void slot_m_font_size_slider(int);
	void slot_m_font_size_spin(int);
	void slot_m_note_number(int);
	void slot_m_note_font_size_slider(int);
	void slot_m_note_font_size_spin(int);
	void slot_m_adopt_self_note(int);
	void slot_color_type(int);
	void slot_significant(int);
	void slot_gradient_combox();
};
