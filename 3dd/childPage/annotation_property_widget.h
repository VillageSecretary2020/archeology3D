#pragma once
#include "base_property_widget.h"

class annotation_property_widget : public base_property_widget
{
	Q_OBJECT
 public:
	annotation_property_widget(int num,G_Comment* annotation=nullptr);
	~annotation_property_widget();
	G_Comment* return_annotation_property_struct();
	virtual void delete_self();
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	void paintEvent(QPaintEvent*);
	QTreeWidget *m_NormalProperty;
	QTreeWidgetItem *m_annotation_treeitem;
	QTreeWidgetItem *m_text_treeitem;
	QTreeWidgetItem *m_font_treeitem;
	QTreeWidgetItem *m_font_size_treeitem;
	QTreeWidgetItem *m_text_align_treeitem;
	QTreeWidgetItem *m_color_treeitem;
	QTreeWidgetItem *m_position_treeitem;
	QTreeWidgetItem *m_origin_treeitem;
	QTreeWidgetItem *m_x_treeitem;
	QTreeWidgetItem *m_y_treeitem;
	QLineEdit *m_Text;
	QComboBox *m_Font;
	QSpinBox *m_FontSize_spin;
	QSlider *m_FontSize_slider;
	QComboBox *m_TextAlign;
	QComboBox *m_Color;
	QComboBox *m_Origin;
	QDoubleSpinBox *m_x_spin;
	QSlider *m_x_slider;
	QDoubleSpinBox *m_y_spin;
	QSlider *m_y_slider;
	G_Comment* m_return_annotation_property_struct;
	void m_update_widget_property();

 public slots:

signals:
	void sig_property_manager_dock_null();
	void sig_note_to_axis();
	void sig_note_to_border();
	void sig_delete_annotation();
private slots:
	void slot_note_to_axis();
	void slot_note_to_border();
	void slot_fontsize_slider(int);
	void slot_fontsize_spin(int);
	void slot_text_align();
	void slot_change_font();
	void slot_color_choose();
	void m_x_slider_setValue(double);
	void m_x_spin_setValue(int a);
	void m_y_slider_setValue(double);
	void m_y_spin_setValue(int);
	void slot_origin_align();
	void slot_input_text();
};
