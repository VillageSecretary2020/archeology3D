#pragma once
#include "base_property_widget.h"

class border_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	border_property_widget(int num,G_BoundingBox* bound=NULL);  //边界框类
	~border_property_widget();
	G_BoundingBox* return_border_form_property_struct();
	virtual void delete_self();
	void update_peak_from_node(PosXYZ &left_down, PosXYZ &right_upon);
private:
	virtual void contextMenuEvent(QContextMenuEvent  *);
	void paintEvent(QPaintEvent*);
private:
	QTreeWidget *m_Normal_TreeWidget;
	QTreeWidgetItem *m_normal_property_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	QTreeWidgetItem *m_left_down_treeitem;
	QTreeWidgetItem *m_right_upon_treeitem;
	QTreeWidgetItem *m_render_treeitem;
	QTreeWidgetItem *m_line_width_treeitem;
	QTreeWidgetItem *m_normal_color_treeitem;
	QTreeWidgetItem *m_note_treeitem;
	QTreeWidgetItem *m_show_note_treeitem;
	QTreeWidgetItem *m_font_treeitem;
	QTreeWidgetItem *m_font_size_treeitem;
	QTreeWidgetItem *m_note_color_treeitem;
	QTreeWidgetItem *m_note_format_treeitem;
	QTreeWidgetItem *m_note_type_treeitem;
	QTreeWidgetItem *m_note_significant_treeitem;
	QTreeWidgetItem *m_note_prefix_treeitem;
	QTreeWidgetItem *m_note_suffix_treeitem;
	QSpinBox *m_LineWidth_spin;
	QSlider *m_LineWidth_slider;
	QComboBox *m_Color;
	QTreeWidget *m_Notation_TreeWidget;
	QCheckBox *m_ShowNotation;
	QComboBox *m_Font;
	QSpinBox *m_FontSize_spin;
	QSlider *m_FontSize_slider;
	QComboBox *m_Notate_color;
	QComboBox *m_Type;
	QSpinBox *m_significant_spin;
	//G_BoundingBox* m_return_border_property_struct;
	void m_update_widget_property();
signals:
	void sig_property_manager_dock_null();
	void sig_border_to_axis();
	void sig_delete_border();
public slots:
private slots:
	void slot_m_linewidth_slider_setValue(int);
	void slot_m_linewidth_spin_setValue(int);
	void slot_border_to_axis();
	void slot_change_border_color();
	void slot_show_note();
	void slot_note_font();
	void slot_font_slider_setvalue(int);
	void slot_font_spin_setvalue(int);
	void slot_note_color_change();
	void slot_note_type_change();
	void slot_note_significant_change();
};

