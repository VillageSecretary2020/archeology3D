#include "subset_property_widget.h"
//#include "myqwidget.h"
#include "CSubsetNode.h"

subset_property_widget::subset_property_widget(int num, SuperNodeStruct* param)
{
	QString label =  (num == 0)? STRING_LABEL_SUBSET : STRING_LABEL_SUBSET + QString::number(num + 1);
	create_2buttons(this, label);
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_subset_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_SUBSET));
	m_input_resolution_treeitem = new QTreeWidgetItem(m_subset_treeitem, QStringList(STRING_ITEM_INPUTRESOL));
	//m_input_resolution_edit = new QLineEdit;
	//m_normal_property->setItemWidget(m_input_resolution_treeitem, 1, m_input_resolution_edit);
	m_output_component_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_OPTCOMPON));
	m_data_component_treeitem = new QTreeWidgetItem(m_output_component_treeitem, QStringList(STRING_ITEM_DATACOMPON));
	m_normal_property->expandAll();

	m_geometry = new QTreeWidget;
	m_geometry->setColumnCount(2);
	m_geometry->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_geometry_treeitem = new QTreeWidgetItem(m_geometry, QStringList(STRING_ITEM_GEOMPROTY));
	m_input_calculate_treeitem = new QTreeWidgetItem(m_geometry_treeitem, QStringList(STRING_ITEM_IPTCALCUL));
	m_input_calculate_check = new QCheckBox;
	m_geometry->setItemWidget(m_input_calculate_treeitem, 1, m_input_calculate_check);
	m_x_limit_treeitem = new QTreeWidgetItem(m_geometry_treeitem, QStringList(STRING_ITEM_XLIMIT));
	m_x_start_node_treeitem = new QTreeWidgetItem(m_x_limit_treeitem, QStringList(STRING_ITEM_XSTARTNODE));
	m_x_start_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_x_start_node_treeitem, 1, m_x_start_node_spinbox);
	m_x_end_node_treeitem = new QTreeWidgetItem(m_x_limit_treeitem, QStringList(STRING_ITEM_XENDNODE));
	m_x_end_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_x_end_node_treeitem, 1, m_x_end_node_spinbox);
	m_x_step_treeitem = new QTreeWidgetItem(m_x_limit_treeitem, QStringList(STRING_ITEM_XSTEP));
	m_x_step_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_x_step_treeitem, 1, m_x_step_spinbox);

	m_y_limit_treeitem = new QTreeWidgetItem(m_geometry_treeitem, QStringList(STRING_ITEM_YLIMIT));
	m_y_start_node_treeitem = new QTreeWidgetItem(m_y_limit_treeitem, QStringList(STRING_ITEM_YSTARTNODE));
	m_y_start_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_y_start_node_treeitem, 1, m_y_start_node_spinbox);
	m_y_end_node_treeitem = new QTreeWidgetItem(m_y_limit_treeitem, QStringList(STRING_ITEM_YENDNODE));
	m_y_end_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_y_end_node_treeitem, 1, m_y_end_node_spinbox);
	m_y_step_treeitem = new QTreeWidgetItem(m_y_limit_treeitem, QStringList(STRING_ITEM_YSTEP));
	m_y_step_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_y_step_treeitem, 1, m_y_step_spinbox);

	m_z_limit_treeitem = new QTreeWidgetItem(m_geometry_treeitem, QStringList(STRING_ITEM_ZLIMIT));
	m_z_start_node_treeitem = new QTreeWidgetItem(m_z_limit_treeitem, QStringList(STRING_ITEM_ZSTARTNODE));
	m_z_start_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_z_start_node_treeitem, 1, m_z_start_node_spinbox);
	m_z_end_node_treeitem = new QTreeWidgetItem(m_z_limit_treeitem, QStringList(STRING_ITEM_ZENDNODE));
	m_z_end_node_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_z_end_node_treeitem, 1, m_z_end_node_spinbox);
	m_z_step_treeitem = new QTreeWidgetItem(m_z_limit_treeitem, QStringList(STRING_ITEM_ZSTEP));
	m_z_step_spinbox = new QSpinBox;
	m_geometry->setItemWidget(m_z_step_treeitem, 1, m_z_step_spinbox);
	m_geometry->expandAll();

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}"); // 界面边框
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_geometry, STRING_TAB_GEOMETRY);

	C_SubsetFilter* subset;
	baseStruct = new C_SubsetFilter;
	if (!param) {
		CSubsetNode::getDefault(baseStruct);
		subset = (C_SubsetFilter*) baseStruct;
	}else
		subset = (C_SubsetFilter*)param;
	{
		m_x_start_node_spinbox->setRange(0, subset->m_xEnd);
		 m_x_start_node_spinbox->setValue(subset->m_xStart);
		 m_x_end_node_spinbox->setRange(0, subset->m_xEnd);
		 m_x_end_node_spinbox->setValue(subset->m_xEnd);
		 m_x_step_spinbox->setValue(subset->m_xStep);

		m_y_start_node_spinbox->setRange(0, subset->m_yEnd);
		 m_y_start_node_spinbox->setValue(subset->m_yStart);
		 m_y_end_node_spinbox->setRange(0, subset->m_yEnd);
		 m_y_end_node_spinbox->setValue(subset->m_yEnd);
		 m_y_step_spinbox->setValue(subset->m_yStep);

		m_z_start_node_spinbox->setRange(0, subset->m_zEnd);
		 m_z_start_node_spinbox->setValue(subset->m_zStart);
		 m_z_end_node_spinbox->setRange(0, subset->m_zEnd);
		 m_z_end_node_spinbox->setValue(subset->m_zEnd);
		 m_z_step_spinbox->setValue(subset->m_zStep);
	}
	this->NodeType = STRING_CTYPE_SUBSET;
	CAN_APPEND_WIDGET_GTYPE_ALL(m_can_append_widget);

	m_can_append_widget.push_back(STRING_CTYPE_TRANSFORM);
	m_can_append_widget.push_back(STRING_CTYPE_SUBSET);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_EXCLU);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_REPEAT);

	connect(m_input_calculate_check, SIGNAL(stateChanged(int)), this, SLOT(slot_input_calculate()));
	connect(m_x_start_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_x_end_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_x_step_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_y_start_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_y_end_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_y_step_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_z_start_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_z_end_node_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_z_step_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
}

