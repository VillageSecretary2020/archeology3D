#pragma once
#include "NodeParaters.h"
#include <QTreeWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#define TRI_POINT 1
class cut_plane_property :public QTreeWidget
{
	Q_OBJECT
public:
	cut_plane_property();
	void set_data(Child_CutingPlane*, int multi, int offset);
	void set_normal(double* nor);
	void set_range(float* range);
	void connect_items( );
	void select_point(int);

	QTreeWidgetItem *m_cut_plane_treeitem;	
	QTreeWidgetItem *m_show_point_treeitem;
	QTreeWidgetItem *m_three_points_treeitem;
	QTreeWidgetItem *m_point_treeitem[3];
//	QCheckBox	*m_show_point;
	QComboBox	*m_select_mode;
	QCheckBox	*m_select_point[3];
	QLabel			*m_label_point[3];
	int m_point_n = 0;
	QTreeWidgetItem *m_axis_treeitem;
	QComboBox	*m_axis_combo;

	QTreeWidgetItem *m_normal_treeitem;
	QTreeWidgetItem *m_place_direction_treeitem;
	QTreeWidgetItem *m_normal_direction_treeitem;
	QTreeWidgetItem *m_normal_x_direction_treeitem;
	QTreeWidgetItem *m_normal_y_direction_treeitem;
	QTreeWidgetItem *m_normal_z_direction_treeitem;
	QTreeWidgetItem *m_offset_from_center_treeitem;

	QLineEdit *m_x_normal_edit;
	QLineEdit *m_y_normal_edit;
	QLineEdit *m_z_normal_edit;
	QComboBox *m_place_direction;
	QLineEdit *m_offset_from_center_edit;
	QSlider *m_offset_from_center_slider;
	QPushButton*m_point_button;

protected slots:
	void slot_place_direction();
	void slot_normal();
	void slot_offset_from_center_slider();
	void slot_offset_from_center_spin(int);
	void slot_show_point();
	void slot_select_point1();
	void slot_select_point2();
	void slot_select_point3();
	void slot_select_mode(int);
	void slot_select_axis(int);

	void slot_property_changed();
signals:
	void cut_plane_property_changed(Child_CutingPlane*, int type);
private:
	void place_direction();
	void set_normal_text();
	Child_CutingPlane* data;				// 操作的数据
	float range[3];

	int mp= 1000, offs= 500;		// 个别调节参数
};

