#pragma once
#include "base_property_widget.h"
#include "color_scale_property.h"
#include "cut_plane_property.h"

class isoline_property_widget:public base_property_widget  //等值线图 类
{
	Q_OBJECT
public:
	isoline_property_widget(int num,G_IsoLine* isoline=nullptr);
	~isoline_property_widget();
	void setCutPlaneRange(float* dataRange);
	void setCutPoint(double* dataRange);
	G_IsoLine* return_isoline_property_struct();
	virtual void delete_self();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	QTreeWidget *m_normal_property;
	QTreeWidgetItem *m_isoline_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_input_data_limit_treeitem;
	QTreeWidgetItem *m_level_treeitem;
	QTreeWidgetItem *m_level_calculate_treeitem;
	QTreeWidgetItem *m_min_level_treeitem;
	QTreeWidgetItem *m_max_level_treeitem;
	QTreeWidgetItem *m_level_number_treeitem;
	QSpinBox *m_level_number_spinbox;
	QLineEdit *m_min_level_edit;
	QLineEdit *m_max_level_edit;
	//自动计算
	//最小最大计数
	//最小最大间隔
	QTreeWidgetItem *m_level_interval_treeitem;
	QLineEdit *m_level_interval_edit;
	//显式的
	QTreeWidgetItem *m_classify_treeitem;
	QLineEdit *m_classify_edit;

	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_line_width_treeitem;
	QTreeWidgetItem *m_gradient_chro_treeitem;
	QTreeWidgetItem *m_show_border_treeitem;
	QTreeWidgetItem *m_border_width_treeitem;
	QTreeWidgetItem *m_border_color_treeitem;
	QComboBox *m_level_calculate;
	QSpinBox *m_line_width_spin;
	QSlider *m_line_width_slider;
	QComboBox *m_gradient_chromatography;
	QCheckBox *m_show_border;
	QSpinBox *m_border_width_spin;
	QSlider *m_border_width_slider;
	QComboBox *m_border_color;
	cut_plane_property *m_cut_plane;
	color_scale_property *m_color_scale;
	G_IsoLine *m_return_isoline_property_struct;
	void m_update_widget_property();

	QTimer ia_tmr;
signals:
	void sig_property_manager_dock_null();
	void sig_isoline_to_axis();
	void sig_isoline_to_border();
		void sig_delete_isoline(); // yzw 0610
public slots:

private slots:
	virtual void slot_cut_plane_property_changed(Child_CutingPlane*, int type);
	void slot_cut_point_timeout();
	void slot_color_scale_property_changed(Child_ColorBar*, int type);

	void slot_isoline_to_axis();
	void slot_isoline_to_border();
	void slot_level_calculate();
	void slot_line_width_slider(int);
	void slot_line_width_spin(int);
	void slot_gradient_chro();
	void slot_show_border();
	void slot_border_width_slider(int);
	void slot_border_width_spin(int);
	void slot_place_direction();
	void slot_x_normal();
	void slot_y_normal();
	void slot_z_normal();
	void slot_offset_from_center_slider(double);
	void slot_offset_from_center_spin(int);
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
	void slot_m_note_font_size_slider(double);
	void slot_m_note_font_size_spin(int);

	void slot_update_widget_property();
};

