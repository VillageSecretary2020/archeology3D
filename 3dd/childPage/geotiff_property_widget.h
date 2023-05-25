#pragma once
#include "base_property_widget.h"

#define SLIDER_OFFSET_MAX		10000
#define SLIDER_OFFSET_MID		(SLIDER_OFFSET_MAX/2)
#define PIC_SCALE_BASE	10000.0
class geotiff_property_widget:public base_property_widget
{
	Q_OBJECT
public:
	geotiff_property_widget(int num, QString filename, G_Geotiff* bound=NULL);
	~geotiff_property_widget();
	G_Geotiff* return_picture_form_property_struct();
	virtual void delete_self();
	void curPos(double*);

private:
	virtual void contextMenuEvent(QContextMenuEvent  *);
	void paintEvent(QPaintEvent*);

	void select_point(int);
private:	
	QTreeWidget *m_Normal_TreeWidget;
	QTreeWidgetItem *m_normal_property_treeitem;

	QTreeWidgetItem *m_textureFile_treeitem;
	QLineEdit *m_textureFile_edit;
	QPushButton*m_file_button ;

	QTreeWidgetItem *m_drag_treeitem;
	QCheckBox	*m_drag;

	QTreeWidgetItem *m_pic_opacity_treeitem;
	QSlider* m_opacity_slider;
	QTreeWidgetItem *m_pic_scale_treeitem;
	QSlider* m_pic_scale;
	QLineEdit *m_pic_scale_edit;
	bool m_scalePressed;
	QTreeWidgetItem *m_pic_adopt_treeitem;
	QComboBox *m_adopt_combo;

	QTreeWidgetItem *m_pic_x_start_treeitem, *m_pic_x_end_treeitem;
	QTreeWidgetItem *m_pic_y_start_treeitem, *m_pic_y_end_treeitem;
	QSlider *m_x_start_slider, *m_x_end_slider;
	QSlider *m_y_start_slider, *m_y_end_slider;
	QTreeWidgetItem *m_probe_pos_treeitem;
	QCheckBox *m_probe_pos_cb;
	QLabel *m_label_pos;

	QTreeWidgetItem *m_pic_x_offset_treeitem, *m_pic_y_offset_treeitem, *m_pic_z_offset_treeitem, *m_pic_z_rotate_treeitem;
	QSlider *m_pic_x_offset, *m_pic_y_offset, *m_pic_z_offset, *m_pic_z_rotate;
	QLineEdit *m_x_offset_edit, *m_y_offset_edit, *m_z_offset_edit, *m_z_rotate_edit ;

	QTreeWidgetItem *m_pic_operate_treeitem;
	QCheckBox	*m_apply;

	QPushButton *m_reset_button ;

	QTreeWidgetItem *m_pic_rect_sel_treeitem, *m_pic_adjust_pos_treeitem;
	QTreeWidgetItem *m_pic_vertex_treeitem[3];
	QCheckBox	*m_select_point[3];
	QLabel			*m_label_point[3];
	int m_point_n = 0;

	G_Geotiff* m_return_picture_property_struct;
private:
	void picture_change(int);
signals:
	void sig_property_manager_dock_null();
	void sig_border_to_axis();
	void sig_delete_geotiff();
public slots:
private slots:
	void m_update_widget_property();
	void slot_drag_enable(int);
	void slot_select_file();
	void slot_opacity_changed(int);
	void slot_scale_edit();
	void slot_scale_slider();
	void slot_pic_scale_pressed();
	void slot_pic_scale_relesed();
	void slot_adopt_combo();
	void slot_pic_apply(int);
	void slot_reset_offset();
	void slot_x_offset_edit();
	void slot_y_offset_edit();
	void slot_z_offset_edit();
	void slot_z_rotate_edit();
	void slot_x_offset_slider(); 
	void slot_y_offset_slider(); 
	void slot_z_offset_slider(); 
	void slot_z_rotate_slider(); 
	void slot_select_point1();
	void slot_select_point2();
	void slot_select_point3();
};