void subset_property_widget::paintEvent(QPaintEvent *e)
{
#if 0
	QPainter p(this);
	p.setPen(Qt::black);
	p.setBrush(Qt::gray);
	static const QPointF points[4] = {
		QPointF(10, 0),
		QPointF(this->width() - 10, 0),
		QPointF(this->width() - 10, 40),
		QPointF(10,40)
	};
	p.drawConvexPolygon(points, 4);
#endif 

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


subset_property_widget::~subset_property_widget()
{
	delete m_form1_tab;
	delete baseStruct;
}

void subset_property_widget::contextMenuEvent(QContextMenuEvent  *event)
{
	QMenu *menu = new QMenu(this);
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAPHIC);
#if 1
	MENU_RENDER_CALC_NODE(out_put, receiver1);
#else
	out_put->addAction(tr("shape render"),			receiver1, SLOT(create_shape_render()));
	out_put->addAction(tr("sactter diagram"),			receiver1, SLOT(create_scatter()));
	out_put->addAction(STRING_ACTION_ORTHOGRAPH,	receiver1, SLOT(create_orthogonal()));
	out_put->addAction(tr("streamline diagram"),	receiver1, SLOT(create_streamline()));
	out_put->addAction(STRING_ACTION_VECTDIAGRAM,			receiver1 , SLOT(create_vector()));
	out_put->addAction(tr("contour map"),				this, SLOT(()));
	out_put->addAction(STRING_ACTION_ISOSURDIAG,			receiver1, SLOT(create_isosurface()));
	out_put->addAction(STRING_ACTION_SUFARENDER,			receiver1, SLOT(create_surface_render()));
	out_put->addAction(tr("axis"),							receiver1, SLOT(slot_subset_to_axis()));	//slot_create_border
	out_put->addAction(tr("bounding box"),			receiver1, SLOT(slot_create_border()));
	out_put->addAction(tr("hign and low area"),		receiver1, SLOT(create_highlowmap()));
	//QMenu *normal = menu->addMenu(tr("normal module"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	QMenu *calc = menu->addMenu(STRING_CALC_MENU);	// todo
	calc->addAction(tr("slice"), this, SLOT(slot_create_mess()));
	calc->addAction(STRING_ACTION_TRANSFORM, this, SLOT(slot_create_mess()));
//	calc->addAction(tr("merge"), this, SLOT(slot_create_mess()));
	calc->addAction(STRING_ACTION_SUBSET, this, SLOT(slot_create_mess()));
	calc->addAction(tr("extract drawing point"), this, SLOT(slot_create_mess()));
	calc->addAction(tr("change type"), this, SLOT(slot_create_mess()));
	calc->addAction(tr("math"), this, SLOT(slot_create_mess()));
	calc->addAction(tr("gradient"), this, SLOT(slot_create_mess()));
	calc->addAction(tr("filter"), this, SLOT(slot_create_mess()));
	calc->addAction(tr("resample"), this, SLOT(slot_create_mess()));
#endif
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input grid(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect input grid(not connect)"), this));
	//menu->addAction(new QAction(tr("connect output data"), this));
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_subset()) );
	menu->move(cursor().pos());
	menu->show();
}

