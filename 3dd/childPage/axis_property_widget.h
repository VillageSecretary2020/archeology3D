// axis_property_widget.h

#pragma once
#include "base_property_widget.h"

class axis_property_widget :public base_property_widget
{
	Q_OBJECT
public:
	axis_property_widget(int num,G_Axis* axis=nullptr);
	~axis_property_widget();
	G_Axis* return_axis_property_struct();
	virtual void delete_self() ;
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void m_update_widget_property();
	void paintEvent(QPaintEvent*);
private:
	QTreeWidget *m_normal_property;  //常规属性
	QTreeWidgetItem *m_normal_axis_treeitem;
	QTreeWidgetItem *m_order_by_data_treeitem;
	QCheckBox *m_order_by_data;
	//QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_show_mesh_treeitem;
	QCheckBox *m_show_mesh;
	QTreeWidgetItem *m_mesh_color_treeitem;
	QComboBox *m_mesh_color;
	QTreeWidgetItem *m_meshline_width_treeitem;
	QTreeWidgetItem *m_font_treeitem;
	QComboBox *m_font;
	QTreeWidgetItem *m_show_arrow_treeitem;
	QCheckBox *m_show_arrow;
	QSpinBox *m_meshline_width_spin;
	QTreeWidgetItem *m_axis_thickness_treeitem;
	QDoubleSpinBox *m_axis_thickness_spin;
	QTreeWidgetItem *m_mark_size_treeitem;
	QSpinBox *m_mark_size_spin;
	QTreeWidgetItem *m_title_size_treeitem;
	QSpinBox *m_title_size_spin;
	QSlider *m_meshline_width_slider;
	QSlider *m_axis_thickness_slider;
	QSlider *m_mark_size_slider;
	QSlider *m_title_size_slider;

	QTreeWidget *m_x_axis;  //x轴
	QTreeWidgetItem *m_x_color_treeitem;
	QComboBox *m_xColor;
	QTreeWidgetItem *m_x_axis_treeitem;
	QTreeWidgetItem *m_x_note_treeitem;
	QTreeWidgetItem *m_x_show_axis_treeitem;
	QCheckBox *m_x_show_axis;
	QTreeWidgetItem *m_x_title_treeitem;
	QLineEdit *m_x_title_edit;
	QTreeWidgetItem *m_x_axis_min_treeitem;
	QLineEdit				*m_x_axis_min_edit;
	QTreeWidgetItem *m_x_axis_max_treeitem;
	QLineEdit				*m_x_axis_max_edit;
	QTreeWidgetItem *m_x_note_show_treeitem;
	QTreeWidgetItem *m_x_note_min_treeitem1;
	QLineEdit				*m_x_note_min_edit;
	QTreeWidgetItem *m_x_note_max_treeitem;
	QLineEdit				*m_x_note_max_edit;
	QTreeWidgetItem *m_x_note_interval_treeitem;
	QLineEdit				*m_x_note_interval_edit;
	QTreeWidgetItem *m_x_note_format_treeitem;
	QTreeWidgetItem *m_x_note_type_treeitem;
	QTreeWidgetItem *m_x_note_significant_treeitem;
	QCheckBox *m_x_show_note;
	QComboBox *m_x_note_type;
	QSpinBox *m_x_note_significant_spin;

	QTreeWidget			*m_y_axis;  //y轴
	QTreeWidgetItem	*m_y_color_treeitem;
	QComboBox			*m_yColor;
	QTreeWidgetItem	*m_y_axis_treeitem;
	QTreeWidgetItem	*m_y_note_treeitem;
	QTreeWidgetItem	*m_y_show_axis_treeitem;
	QCheckBox			*m_y_show_axis;
	QTreeWidgetItem	*m_y_title_treeitem;
	QLineEdit				*m_y_title_edit;
	QTreeWidgetItem	*m_y_axis_min_treeitem;
	QLineEdit				*m_y_axis_min_edit;
	QTreeWidgetItem	*m_y_axis_max_treeitem;
	QLineEdit				*m_y_axis_max_edit;
	QTreeWidgetItem	*m_y_note_show_treeitem;
	QTreeWidgetItem	*m_y_note_min_treeitem;
	QLineEdit				*m_y_note_min_edit;
	QTreeWidgetItem	*m_y_note_max_treeitem;
	QLineEdit				*m_y_note_max_edit;
	QTreeWidgetItem	*m_y_note_interval_treeitem;
	QLineEdit				*m_y_note_interval_edit;
	QTreeWidgetItem	*m_y_note_format_treeitem;
	QTreeWidgetItem	*m_y_note_type_treeitem;
	QTreeWidgetItem	*m_y_note_significant_treeitem;
	QCheckBox			*m_y_note_show_check;
	QComboBox			*m_y_note_type_combox;
	QSpinBox				*m_y_note_significant_spin;

	QTreeWidget *m_z_axis;  //z轴
	QTreeWidgetItem *m_z_color_treeitem;
	QComboBox *m_zColor;
	QTreeWidgetItem *m_z_axis_treeitem;
	QTreeWidgetItem	*m_z_note_treeitem;
	QTreeWidgetItem *m_z_show_axis_treeitem;
	QCheckBox			*m_z_show_axis;
	QTreeWidgetItem *m_z_title_treeitem;
	QLineEdit				*m_z_title_edit;
	QTreeWidgetItem *m_z_axis_min_treeitem;
	QLineEdit				*m_z_axis_min_edit;
	QTreeWidgetItem	*m_z_axis_max_treeitem;
	QLineEdit				*m_z_axis_max_edit;
	QTreeWidgetItem *m_z_note_show_treeitem;
	QTreeWidgetItem *m_z_note_min_treeitem;
	QLineEdit				*m_z_note_min_edit;
	QTreeWidgetItem *m_z_note_max_treeitem;
	QLineEdit				*m_z_note_max_edit;
	QTreeWidgetItem *m_z_note_interval_treeitem;
	QLineEdit				*m_z_note_interval_edit;
	QTreeWidgetItem *m_z_note_format_treeitem;
	QTreeWidgetItem *m_z_note_type_treeitem;
	QTreeWidgetItem *m_z_note_significant_treeitem;
//	QCheckBox *m_z_show_axis_check;
//	QSpinBox *m_z_note_scale_spin;
	QCheckBox *m_z_note_show_check;
	QComboBox *m_z_note_type_combox;
	QSpinBox *m_z_note_significant_spin;
	G_Axis* m_return_axis_property_struct;
signals:
	void sig_property_manager_dock_null();
	void sig_axis_to_border();
	void sig_delete_axis();	// yzw 0606

private slots:
	void slot_axis_to_border();
	void slot_show_mesh();
	void slot_meshline_width_slider_setValue(int);
	void slot_meshline_width_spin_setValue(int);
	void slot_axis_thickness_slider_setValue(double a);
	void slot_axis_thickness_spin_setValue(int a);
	void slot_mark_size_slider_setValue(int);
	void slot_mark_size_spin_setValue(int);
	void slot_title_size_slider_setValue(int);
	void slot_title_szie_spin_setValue(int);
	void slot_update_widget_property();
};

