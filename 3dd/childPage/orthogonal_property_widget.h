// orthogonal_property_widget.h

#pragma once
#include "base_property_widget.h"
#include "color_scale_property.h"

class orthogonal_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	orthogonal_property_widget(int num,G_Orthogonal_Factor* orthogonal=nullptr);
	~orthogonal_property_widget();
	void setResolution(int * slt);
	G_Orthogonal_Factor* return_orthogonal_struct();
	virtual void delete_self();
private:
	void place_direction();
	void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	G_Orthogonal_Factor *m_return_orthogonal_struct;
	custom_button_left *m_btn_left;
	//custom_button_right *m_btn_right;
	QTreeWidget* m_NormalProperty;
	QTreeWidgetItem *m_orthogonal_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_component_treeitem;
	QTreeWidgetItem *m_place_direction_treeitem;
	QTreeWidgetItem *m_slice_number_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	//QTreeWidgetItem *m_lighting_treeitem;
	//QTreeWidgetItem *m_opacity_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QLineEdit *m_input_component_edit;
	QComboBox* m_PlaceDirection;
	QSpinBox *m_slice_number_spin;
	QSlider *m_slice_number_slider;
	//QCheckBox* m_Lighting;
	//QDoubleSpinBox* m_Opacity_spin;
	//QSlider* m_Opacity_slider;
	QComboBox *m_color_method_combox;
	color_scale_property *m_color_scale;
	void m_update_widget_property();
signals:
	void sig_delete_orthogonal(); // yzw 0610
	void sig_property_manager_dock_null();
public slots:
private slots:
	void slot_color_scale_property_changed(Child_ColorBar*, int type);
	void slot_input_component();
	void slot_place_direction();
	void slot_slice_number_slider(int);
	void slot_slice_number_spin(int);
	//void slot_lighting();
	//void m_opacity_slider_setValue(double);
	//void m_opacity_spin_setValue(int);
	void slot_color_method();
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
	void slot_m_note_font_size_slider(double);
	void slot_m_note_font_size_spin(int);
	void slot_m_adopt_self_note(int);
	void slot_color_type(int);
	void slot_significant(int);
private:
	int resolution[3]; //解析度
};

