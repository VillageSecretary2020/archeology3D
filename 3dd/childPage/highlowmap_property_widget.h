#pragma once
#include "base_property_widget.h"

class highlowmap_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	highlowmap_property_widget(int num, QString filename, G_HighLowZoneMap* bound=NULL);  //边界框类
	~highlowmap_property_widget();
	G_HighLowZoneMap* return_highlowmap_form_property_struct();
	virtual void delete_self();
private:
	virtual void contextMenuEvent(QContextMenuEvent  *);
	void paintEvent(QPaintEvent*);
private:	
	QTreeWidget *m_Normal_TreeWidget;
	QTreeWidgetItem *m_normal_property_treeitem;
#if 0
	QTreeWidgetItem *m_directtion_treeitem;
	QComboBox *m_directtion_combo;
	QTreeWidgetItem *m_sectionId_treeitem;
	QLineEdit				*m_sectionId_edit;
#endif
	QTreeWidgetItem *m_scaleFactor_treeitem;
	QLineEdit				*m_scaleFactor_edit;

	QTreeWidgetItem *m_renderInfo_treeitem;
	QTreeWidgetItem *m_drawStyle_treeitem;
	QComboBox *m_drawStyle_combo;
	QTreeWidgetItem *m_opacity_treeitem;
	QLineEdit				*m_opacity_t_edit;

	QTreeWidgetItem *m_overlap_treeitem;
	QTreeWidgetItem *m_showCover_treeitem;
	QComboBox *m_showCover_combo;
	QTreeWidgetItem *m_modulation_tree_item;
	QComboBox *m_modulation_combo;
	QTreeWidgetItem *m_suitedType_tree_item;
	QComboBox *m_suitedType_combo;
	QTreeWidgetItem *m_textureFile_treeitem;
	QLineEdit *m_textureFile_edit;
	QPushButton*m_file_button ;

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
	G_HighLowZoneMap* m_return_border_property_struct;
signals:
	void sig_property_manager_dock_null();
	void sig_border_to_axis();
	void sig_delete_highlowmap();
public slots:
private slots:
	void m_update_widget_property();
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
	void slot_select_file();
};

