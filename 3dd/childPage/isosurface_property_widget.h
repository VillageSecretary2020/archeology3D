#pragma once
#include "base_property_widget.h"
#include "color_scale_property.h"

class isosurface_property_widget:public base_property_widget  //等值面图 类
{
	Q_OBJECT
public:
	isosurface_property_widget(int num,G_IsoSurface* surface=nullptr);
	~isosurface_property_widget();
	G_IsoSurface* return_isosurface_property_struct();
	virtual void delete_self();
	void m_update_widget_property();
	void setScalarRange(double* dataRange);
	void displayVolume(double v1, double v2);
private:
	void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	G_IsoSurface *m_return_isosurface_property_struct=nullptr;
	custom_button_left *m_btn_left;
	//custom_button_right *m_btn_right;
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_isosurface_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_component_treeitem;
	QTreeWidgetItem *m_critical_value_treeitem;
	QTreeWidgetItem *m_calculate_volume_treeitem;
	QTreeWidgetItem *m_volume_bigger_critical_treeitem;
	QTreeWidgetItem *m_volume_smaller_critical_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_style_sheet_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QTreeWidgetItem *m_gradient_color_treeitem;
	QTreeWidgetItem *m_render_color_treeitem;
	QTreeWidgetItem *m_material_treeitem;
	QTreeWidgetItem *m_highlight_color_treeitem;
	QTreeWidgetItem *m_highlight_intense_treeitem;
	QTreeWidgetItem *m_gloss_treeitem;
	QTreeWidgetItem *m_opacity_treeitem;
	QLineEdit *m_critical_edit;
	QSlider *m_critical_slider;
	QCheckBox *m_calculate_check;
	QLineEdit *m_volume_bigger_edit;
	QLineEdit *m_volume_smaller_edit;
	QComboBox *m_color_method_combox;
	QComboBox *m_gradient_color_combox;
	QComboBox *m_render_color_combox;
	QComboBox *m_highlight_color_combox;
	QLineEdit *m_highlight_intense_edit;
	QSlider *m_highlight_intense_slider;
	QLineEdit *m_gloss_edit;
	QSlider *m_gloss_slider;
	QLineEdit *m_opacity_edit;
	QSlider *m_opacity_slider;
	color_scale_property *m_color_scale;

	bool m_sliderPressed;//主要是解决滑动条滑动时槽函数重复调用问题
signals:
	void sig_property_manager_dock_null();
	void sig_vtk_interactor_render();
	void  sig_delete_isosurface();	// yzw 0610
public slots:
private slots:
	void slot_color_scale_property_changed(Child_ColorBar*, int type);
	void slot_critical_edit();
	void slot_critical_slider(int);
	void slot_critical_slider_pressed();
	void slot_critical_slider_relesed();
	void slot_calculate_volume();
	void slot_color_method();
	void slot_gradient_color();
	void slot_color_choose();
	void slot_high_intense_edit();
	void slot_high_intense_slider(int);
	void slot_highlight_slider_pressed();
	void slot_highlight_slider_relesed();
	void slot_gloss_edit();
	void slot_gloss_slider(int);
	void slot_gloss_slider_pressed();
	void slot_gloss_slider_relesed();
	void slot_opacity_slider();
	void slot_opacity_spin(int);
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

	void slot_slider_pressed();
	void slot_slider_relesed();
};
