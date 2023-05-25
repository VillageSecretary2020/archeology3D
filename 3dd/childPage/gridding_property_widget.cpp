// gridding_property_widget.cpp

#include <QProgressBar>

#include "gridding_property_widget.h"
#include "myqwidget.h"

gridding_property_widget::gridding_property_widget(int num,C_Mesh* mesh)
{
	qRegisterMetaType<vtkSmartPointer<vtkRenderer>>("vtkSmartPointer<vtkRenderer>");
	QString label = (mesh) ? QString::fromStdString(mesh->m_name) : ( (num == 0)? STRING_LABEL_MESH : STRING_LABEL_MESH + QString::number(num + 1));
	create_2buttons(this, label);
	//常规属性
	m_normal_tree = new QTreeWidget;
	m_normal_tree->setColumnCount(2);
	m_normal_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_gridding_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_GRIDDING));
	m_gridding_operator_treeitem = new QTreeWidgetItem(m_gridding_treeitem, QStringList(STRING_ITEM_GRIDDINGOPER));
	QWidget *m_start_gridding_widget = new QWidget;
	m_start_gridding_layout = new QHBoxLayout;
	m_start_gridding_button = new QPushButton;
	m_start_gridding_button->setText(STRING_START_BUTTON);
	m_start_gridding_layout->addWidget(m_start_gridding_button);
	progressBar = new QProgressBar; 
	progressBar->hide();
	progressBar->setRange(0, 0);
	m_start_gridding_layout->addWidget(progressBar);
	m_start_gridding_widget->setLayout(m_start_gridding_layout);
	m_normal_tree->insertTopLevelItem(0, m_gridding_treeitem);
	m_normal_tree->setItemWidget(m_gridding_operator_treeitem, 1, m_start_gridding_widget);

	m_pre_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);
	m_input_data_count_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_INPUTCOUNT));
	m_input_data_count_treeitem->setText(1, "0");
	m_field_index_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_FIELD_INDEX));
	m_field_index_edit = new QLineEdit;
	m_field_index_edit->setText("0");
	m_normal_tree->setItemWidget(m_field_index_treeitem, 1, m_field_index_edit);

	m_polation_tree = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_POLATMETHOD));
	m_normal_tree->insertTopLevelItem(1, m_polation_tree);
	m_polation_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_POLATMETHOD));
	m_polation_combox = new QComboBox;
	m_polation_combox->insertItem(0, STRING_ITEM_INVERDISTAN);
	m_polation_combox->insertItem(1, STRING_ITEM_KRIGING);
	m_normal_tree->setItemWidget(m_polation_treeitem, 1, m_polation_combox);
	//反距离法
	m_anisotropy_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_ANISOTROPY));
	m_anisotropy_combox = new QComboBox;
	m_anisotropy_combox->insertItem(0, STRING_COMBOX_ISOTROPY);
	m_anisotropy_combox->insertItem(1, STRING_ITEM_ANISOTROPY);
	m_anisotropy_combox->insertItem(2, STRING_COMBOX_GENERPROP);
	m_normal_tree->setItemWidget(m_anisotropy_treeitem, 1, m_anisotropy_combox);
	m_weighted_index_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_WEIGHINDEX));
	m_weighted_index_edit = new QLineEdit;
	m_weighted_index_edit->setText("2");
	m_normal_tree->setItemWidget(m_weighted_index_treeitem, 1, m_weighted_index_edit);
	m_smooth_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_SMOOTH));
	m_smooth_edit = new QLineEdit;
	m_smooth_edit->setText("0");
	m_normal_tree->setItemWidget(m_smooth_treeitem, 1, m_smooth_edit);
	//Kriging 法
	m_function_model_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_FUNCMODEL));
	m_function_model_combox = new QComboBox;
	m_function_model_combox->insertItem(0, "1");
	m_function_model_combox->insertItem(1, "2");
	m_function_model_combox->insertItem(2, "3");
	m_normal_tree->setItemWidget(m_function_model_treeitem, 1, m_function_model_combox);
	m_nugget_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_NUGGET));
	m_nugget_edit = new QLineEdit;
	m_nugget_edit->setText("2");
	m_normal_tree->setItemWidget(m_nugget_treeitem, 1, m_nugget_edit);
	m_abutment_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_ABUTMENT));
	m_abutment_edit = new QLineEdit;
	m_abutment_edit->setText("2");
	m_normal_tree->setItemWidget(m_abutment_treeitem, 1, m_abutment_edit);
	m_range_treeitem = new QTreeWidgetItem(m_polation_tree, QStringList(STRING_ITEM_RANGE));
	m_range_edit = new QLineEdit;
	m_range_edit->setText("2");
	m_normal_tree->setItemWidget(m_range_treeitem, 1, m_range_edit);
	m_polation_combox->setCurrentIndex(0);
	m_function_model_treeitem->setHidden(true);
	m_nugget_treeitem->setHidden(true);
	m_abutment_treeitem->setHidden(true);
	m_range_treeitem->setHidden(true);
	m_normal_tree->expandAll();
	
	//几何属性
	m_geometry_property_tree = new QTreeWidget;
	m_geometry_property_tree->setColumnCount(2);
	m_geometry_property_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_geometry_property_treeitem = new QTreeWidgetItem(m_geometry_property_tree, QStringList(STRING_ITEM_GEOMPROTY));
	m_x_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(STRING_ITEM_XLIMIT));
	m_x_min_treeitem = new QTreeWidgetItem(m_x_limit_treeitem, QStringList(STRING_ITEM_XMIN));
	m_x_min_edit = new QLineEdit;
	m_x_min_edit->setText("0");
	m_geometry_property_tree->setItemWidget(m_x_min_treeitem, 1, m_x_min_edit);
	m_x_max_treeitem = new QTreeWidgetItem(m_x_limit_treeitem, QStringList(STRING_ITEM_XMAX));
	m_x_max_edit = new QLineEdit;
	m_x_max_edit->setText("1");
	m_geometry_property_tree->setItemWidget(m_x_max_treeitem, 1, m_x_max_edit);

	m_y_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(STRING_ITEM_YLIMIT));
	m_y_min_treeitem = new QTreeWidgetItem(m_y_limit_treeitem, QStringList(STRING_ITEM_YMIN));
	m_y_min_edit = new QLineEdit;
	m_y_min_edit->setText("0");
	m_geometry_property_tree->setItemWidget(m_y_min_treeitem, 1, m_y_min_edit);
	m_y_max_treeitem = new QTreeWidgetItem(m_y_limit_treeitem, QStringList(STRING_ITEM_YMAX));
	m_y_max_edit = new QLineEdit;
	m_y_max_edit->setText("1");
	m_geometry_property_tree->setItemWidget(m_y_max_treeitem, 1, m_y_max_edit);

	m_z_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(STRING_ITEM_ZLIMIT));
	m_z_min_treeitem = new QTreeWidgetItem(m_z_limit_treeitem, QStringList(STRING_ITEM_ZMIN));
	m_z_min_edit = new QLineEdit;
	m_z_min_edit->setText("0");
	m_geometry_property_tree->setItemWidget(m_z_min_treeitem, 1, m_z_min_edit);
	m_z_max_treeitem = new QTreeWidgetItem(m_z_limit_treeitem, QStringList(STRING_ITEM_ZMAX));
	m_z_max_edit = new QLineEdit;
	m_z_max_edit->setText("1");
	m_geometry_property_tree->setItemWidget(m_z_max_treeitem, 1, m_z_max_edit);

	m_resolution_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(STRING_ITEM_RESOLUTION));
	m_Nx_treeitem = new QTreeWidgetItem(m_resolution_treeitem, QStringList("Nx"));
	m_Nx_spinbox = new QSpinBox;
	m_Nx_spinbox->setValue(50);
	m_geometry_property_tree->setItemWidget(m_Nx_treeitem, 1, m_Nx_spinbox);

	m_Ny_treeitem = new QTreeWidgetItem(m_resolution_treeitem, QStringList("Ny"));
	m_Ny_spinbox = new QSpinBox;
	m_Ny_spinbox->setValue(50);
	m_geometry_property_tree->setItemWidget(m_Ny_treeitem, 1, m_Ny_spinbox);

	m_Nz_treeitem = new QTreeWidgetItem(m_resolution_treeitem, QStringList("Nz"));
	m_Nz_spinbox = new QSpinBox;
	m_Nz_spinbox->setValue(50);
	m_geometry_property_tree->setItemWidget(m_Nz_treeitem, 1, m_Nz_spinbox);

	m_interval_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(STRING_ITEM_INTERVAL));
	m_x_interval_treeitem = new QTreeWidgetItem(m_interval_treeitem, QStringList(STRING_ITEM_XINTERVAL));
	m_x_interval_edit = new QLineEdit;
	m_x_interval_edit->setText("0.02");
	m_geometry_property_tree->setItemWidget(m_x_interval_treeitem, 1, m_x_interval_edit);

	m_y_interval_treeitem = new QTreeWidgetItem(m_interval_treeitem, QStringList(STRING_ITEM_YINTERVAL));
	m_y_interval_edit = new QLineEdit;
	m_y_interval_edit->setText("0.02");
	m_geometry_property_tree->setItemWidget(m_y_interval_treeitem, 1, m_y_interval_edit);

	m_z_interval_treeitem = new QTreeWidgetItem(m_interval_treeitem, QStringList(STRING_ITEM_ZINTERVAL));
	m_z_interval_edit = new QLineEdit;
	m_z_interval_edit->setText("0.02");
	m_geometry_property_tree->setItemWidget(m_z_interval_treeitem, 1, m_z_interval_edit);
	m_geometry_property_tree->expandAll();

	//搜索属性
	m_search_tree = new QTreeWidget;
	m_search_tree->setColumnCount(2);
	m_search_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_search_treeitem = new QTreeWidgetItem(m_search_tree, QStringList(STRING_ITEM_SEARCH));
	m_search_type_treeitem = new QTreeWidgetItem(m_search_treeitem, QStringList(STRING_ITEM_SEARCHTYPE));
	m_search_type_combox = new QComboBox;
	m_search_type_combox->insertItem(0, STRING_COMBOX_ALLDATA);
	m_search_type_combox->insertItem(1, STRING_COMBOX_SIMPLE);
	m_search_type_combox->insertItem(2, STRING_COMBOX_ANTISOPHY);
	m_search_type_combox->setCurrentIndex(1);
	m_search_tree->setItemWidget(m_search_type_treeitem, 1, m_search_type_combox);
	//简单数据类型
	m_search_radius_treeitem = new QTreeWidgetItem(m_search_treeitem, QStringList(STRING_ITEM_SEARCHRADIUS));
	m_search_radius_edit = new QLineEdit;
	m_search_radius_edit->setText("1");
	m_search_tree->setItemWidget(m_search_radius_treeitem, 1, m_search_radius_edit);
	//各项异性类型
	m_search_ellipse_treeitem = new QTreeWidgetItem(m_search_treeitem,QStringList(STRING_ITEM_SEARCHELLIPSE));
	m_x_length_treeitem = new QTreeWidgetItem(m_search_ellipse_treeitem, QStringList(STRING_ITEM_XLENGTH));
	m_x_length_edit = new QLineEdit;
	m_x_length_edit->setText("1");
	m_search_tree->setItemWidget(m_x_length_treeitem, 1, m_x_length_edit);

	m_y_length_treeitem = new QTreeWidgetItem(m_search_ellipse_treeitem, QStringList(STRING_ITEM_YLENGTH));
	m_y_length_edit = new QLineEdit;
	m_y_length_edit->setText("1");
	m_search_tree->setItemWidget(m_y_length_treeitem, 1, m_y_length_edit);

	m_z_length_treeitem = new QTreeWidgetItem(m_search_ellipse_treeitem, QStringList(STRING_ITEM_ZLENGTH));
	m_z_length_edit = new QLineEdit;
	m_z_length_edit->setText("1");
	m_search_tree->setItemWidget(m_z_length_treeitem, 1, m_z_length_edit);

	m_min_count_treeitem = new QTreeWidgetItem(m_search_treeitem, QStringList(STRING_ITEM_MINCOUNT));
	m_min_count_spinbox = new QSpinBox;
	m_min_count_spinbox->setValue(1);
	m_search_tree->setItemWidget(m_min_count_treeitem, 1, m_min_count_spinbox);
	m_max_count_treeitem = new QTreeWidgetItem(m_search_treeitem, QStringList(STRING_ITEM_MAXCOUNT));
	m_max_count_spinbox = new QSpinBox;
	m_max_count_spinbox->setValue(100);
	m_search_tree->setItemWidget(m_max_count_treeitem, 1, m_max_count_spinbox);
	m_search_ellipse_treeitem->setHidden(true);
	m_search_tree->expandAll();

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_tree, QString(STRING_TAB_GENERAL));
	m_form1_tab->addTab(m_geometry_property_tree, QString(STRING_TAB_GEOMETRY));
	m_form1_tab->addTab(m_search_tree, QString(STRING_TAB_SEARCH));

	baseStruct = new C_Mesh();
	if (!mesh) {
		C_Mesh *m_return_mesh_struct=(C_Mesh*)baseStruct;
		CMeshNode::getDefault(m_return_mesh_struct);
		mesh = m_return_mesh_struct;
	}
	{
		m_input_treeitem->setText(1, QString::fromStdString(mesh->m_putIn));
		m_input_data_count_treeitem->setText(1, QString::number(mesh->m_putInPointNum));
			if (mesh->m_MethosType == INVERSEDISTANCE)
			{
				m_polation_combox->setCurrentIndex(0);
				m_weighted_index_edit->setText(QString::number(mesh->m_method_type.m_idw.order));
				m_smooth_edit->setText(QString("%1").arg(mesh->m_method_type.m_idw.smooth));
			}
			else  //mesh->m_MethosType==KRIGING
			{
				m_polation_combox->setCurrentIndex(1);
				m_function_model_combox->setCurrentIndex(mesh->m_method_type.m_kriging.mode);
				m_nugget_edit->setText(QString::number(mesh->m_method_type.m_kriging.c0, 10, 2));
				m_abutment_edit->setText(QString::number(mesh->m_method_type.m_kriging.c1, 10, 2));
				m_range_edit->setText(QString::number(mesh->m_method_type.m_kriging.a,10,2));
			}
			m_x_min_edit->setText(QString("%1").arg(mesh->m_geometry.limits[0]));	// mesh->m_geometry.limits[0] = m_x_min_edit->text().toDouble(); 
			m_x_max_edit->setText(QString("%1").arg(mesh->m_geometry.limits[3])); // mesh->m_geometry.limits[3] = m_x_max_edit->text().toDouble();
			m_y_min_edit->setText(QString("%1").arg(mesh->m_geometry.limits[1])); // mesh->m_geometry.limits[1] = m_y_min_edit->text().toDouble();
			m_y_max_edit->setText(QString("%1").arg(mesh->m_geometry.limits[4])); // mesh->m_geometry.limits[4] = m_y_max_edit->text().toDouble();
			m_z_min_edit->setText(QString("%1").arg(mesh->m_geometry.limits[2])); // mesh->m_geometry.limits[2] = m_z_min_edit->text().toDouble();
			m_z_max_edit->setText(QString("%1").arg(mesh->m_geometry.limits[5])); // mesh->m_geometry.limits[5] = m_z_max_edit->text().toDouble();
			m_Nx_spinbox->setValue(mesh->m_geometry.resolution[0]); // mesh->m_geometry.resolution[0] = m_Nx_spinbox->value();
			m_Ny_spinbox->setValue((mesh->m_geometry.resolution[1])); // mesh->m_geometry.resolution[1] = m_Ny_spinbox->value();
			m_Nz_spinbox->setValue((mesh->m_geometry.resolution[2])); // mesh->m_geometry.resolution[2] = m_Nz_spinbox->value();
			m_x_interval_edit->setText(QString("%1").arg(mesh->m_geometry.space[0])); // mesh->m_geometry.space[0] = m_x_interval_edit->text().toDouble();
			m_y_interval_edit->setText(QString("%1").arg(mesh->m_geometry.space[1])); // mesh->m_geometry.space[1] = m_y_interval_edit->text().toDouble();
			m_z_interval_edit->setText(QString("%1").arg(mesh->m_geometry.space[2])); // mesh->m_geometry.space[2] = m_z_interval_edit->text().toDouble();
		
			if (mesh->m_SearchType == SEARCH_AllDATA)
			{
				m_search_type_combox->setCurrentIndex(0);
				
			}
			else if (mesh->m_SearchType == SEARCH_SIMPLE)
			{
				m_search_type_combox->setCurrentIndex(1);
				mesh->m_search_type.m_simple.Radius = m_search_radius_edit->text().toFloat();
				mesh->m_search_type.m_simple.MinCount = m_min_count_spinbox->value();
				mesh->m_search_type.m_simple.MaxCount = m_max_count_spinbox->value();
			}
			else
			{
				m_search_type_combox->setCurrentIndex(2);
				mesh->m_search_type.m_anisotropic.SearchEllipse_x = m_x_length_edit->text().toFloat();
				mesh->m_search_type.m_anisotropic.SearchEllipse_y = m_y_length_edit->text().toFloat();
				mesh->m_search_type.m_anisotropic.SearchEllipse_z = m_z_length_edit->text().toFloat();
				mesh->m_search_type.m_anisotropic.MinCount = m_min_count_spinbox->value();
				mesh->m_search_type.m_anisotropic.MaxCount = m_max_count_spinbox->value();
			}
	}

	this->NodeType = STRING_CTYPE_GRIDDING;	// "mesh_widget";
	CAN_APPEND_WIDGET_GTYPE_ALL(m_can_append_widget);

	m_can_append_widget.push_back(STRING_CTYPE_TRANSFORM);
	m_can_append_widget.push_back(STRING_CTYPE_SUBSET);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_EXCLU);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_REPEAT);
	//m_return_mesh_struct = new C_Mesh();

	connect(m_field_index_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_field_num()));
	connect(m_start_gridding_button, SIGNAL(clicked(bool)), this, SLOT(slot_start_mesh()));
	connect(m_polation_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_polation_method()));
	//connect(m_anisotropy_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_anisotropy()));
	connect(m_weighted_index_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_weighted_index()));
	connect(m_smooth_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_smooth()));
	connect(m_nugget_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_nugget()));
	connect(m_abutment_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_abutment()));
	connect(m_range_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_range()));
	connect(m_x_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_x_min_text()));
	connect(m_x_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_x_max_text()));
	connect(m_y_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_y_min_text()));
	connect(m_y_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_y_max_text()));
	connect(m_z_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_z_min_text()));
	connect(m_z_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_z_max_text()));
	connect(m_Nx_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_resolution_Nx()));
	connect(m_Ny_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_resolution_Ny()));
	connect(m_Nz_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_resolution_Nz()));
	connect(m_x_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_x_interval_text()));
	connect(m_y_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_y_interval_text()));
	connect(m_z_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_z_interval_text()));
	connect(m_search_type_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_search_type()));
	connect(m_search_radius_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_search_radius()));
	connect(m_x_length_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_ellipse_x_length()));
	connect(m_y_length_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_ellipse_y_length()));
	connect(m_z_length_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_ellipse_z_length()));
	connect(m_min_count_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_min_count()));
	connect(m_max_count_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_max_count()));
	//connect(this, &gridding_property_widget::sig_updateRender, this, &gridding_property_widget::slot_updateRender);
	connect(this, SIGNAL(sig_set_progress(int, int, const char*)), this, SLOT(slot_set_progress(int, int, const char*)));
}