void subset_property_widget::delete_self()
{
	emit sig_delete_ubset();
}

void subset_property_widget::showInputResolution(int res[3])
{
	QString str = QString("%1, %2, %3").arg(res[0]).arg(res[1]).arg( res[2]);
	m_input_resolution_treeitem->setText(1, str);
}

void subset_property_widget::m_update_widget_property()
{
	C_SubsetFilter* pTemp = (C_SubsetFilter* )return_property_struct();
	std::shared_ptr<C_SubsetFilter>ptr(std::make_shared<C_SubsetFilter>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void subset_property_widget::slot_update_widget_property()
{
	m_update_widget_property();
}

SuperNodeStruct* subset_property_widget::return_property_struct()
{
	getBaseStruct();
	C_SubsetFilter* m_property_struct = ( C_SubsetFilter*)baseStruct;
	m_property_struct->m_xStart = m_x_start_node_spinbox->text().toInt();
	m_property_struct->m_xEnd = m_x_end_node_spinbox->text().toInt();
	m_property_struct->m_xStep = m_x_step_spinbox->text().toInt();

	m_property_struct->m_yStart = m_y_start_node_spinbox->text().toInt();
	m_property_struct->m_yEnd = m_y_end_node_spinbox->text().toInt();
	m_property_struct->m_yStep = m_y_step_spinbox->text().toInt();

	m_property_struct->m_zStart = m_z_start_node_spinbox->text().toInt();
	m_property_struct->m_zEnd = m_z_end_node_spinbox->text().toInt();
	m_property_struct->m_zStep = m_z_step_spinbox->text().toInt();
	return m_property_struct;
}

void subset_property_widget::slot_subset_to_axis()
{
	emit sig_subset_to_axis();
}

void subset_property_widget::slot_subset_to_border()
{
	emit sig_subset_to_border();
}

void subset_property_widget::slot_input_calculate()
{

}

void subset_property_widget::slot_x_start_node()
{

}

void subset_property_widget::slot_x_end_node()
{

}

void subset_property_widget::slot_x_step()
{

}

void subset_property_widget::slot_y_start_node()
{

}

void subset_property_widget::slot_y_end_node()
{

}

void subset_property_widget::slot_y_step()
{

}

void subset_property_widget::slot_z_start_node()
{

}

void subset_property_widget::slot_z_end_node()
{

}

void subset_property_widget::slot_z_step()
{

}
