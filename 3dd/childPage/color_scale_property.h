#pragma once
#include "NodeParaters.h"
//#include <qtreewidget.h>
#include <QTreeWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QStringList>

class color_scale_property :public QTreeWidget
{
   Q_OBJECT
public:
	color_scale_property();
	void set_data(Child_ColorBar*);
	void connect_items( );

	QTreeWidgetItem *m_color_scale_treeitem;
	QTreeWidgetItem *m_show_color_treeitem;
	QTreeWidgetItem *m_color_place_direction_treeitem;
	QTreeWidgetItem *m_x_position_treeitem;
	QTreeWidgetItem *m_y_position_treeitem;
	QTreeWidgetItem *m_width_treeitem;
	QTreeWidgetItem *m_length_treeitem;
	QTreeWidgetItem *m_title;
	QTreeWidgetItem *m_title_treeitem;
	QLineEdit				*m_title_edit;
	QTreeWidgetItem *m_font_size_treeitem;
	QTreeWidgetItem *m_note_treeitem;
	QTreeWidgetItem *m_note_number_treeitem;
	QTreeWidgetItem *m_note_font_size_treeitem;
	QTreeWidgetItem *m_adopt_self_note_treeitem;
	QTreeWidgetItem *m_self_note_treeitem;
	QTreeWidgetItem *m_note_format_treeitem;
	QTreeWidgetItem *m_type_treeitem;
	QTreeWidgetItem *m_significant_treeitem;
	QTreeWidgetItem *m_prefix_treeitem;
	QLineEdit				*m_prefix_edit;
	QTreeWidgetItem *m_suffix_treeitem;
	QLineEdit				*m_suffix_edit;
	QCheckBox *m_show_color_scale;
	QComboBox *m_color_place_direction;
	QDoubleSpinBox *m_x_position_spin;
	QSlider *m_x_position_slider;
	QDoubleSpinBox *m_y_position_spin;
	QSlider *m_y_position_slider;
	QDoubleSpinBox *m_width_spin;
	QSlider *m_width_slider;
	QDoubleSpinBox *m_length_spin;
	QSlider *m_length_slider;
	QSpinBox *m_font_size_spin;
	QSlider *m_font_size_slider;
	QSpinBox *m_note_number_spin;
	QSpinBox *m_note_font_size_spin;
	QSlider *m_note_font_size_slider;
	QCheckBox *m_adopt_self_note;
	QComboBox *m_type;
	QSpinBox *m_significant_number_spin;

protected slots:
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

	void slot_property_changed();
	void slot_annotation_changed();
signals:
	void color_scale_property_changed(Child_ColorBar*, int type);
private:
	void property_changed(int);
	Child_ColorBar* data;				// 操作的数据
};