gridding_property_widget::~gridding_property_widget()
{
	delete m_form1_tab;
	
}

void gridding_property_widget::paintEvent(QPaintEvent *e)
{
#if 0
	QPainter p(this);
	//p.setCompositionMode(QPainter::CompositionMode_Clear);
	//p.fillRect(0, 0, 10, 40, Qt::SolidPattern);
	p.fillRect(0, 0, 10, 30, Qt::gray);
	p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);
#endif 
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

C_Mesh* gridding_property_widget::return_mesh_property_struct()
{
	getBaseStruct();
	C_Mesh *m_return_mesh_struct = (C_Mesh*)baseStruct;
	m_return_mesh_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	int point_count = m_input_data_count_treeitem->text(1).toInt();
	m_return_mesh_struct->m_dataIndex= m_field_index_edit->text().toInt();
	m_return_mesh_struct->m_putInPointNum = point_count;
	int index_polation = m_polation_combox->currentIndex();
	 if (index_polation == 0)//反距离法
		{
		 m_return_mesh_struct->m_MethosType = INVERSEDISTANCE;
		int anisophy_index = m_anisotropy_combox->currentIndex();
		//if (anisophy_index == 0)
		//	m_return_mesh_struct->m_parameter.m_InverseDistance.m_anisotropic = ISOTROPY;
		//else if (anisophy_index == 1)
		//	m_return_mesh_struct->m_parameter.m_InverseDistance.m_anisotropic = ANISOTROPIC;
		//else
		//	m_return_mesh_struct->m_parameter.m_InverseDistance.m_anisotropic = GENERAL_PROPERTIES;
		m_return_mesh_struct->m_SearchType = (SearchType)anisophy_index;
		m_return_mesh_struct->m_method_type.m_idw.order = m_weighted_index_edit->text().toFloat();
		m_return_mesh_struct->m_method_type.m_idw.smooth = m_smooth_edit->text().toInt();
	}
	else  //kriging 法
	{
		 m_return_mesh_struct->m_MethosType = KRIGING;
		 m_return_mesh_struct->m_method_type.m_kriging.mode = m_function_model_combox->currentIndex();
		 m_return_mesh_struct->m_method_type.m_kriging.c0 = m_nugget_edit->text().toDouble();
		 m_return_mesh_struct->m_method_type.m_kriging.c1 = m_abutment_edit->text().toDouble();
		 m_return_mesh_struct->m_method_type.m_kriging.a = m_range_edit->text().toDouble();
	}
	 m_return_mesh_struct->m_geometry.limits[0] = m_x_min_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.limits[3] = m_x_max_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.limits[1] = m_y_min_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.limits[4] = m_y_max_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.limits[2] = m_z_min_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.limits[5] = m_z_max_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.resolution[0] = m_Nx_spinbox->value();
	 m_return_mesh_struct->m_geometry.resolution[1] = m_Ny_spinbox->value();
	 m_return_mesh_struct->m_geometry.resolution[2] = m_Nz_spinbox->value();
	 m_return_mesh_struct->m_geometry.space[0] = m_x_interval_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.space[1] = m_y_interval_edit->text().toDouble();
	 m_return_mesh_struct->m_geometry.space[2] = m_z_interval_edit->text().toDouble();
	 int search_type = m_search_type_combox->currentIndex();
	 if (search_type == 0)
	 {
		 m_return_mesh_struct->m_SearchType = SEARCH_AllDATA;
		 //m_return_mesh_struct.m_search_type.m_alldata
	 }
	 else if (search_type == 1)
	 {
		 m_return_mesh_struct->m_SearchType = SEARCH_SIMPLE;
		 m_return_mesh_struct->m_search_type.m_simple.Radius = m_search_radius_edit->text().toFloat();
		 m_return_mesh_struct->m_search_type.m_simple.MaxCount = m_max_count_spinbox->value();
		 m_return_mesh_struct->m_search_type.m_simple.MinCount = m_min_count_spinbox->value();
	 }
	 else 
	 {
		 m_return_mesh_struct->m_SearchType = SEARCH_ANISOTROPIC;
		 m_return_mesh_struct->m_search_type.m_anisotropic.SearchEllipse_x = m_x_length_edit->text().toFloat();
		 m_return_mesh_struct->m_search_type.m_anisotropic.SearchEllipse_y = m_y_length_edit->text().toFloat();
		 m_return_mesh_struct->m_search_type.m_anisotropic.SearchEllipse_z = m_z_length_edit->text().toFloat();
		 m_return_mesh_struct->m_search_type.m_anisotropic.MaxCount = m_max_count_spinbox->value();
		 m_return_mesh_struct->m_search_type.m_anisotropic.MinCount= m_min_count_spinbox->value();
	 }

	return m_return_mesh_struct;
}

