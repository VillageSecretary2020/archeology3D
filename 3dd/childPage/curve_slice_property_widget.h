// curve_slice_property_widget.h

#pragma once
#include "base_property_widget.h"
#include "cut_plane_property.h"
#include "color_scale_property.h"

class curve_slice_property_widget:public base_property_widget  //切片图  类
{
    Q_OBJECT
public:
	curve_slice_property_widget(int num, SuperNodeStruct* slice=NULL);
	~curve_slice_property_widget();
public:
	SuperNodeStruct *return_slice_property_struct();
	void setCutPlaneRange(float* dataRange);
	void setCutPoint(double* dataRange);
	virtual void delete_self();
private:
	//custom_button_left *m_btn_left;
	//custom_button_right *m_btn_right;
	G_Curve_Slice *m_return_slice_property_struct;
	struct Child_CutingPlane m_cut_plane_struct;
	struct Child_ColorBar m_color_scale_struct;
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	void slot_slice_to_axis();
	void slot_slice_to_border();
private:
	//常规属性
	QTreeWidget *m_normal_property;
	//QTreeWidgetItem *m_slice_treeitem;
	//QTreeWidgetItem *m_input_treeitem;	// 0802 移至base_property_widget内
	QTreeWidgetItem *m_textureFile_treeitem, *m_texturePath_treeitem;
	QLineEdit *m_textureFile_edit;
	QPushButton*m_file_button;

	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_lighting_treeitem;
	QTreeWidgetItem *m_opacity_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QCheckBox *m_lighting;
	QDoubleSpinBox *m_opacity_spin;
	QSlider *m_opacity_slider;
	//添加一个滑动条是否按下的标志判断
	bool m_sliderPressed;//主要是解决滑动条滑动时槽函数重复调用问题
	QComboBox *m_color_method_combox;
	//切割平面
	cut_plane_property *m_cut_plane;
	//色标刻度尺
	color_scale_property *m_color_scale;

signals:
	void sig_property_manager_dock_null();
	void sig_slice_to_axis();
	void sig_slice_to_border();
	//void sig_vtk_camera_render();

	void sig_delete_curve_slice();
public slots:
 
 
private slots:
	void m_update_widget_property();
	virtual void slot_cut_plane_property_changed(Child_CutingPlane*, int type);
	void slot_color_scale_property_changed(Child_ColorBar*, int type);
	void slot_select_file();

	void slot_lighting();
	void m_slider_setValue(double a);
	void m_spin_setValue(int a);
	void slot_color_method();
	void slot_place_direction();
	void slot_x_normal();
	void slot_y_normal();
	void slot_z_normal();
	void slot_offset_from_center_slider();
	void slot_offset_from_center_spin(int);

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

	void slot_slider_pressed();
	void slot_slider_relesed();
	void slot_offset_center_slider_pressed();
	void slot_offset_center_slider_relesed();
};

