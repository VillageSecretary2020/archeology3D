#pragma once
#include "base_property_widget.h"
#include "color_scale_property.h"

class scatter_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	scatter_property_widget(int num,G_ScatterMap* pData = NULL);
	~scatter_property_widget();
	G_ScatterMap* return_scatter_property_struct();
	virtual void delete_self();
private:
    G_ScatterMap* m_return_scatter_struct;
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	void m_update_widget_property();
private:
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_scatter_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_density_treeitem;
	QTreeWidgetItem *m_interpolation_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_symbol_treeitem;
	QTreeWidgetItem *m_size_treeitem;
	QTreeWidgetItem *m_color_method_treeitem;
	QTreeWidgetItem *m_color_treeitem;
	QTreeWidgetItem *m_gradient_color_treeitem;
	QComboBox *m_gradient_color_combox;
	QTreeWidgetItem *m_color_range_treeitem;
	QComboBox *m_color_range_combox;
	QTreeWidgetItem *m_red_component_treeitem;
	QTreeWidgetItem *m_green_component_treeitem;
	QTreeWidgetItem *m_blue_component_treeitem;
	QComboBox *m_red_component_combox;
	QComboBox *m_green_component_combox;
	QComboBox *m_blue_component_combox;
	QComboBox *m_color_combox;
	QComboBox *m_density;
	QComboBox *m_symbol;
	QComboBox *m_color_method;
	QSlider *m_size_slider;  //大小  滑动条
	QLineEdit *m_size_edit;   //大小 文本框
    //标注
	QTreeWidget *m_note_tree;
	QTreeWidgetItem *m_note_treeitem;
	QTreeWidgetItem *m_show_note_treeitem;
	QTreeWidgetItem *m_note_font_treeitem;
	QTreeWidgetItem *m_note_density_treeitem;
	QTreeWidgetItem *m_show_line_treeitem;
	QTreeWidgetItem *m_note_type_treeitem;
	QTreeWidgetItem *m_note_significant_treeitem;
	QCheckBox *m_show_note_check;
	QComboBox *m_note_font_combox;
	QComboBox *m_density_combox;
	QCheckBox *m_show_line_check;
	QComboBox *m_type_combox;
	QSpinBox *m_note_significant_spin;
	//色标刻度尺
	color_scale_property *m_color_scale;

	bool m_sliderPressed;//主要是解决滑动条滑动时槽函数重复调用问题
signals:
	void sig_property_manager_dock_null();
	void sig_scatter_to_cut();
	void sig_scatter_to_axis();
	void sig_scatter_to_border();
	void sig_delete_scatter();	// yzw 0607
private slots:
	void slot_update_widget_property();
	void slot_density_change(int);
	void slot_symbol_change(int);
	void slot_color_method_change(int);
	void m_size_slider_setValue();
	void m_size_spin_setValue(int a);
	void slot_size_pressed();
	void slot_size_released();
	void slot_color_choose();
	void slot_gradient_color();

	void slot_show_note();
	void slot_note_font();
	void slot_density();
	void slot_show_line();
	void slot_type();
	void slot_sig();

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
	void slot_color_scale_property_changed(Child_ColorBar*, int type);

	//treeitem点击显示状态信息槽函数
	void slot_status_show_input(QTreeWidgetItem*, int);
	void slot_status_show_note(QTreeWidgetItem*, int);

	void slot_create_cut();
	void slot_create_axis();
	void slot_create_border();
private:
	void updateWindow();
};