void gridding_property_widget::contextMenuEvent(QContextMenuEvent  *event)
{
	QMenu *menu = new QMenu(this);
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAPHIC);
#if 1
	MENU_RENDER_CALC_NODE(out_put, receiver1);
#else
	out_put->addAction(STRING_ACTION_SLICE,	receiver1, SLOT(create_slice())); // this, SLOT(slot_gridding_to_slice()));
	out_put->addAction(STRING_ACTION_SHAPRENDER,	receiver1, SLOT(create_shape_render()));// this, SLOT(slot_gridding_to_shape_render()));
	out_put->addAction(STRING_ITEM_SCATDIAG,		receiver1, SLOT(create_scatter()) ); // this, SLOT(slot_gridding_to_scatter()));
	out_put->addAction(STRING_ACTION_ORTHOGRAPH, receiver1, SLOT(create_orthogonal()));// this, SLOT(slot_gridding_to_orthogonal()));
	out_put->addAction(STRING_ITEM_ISOLINE, receiver1, SLOT(create_isoline())); // this, SLOT(slot_gridding_to_isoline()));
	out_put->addAction(STRING_ISOSURF_MENU, receiver1, SLOT(create_isosurface()));// this, SLOT(slot_gridding_to_isosurface()));	// YZW 0607 tr("isosurface")改为宏，支持中文
	out_put->addAction(STRING_ACTION_SUFARENDER, receiver1, SLOT(create_surface_render()));// this, SLOT(slot_gridding_to_surface_render()));
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));// this, SLOT(slot_gridding_to_axis()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border())); // this, SLOT(slot_gridding_to_border()));	// yzw 0610 slot_create_border
	out_put->addAction(STRING_ACTION_HLAREAMAP, receiver1, SLOT(create_highlowmap()));// this, SLOT(slot_create_border()));
