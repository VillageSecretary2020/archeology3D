#pragma once
#include <QProgressDialog>
#include "base_property_widget.h"

class gridding_property_widget:public base_property_widget  //   网格化  类
{
	Q_OBJECT
public:
	gridding_property_widget(int num,C_Mesh* mesh=NULL);
	~gridding_property_widget();
	C_Mesh* return_mesh_property_struct();
	virtual void delete_self();
	//static void set_progress_range(int a, int b, void *p);
	//static void set_progressBar_value(int i, void*p);
	bool set_progress(int r, int p, const char* m);

	//	void close_progress(); // 移至 void base_property_widget
	virtual void end_progress();
	bool isRunning() { return node_running; };
private:
	virtual void contextMenuEvent(QContextMenuEvent *);
	//C_Mesh *m_return_mesh_struct;
	void m_update_widget_property();
	void paintEvent(QPaintEvent*);
	QProgressDialog *m_progress=nullptr;
	bool node_running = false;
private:
	QTreeWidget *m_normal_tree;
	QTreeWidgetItem *m_gridding_treeitem;
	//	QTreeWidgetItem *m_input_treeitem;
	//QTreeWidgetItem *m_input_data_count_treeitem;
	//QTreeWidgetItem *m_field_index_treeitem;
	QLineEdit *m_field_index_edit;

	QTreeWidgetItem *m_gridding_operator_treeitem;
	QTreeWidgetItem *m_polation_tree;
	QTreeWidgetItem *m_polation_treeitem;

	QWidget *m_start_gridding_widge;
	QHBoxLayout *m_start_gridding_layout;
	QPushButton *m_start_gridding_button;
	QProgressBar *progressBar;

	QComboBox *m_polation_combox;
	  //海量数据法
	QTreeWidgetItem *m_measure_type_treeitem;
	QComboBox *m_measure_type_combox;
	  //反距离法
	QTreeWidgetItem *m_anisotropy_treeitem;
	QComboBox *m_anisotropy_combox;
	QTreeWidgetItem *m_weighted_index_treeitem;
	QTreeWidgetItem *m_smooth_treeitem;
	QLineEdit *m_weighted_index_edit;
	QLineEdit *m_smooth_edit;
	  //kriging法
	QTreeWidgetItem *m_function_model_treeitem;
	QComboBox *m_function_model_combox;
	QTreeWidgetItem *m_nugget_treeitem;
	QLineEdit *m_nugget_edit;
	QTreeWidgetItem *m_abutment_treeitem;
	QLineEdit *m_abutment_edit;
	QTreeWidgetItem *m_range_treeitem;
	QLineEdit *m_range_edit;
	//几何属性
	QTreeWidget *m_geometry_property_tree;
	QTreeWidgetItem *m_geometry_property_treeitem;
	QTreeWidgetItem *m_x_limit_treeitem;
	QTreeWidgetItem *m_x_min_treeitem;
	QLineEdit *m_x_min_edit;
	QTreeWidgetItem *m_x_max_treeitem;
	QLineEdit *m_x_max_edit;
	QTreeWidgetItem *m_y_limit_treeitem;
	QTreeWidgetItem *m_y_min_treeitem;
	QLineEdit *m_y_min_edit;
	QTreeWidgetItem *m_y_max_treeitem;
	QLineEdit *m_y_max_edit;
	QTreeWidgetItem *m_z_limit_treeitem;
	QTreeWidgetItem *m_z_min_treeitem;
	QLineEdit *m_z_min_edit;
	QTreeWidgetItem *m_z_max_treeitem;
	QLineEdit *m_z_max_edit;
	QTreeWidgetItem *m_resolution_treeitem;
	QTreeWidgetItem *m_Nx_treeitem;
	QTreeWidgetItem *m_Ny_treeitem;
	QTreeWidgetItem *m_Nz_treeitem;
	QTreeWidgetItem *m_interval_treeitem;
	QTreeWidgetItem *m_x_interval_treeitem;
	QTreeWidgetItem *m_y_interval_treeitem;
	QTreeWidgetItem *m_z_interval_treeitem;
	QSpinBox *m_Nx_spinbox;
	QSpinBox *m_Ny_spinbox;
	QSpinBox *m_Nz_spinbox;
	QLineEdit *m_x_interval_edit;
	QLineEdit *m_y_interval_edit;
	QLineEdit *m_z_interval_edit;
	//搜索
	QTreeWidget *m_search_tree;
	QTreeWidgetItem *m_search_treeitem;
	QTreeWidgetItem *m_search_type_treeitem;
	QComboBox *m_search_type_combox;
	QTreeWidgetItem *m_min_count_treeitem;
	QTreeWidgetItem *m_max_count_treeitem;
	QSpinBox *m_min_count_spinbox;
	QSpinBox *m_max_count_spinbox;
	//简单类型
	QTreeWidgetItem *m_search_radius_treeitem;
	QLineEdit *m_search_radius_edit;

	//各项异性
	QTreeWidgetItem *m_search_ellipse_treeitem;
	QTreeWidgetItem *m_x_length_treeitem;
	QLineEdit *m_x_length_edit;
	QTreeWidgetItem *m_y_length_treeitem;
	QLineEdit *m_y_length_edit;
	QTreeWidgetItem *m_z_length_treeitem;
	QLineEdit *m_z_length_edit;
signals:
	void sig_delete_gridding();
	void sig_vtk_adapt_win();
	void sig_property_manager_dock_null();
	/*
	void sig_gridding_to_slice_render();
	void sig_gridding_to_shape_render();
	void sig_gridding_to_scatter();
	void sig_gridding_to_orthogonal();
	void sig_gridding_to_isoline();
	void sig_gridding_to_isosurface();
	void sig_gridding_to_axis();
	void sig_gridding_to_border();
	void sig_gridding_to_surface_render();
	void sig_gridding_to_transform();
	void sig_gridding_to_subset();
	*/
	//void sig_vtk_camera_render();
	void sig_updateRender(vtkSmartPointer<vtkRenderer> render);
	void sig_set_progress(int range, int pos, const char*);

public slots:
private slots:
	void slot_updateRender(vtkSmartPointer<vtkRenderer> render);	

	/*
	void slot_gridding_to_slice();
	void slot_gridding_to_shape_render();
	void slot_gridding_to_scatter();
	void slot_gridding_to_orthogonal();
	void slot_gridding_to_isoline();
	void slot_gridding_to_isosurface();
	void slot_gridding_to_axis();
	void slot_gridding_to_border();
	void slot_gridding_to_transform();
	void slot_gridding_to_subset();
	void slot_gridding_to_surface_render();
	*/

	void slot_field_num();
	void slot_polation_method();
	void slot_search_type();
	void slot_x_min_text();
	void slot_start_mesh();
	void slot_ellipse_x_length();
	void slot_ellipse_y_length();
	void slot_ellipse_z_length();
	void slot_min_count();
	void slot_max_count();
	void slot_x_max_text();
	void slot_y_min_text();
	void slot_y_max_text();
	void slot_z_min_text();
	void slot_z_max_text();
	void slot_resolution_Nx();
	void slot_resolution_Ny();
	void slot_resolution_Nz();
	void slot_x_interval_text();
	void slot_y_interval_text();
	void slot_z_interval_text();
	void slot_search_radius();
	void slot_range();
	void slot_abutment();
	void slot_nugget();
	void slot_smooth();
	void slot_weighted_index();
	void slot_set_progress(int range, int pos, const char*);
};