#endif
	//QMenu  *normal = menu->addMenu(tr("normal module"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	QMenu   *calc = menu->addMenu(STRING_MENU_CALC);
	calc->addAction(STRING_ACTION_TRANSFORM, receiver1, SLOT(create_transform()));// this, SLOT(slot_gridding_to_transform()));
	calc->addAction(STRING_ACTION_SUBSET, receiver1, SLOT(create_subset()));// this, SLOT(slot_gridding_to_subset()));
	//calc->addAction(STRING_ACTION_REPEATFILT, receiver1, SLOT(create_repeat_filter()));
	//calc->addAction(STRING_ACTION_EXCLUFILTER, receiver1, SLOT(create_exclusion_filter()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect the number of point input(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("conect the number of  point input(not connect)"), this));
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	//menu->addAction(new QAction(tr("delete"), this));// tr(u8"删除")
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_source()));// this, SLOT(slot_delete_self()));	// slot_delete_self_widget
	menu->move(cursor().pos());
	menu->show();
}

void gridding_property_widget::delete_self()
{
	emit sig_delete_gridding();
}

void gridding_property_widget::m_update_widget_property()
{
	//myqwidget* widget = (myqwidget*)this->parentWidget();
	C_Mesh* pTemp = this->return_mesh_property_struct();
	std::shared_ptr<C_Mesh>ptr(std::make_shared<C_Mesh>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
}

void gridding_property_widget::slot_updateRender(vtkSmartPointer<vtkRenderer> render)
{
	render->ResetCamera();
	render->Render();
}

#if 0
void gridding_property_widget::slot_gridding_to_slice()
{
	emit sig_gridding_to_slice_render();
}

void gridding_property_widget::slot_gridding_to_shape_render()
{
	emit sig_gridding_to_shape_render();
}

void gridding_property_widget::slot_gridding_to_scatter()
{
	emit sig_gridding_to_scatter();
}

void gridding_property_widget::slot_gridding_to_orthogonal()
{
	emit sig_gridding_to_orthogonal();
}

void gridding_property_widget::slot_gridding_to_isoline()
{
	emit sig_gridding_to_isoline();
}

void gridding_property_widget::slot_gridding_to_isosurface()
{
	emit sig_gridding_to_isosurface();
}

void gridding_property_widget::slot_gridding_to_axis()
{
	emit sig_gridding_to_axis();
}

void gridding_property_widget::slot_gridding_to_border()
{
	emit sig_gridding_to_border();
}

void gridding_property_widget::slot_gridding_to_surface_render()
{
	emit sig_gridding_to_surface_render();
}

void gridding_property_widget::slot_gridding_to_transform()
{
	emit sig_gridding_to_transform();
}

void gridding_property_widget::slot_gridding_to_subset()
{
	emit sig_gridding_to_subset();
}
#endif

void gridding_property_widget::slot_field_num()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_polation_method()
{
	int index = m_polation_combox->currentIndex();
	if (index == 0)
	{
		m_anisotropy_treeitem->setHidden(false);
		m_weighted_index_treeitem->setHidden(false);
		m_smooth_treeitem->setHidden(false);
		m_function_model_treeitem->setHidden(true);
		m_nugget_treeitem->setHidden(true);
		m_abutment_treeitem->setHidden(true);
		m_range_treeitem->setHidden(true);
	}
	else
	{
		m_function_model_treeitem->setHidden(false);
		m_nugget_treeitem->setHidden(false);
		m_abutment_treeitem->setHidden(false);
		m_range_treeitem->setHidden(false);
		m_anisotropy_treeitem->setHidden(true);
		m_weighted_index_treeitem->setHidden(true);
		m_smooth_treeitem->setHidden(true);
	}
	m_update_widget_property();
}

void gridding_property_widget::slot_search_type()
{
	int index = m_search_type_combox->currentIndex();
	if (index == 0)
	{
		m_search_radius_treeitem->setHidden(true);
		m_search_ellipse_treeitem->setHidden(true);
		m_min_count_treeitem->setHidden(true);
		m_max_count_treeitem->setHidden(true);
	}
	else if (index == 1)
	{
		m_search_radius_treeitem->setHidden(false);
		m_min_count_treeitem->setHidden(false);
		m_max_count_treeitem->setHidden(false);
		m_search_ellipse_treeitem->setHidden(true);
	}
	else
	{
		m_search_ellipse_treeitem->setHidden(false);
		//m_x_length_treeitem->setHidden(false);
		//m_y_length_treeitem->setHidden(false);
		m_min_count_treeitem->setHidden(false);
		m_max_count_treeitem->setHidden(false);
		m_search_radius_treeitem->setHidden(true);
	}
	m_update_widget_property();
}

void gridding_property_widget::slot_weighted_index()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_smooth()
{
	m_update_widget_property();
}


void gridding_property_widget::slot_nugget()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_abutment()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_range()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_x_min_text()
{
	QString x_min_value = m_x_min_edit->text();
	QString x_max_value = m_x_max_edit->text();
	QString a = "(";
	a += x_min_value;
	a += ",";
	a += x_max_value;
	a += ")";
	m_x_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_x_max_text()
{
	QString x_min_value = m_x_min_edit->text();
	QString x_max_value = m_x_max_edit->text();
	QString a = "(";
	a += x_min_value;
	a += ",";
	a += x_max_value;
	a += ")";
	m_x_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_y_min_text()
{
	QString y_min_value = m_y_min_edit->text();
	QString y_max_value = m_y_max_edit->text();
	QString a = "(";
	a += y_min_value;
	a += ",";
	a += y_max_value;
	a += ")";
	m_y_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_y_max_text()
{
	QString y_min_value = m_y_min_edit->text();
	QString y_max_value = m_y_max_edit->text();
	QString a = "(";
	a += y_min_value;
	a += ",";
	a += y_max_value;
	a += ")";
	m_y_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_z_min_text()
{
	QString z_min_value = m_z_min_edit->text();
	QString z_max_value = m_z_max_edit->text();
	QString a = "(";
	a += z_min_value;
	a += ",";
	a += z_max_value;
	a += ")";
	m_z_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_z_max_text()
{
	QString z_min_value = m_z_min_edit->text();
	QString z_max_value = m_z_max_edit->text();
	QString a = "(";
	a += z_min_value;
	a += ",";
	a += z_max_value;
	a += ")";
	m_z_limit_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_resolution_Nx()
{
	QString Nx = QString::number(m_Nx_spinbox->value());
	QString Ny = QString::number(m_Ny_spinbox->value());
	QString Nz = QString::number(m_Nz_spinbox->value());
	QString a = "(";
	a += Nx;
	a += ",";
	a += Ny;
	a += ",";
	a += Nz;
	a += ")";
	m_resolution_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_resolution_Ny()
{
	QString Nx = QString::number(m_Nx_spinbox->value());
	QString Ny = QString::number(m_Ny_spinbox->value());
	QString Nz = QString::number(m_Nz_spinbox->value());
	QString a = "(";
	a += Nx;
	a += ",";
	a += Ny;
	a += ",";
	a += Nz;
	a += ")";
	m_resolution_treeitem->setText(1, a);
	m_update_widget_property();

}

void gridding_property_widget::slot_resolution_Nz()
{
	QString Nx = QString::number(m_Nx_spinbox->value());
	QString Ny = QString::number(m_Ny_spinbox->value());
	QString Nz = QString::number(m_Nz_spinbox->value());
	QString a = "(";
	a += Nx;
	a += ",";
	a += Ny;
	a += ",";
	a += Nz;
	a += ")";
	m_resolution_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_x_interval_text()
{
	QString interval_x = m_x_interval_edit->text();
	QString interval_y = m_y_interval_edit->text();
	QString interval_z = m_z_interval_edit->text();
	QString a = "(";
	a += interval_x;
	a += ",";
	a += interval_y;
	a += ",";
	a += interval_z;
	a += ")";
	m_interval_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_y_interval_text()
{
	QString interval_x = m_x_interval_edit->text();
	QString interval_y = m_y_interval_edit->text();
	QString interval_z = m_z_interval_edit->text();
	QString a = "(";
	a += interval_x;
	a += ",";
	a += interval_y;
	a += ",";
	a += interval_z;
	a += ")";
	m_interval_treeitem->setText(1, a);
	m_update_widget_property();
}

void gridding_property_widget::slot_z_interval_text()
{
	QString interval_x = m_x_interval_edit->text();
	QString interval_y = m_y_interval_edit->text();
	QString interval_z = m_z_interval_edit->text();
	QString a = "(";
	a += interval_x;
	a += ",";
	a += interval_y;
	a += ",";
	a += interval_z;
	a += ")";
	m_interval_treeitem->setText(1, a);
	m_update_widget_property();
}


void gridding_property_widget::slot_search_radius()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_ellipse_x_length()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_ellipse_y_length()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_ellipse_z_length()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_min_count()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_max_count()
{
	m_update_widget_property();
}

void gridding_property_widget::slot_start_mesh()
{
#if 0

	if (!this->m_progress){// yzw 0520
		this->m_progress = new QProgressDialog(this);
		this->m_progress->setMinimum(0);
		this->m_progress->setMaximum(0);
		this->m_progress->setFixedSize(400, 100);	// yzw 0607
		this->m_progress->setWindowTitle(STRING_TITLE_WAIT);
		this->m_progress->setOrientation(Qt::Horizontal);
		this->m_progress->setWindowModality(Qt::WindowModal);
		this->m_progress->setAttribute(Qt::WA_DeleteOnClose, true);
	}
	else
		m_progress->show(); 
#else
	//m_start_gridding_button->hide();
	//progressBar->show();
	//running();
#endif
	//开始网格化按钮点击时候的处理逻辑
//	if (this->m_pre_widget->NodeType == STRING_TYPE_SOURCE)  //"data source"只有前节点是数据源的时候，才会传递数据给网格化节点。
	{
		if (!node_running)  //只有未被网格化的时候，才会开始网格化，一旦已经网格化，后面点击就不会再次网格化操作
		{
			m_start_gridding_button->setText(STRING_STOP_BUTTON);
			running();
			node_running = true;
			//OperationalLayer::getOperationInstance().func_excut(this->id_key , close_progress, this);
			OperationalLayer::getOperationInstance().func_excut(this->id_key , std::bind(&gridding_property_widget::close_progress, this));
		}else{
			m_start_gridding_button->setText(STRING_START_BUTTON);
			node_running = false;	// flag to stop
		}
	}
}

void gridding_property_widget::end_progress()
{
	OperationalLayer::getOperationInstance().nextNodeRun(this->id_key);
	node_running = false;
	progressBar->hide();
	m_start_gridding_button->setText(STRING_START_BUTTON);
	m_start_gridding_button->show();
	emit  sig_vtk_adapt_win();
}

void gridding_property_widget::slot_set_progress(int r, int p, const char* m)
{
	/*m_start_gridding_button->hide();
	progressBar->show();
	progressBar->setRange(0, r);
	progressBar->setValue(p);
	progressBar->update();*/
	QString str = QString("%1%").arg(100 * p / r);
	setLabel(&str);
}

bool gridding_property_widget::set_progress(int r, int p, const char* m)
{
	emit  sig_set_progress(r, p, m);
	return node_running;
}

