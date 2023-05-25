#include "transform_property_widget.h"
#include "myqwidget.h"

transform_property_widget::transform_property_widget(int num, C_Conversion* transform)
{
	QString label = (transform) ? QString::fromStdString(transform->m_name) : ( (num == 0)? STRING_LABEL_TRANSFORM : STRING_LABEL_TRANSFORM + QString::number(num + 1));
	create_2buttons(this, label);
	
	//常规属性
	m_normal_tree = new QTreeWidget;
	m_normal_tree->setColumnCount(2);
	m_normal_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_pre_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_transform_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_TRANSLATE));
	m_origin_treeitem = new QTreeWidgetItem(m_transform_treeitem, QStringList(STRING_ITEM_ORIGIN));
	m_origin_combox = new QComboBox;
	m_origin_combox->insertItem(0, STRING_COMBOX_CENTER);
	m_origin_combox->insertItem(1, STRING_COMBOX_LEFTDOWN);
	m_origin_combox->insertItem(2, STRING_COMBOX_RIGHTUPON);
	m_normal_tree->setItemWidget(m_origin_treeitem, 1, m_origin_combox);
	m_input_x_treeitem = new QTreeWidgetItem(m_origin_treeitem, QStringList("x"));
	m_input_y_treeitem = new QTreeWidgetItem(m_origin_treeitem, QStringList("y"));
	m_input_z_treeitem = new QTreeWidgetItem(m_origin_treeitem, QStringList("z"));
	 m_translale_x_edit= new QLineEdit;
	 m_normal_tree->setItemWidget(m_input_x_treeitem, 1, m_translale_x_edit);
	 m_translale_y_edit= new QLineEdit;
	 m_normal_tree->setItemWidget(m_input_y_treeitem, 1, m_translale_y_edit);
	 m_translale_z_edit= new QLineEdit;
	 m_normal_tree->setItemWidget(m_input_z_treeitem, 1, m_translale_z_edit);
	m_normal_tree->expandAll();

	//变换属性
	m_transform_tree = new QTreeWidget;
	m_transform_tree->setColumnCount(2);
	m_scale_factor_treeitem = new QTreeWidgetItem(m_transform_tree, QStringList(STRING_ITEM_SCALEFACTOR));
	m_scale_x_treeitem = new QTreeWidgetItem(m_scale_factor_treeitem, QStringList("x"));
	m_scale_x_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_scale_x_treeitem, 1, m_scale_x_edit);
	m_scale_y_treeitem = new QTreeWidgetItem(m_scale_factor_treeitem, QStringList("y"));
	m_scale_y_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_scale_y_treeitem, 1, m_scale_y_edit);
	m_scale_z_treeitem = new QTreeWidgetItem(m_scale_factor_treeitem, QStringList("z"));
	m_scale_z_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_scale_z_treeitem, 1, m_scale_z_edit);
	m_rotate_treeitem = new QTreeWidgetItem(m_transform_tree, QStringList(STRING_ITEM_ROTATE));
	m_rotate_x_treeitem = new QTreeWidgetItem(m_rotate_treeitem, QStringList(STRING_ITEM_XCOMPONT));
	m_rotate_x_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_rotate_x_treeitem, 1, m_rotate_x_edit);
	m_rotate_y_treeitem = new QTreeWidgetItem(m_rotate_treeitem, QStringList(STRING_ITEM_YCOMPONT));
	m_rotate_y_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_rotate_y_treeitem, 1, m_rotate_y_edit);
	m_rotate_z_treeitem = new QTreeWidgetItem(m_rotate_treeitem, QStringList(STRING_ITEM_ZCOMPONT));
	m_rotate_z_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_rotate_z_treeitem, 1, m_rotate_z_edit);
	m_angle_treeitem = new QTreeWidgetItem(m_rotate_treeitem, QStringList(STRING_ITEM_ANGLE));
	m_angle_spin = new QDoubleSpinBox;
	m_angle_spin->setDecimals(2);
	m_angle_spin->setValue(0);
	m_angle_spin->setRange(-360, 360);
	m_angle_slider = new QSlider(Qt::Horizontal);
	m_angle_slider->setRange(0, 72000);
	m_angle_slider->setValue(36000);
	QWidget *m_angle_widget = new QWidget;
	QHBoxLayout *m_angle_layout = new QHBoxLayout;
	m_angle_layout->addWidget(m_angle_spin);
	m_angle_layout->addWidget(m_angle_slider);
	m_angle_widget->setLayout(m_angle_layout);
	m_transform_tree->setItemWidget(m_angle_treeitem, 1, m_angle_widget);

	m_transform_treeitem = new QTreeWidgetItem(m_transform_tree,QStringList(STRING_ITEM_TRANSFORM));
	m_transform_x_treeitem = new QTreeWidgetItem(m_transform_treeitem, QStringList("x"));
	m_transform_x_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_transform_x_treeitem, 1, m_transform_x_edit);
	m_transform_y_treeitem = new QTreeWidgetItem(m_transform_treeitem, QStringList("y"));
	m_transform_y_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_transform_y_treeitem, 1, m_transform_y_edit);
	m_transform_z_treeitem = new QTreeWidgetItem(m_transform_treeitem, QStringList("z"));
	m_transform_z_edit = new QLineEdit;
	m_transform_tree->setItemWidget(m_transform_z_treeitem, 1, m_transform_z_edit);
	m_transform_tree->expandAll();

	m_return_transform_struct = new C_Conversion();
	baseStruct = m_return_transform_struct;

	if (!transform) {
		CConversionNode::getDefault(baseStruct);
	}
	else {
		*m_return_transform_struct = *transform;
	}
	transform = m_return_transform_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(transform->m_putIn));
		if (transform->m_origin == CENTERPOINT)
		{
			m_origin_combox->setCurrentIndex(0);
		}
		else if (transform->m_origin == LEFT_BOTTOM)
		{
			m_origin_combox->setCurrentIndex(1);
		}
		else
		{
			m_origin_combox->setCurrentIndex(2);
		}
			
		m_translale_x_edit->setText( QString("%1").arg(transform->m_pos.m_x));
		m_translale_y_edit->setText( QString("%1").arg(transform->m_pos.m_y));
		m_translale_z_edit->setText( QString("%1").arg(transform->m_pos.m_z));
		m_scale_x_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_scaleFactor.m_xScaleFactor));
		m_scale_y_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_scaleFactor.m_yScaleFactor));
		m_scale_z_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_scaleFactor.m_zScaleFactor));
		m_rotate_x_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_rotate.m_xRotate));
		m_rotate_y_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_rotate.m_yRotate));
		m_rotate_z_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_rotate.m_zRotate));
		m_angle_spin->setValue(transform->m_conversionAttribute.m_rotate.m_angle);
		m_angle_slider->setValue((int)(transform->m_conversionAttribute.m_rotate.m_angle * 100) + 36000);
		m_transform_x_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_exchange.m_xExchange));
		m_transform_y_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_exchange.m_yExchange));
		m_transform_z_edit->setText(QString("%1").arg(transform->m_conversionAttribute.m_exchange.m_zExchange));
	}
	//m_return_transform_struct = new C_Conversion();
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_tree, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_transform_tree, STRING_ITEM_TRANSPRO);

	this->NodeType = STRING_CTYPE_TRANSFORM;
	//CAN_APPEND_WIDGET_GTYPE_ALL(m_can_append_widget);
	this->m_can_append_widget.push_back(STRING_WIDGET_SCATTER);//("scatter_widget");
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);
	this->m_can_append_widget.push_back(STRING_GTYPE_HIGH_LOW);

	this->m_can_append_widget.push_back(STRING_CTYPE_TRANSFORM);
	this->m_can_append_widget.push_back(STRING_CTYPE_SUBSET);
	this->m_can_append_widget.push_back(STRING_CTYPE_FILTER_EXCLU);
	this->m_can_append_widget.push_back(STRING_CTYPE_FILTER_REPEAT);
	this->m_can_append_widget.push_back(STRING_CTYPE_GRIDDING);

	connect(m_origin_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_origin_combox()));
	connect(m_translale_x_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_x()));
	connect(m_translale_y_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_y()));
	connect(m_translale_z_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_z()));
	connect(m_scale_x_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_x()));
	connect(m_scale_y_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_y()));
	connect(m_scale_z_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_scale_z()));
	connect(m_rotate_x_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_rotate_x()));
	connect(m_rotate_y_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_rotate_y()));
	connect(m_rotate_z_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_rotate_z()));
	connect(m_angle_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_angle_slider(double)));
	connect(m_angle_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_angle_spin(int)));
	connect(m_transform_x_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_transform_x()));
	connect(m_transform_y_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_transform_y()));
	connect(m_transform_z_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_transform_z()));
}

transform_property_widget::~transform_property_widget()
{
	delete m_form1_tab;
}

void transform_property_widget::paintEvent(QPaintEvent *e)
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

void transform_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMenu *menu = new QMenu(this);
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAPHIC);	
#if 1
	if (m_pre_widget->NodeType==STRING_CTYPE_GRIDDING)
	{
		MENU_RENDER_CALC_NODE(out_put, receiver1);
	}
	else
	{
		out_put->addAction(STRING_ACTION_SACTDIAGRAM, receiver1, SLOT(create_scatter()));
		out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
		//out_put->addAction(STRING_ACTION_HIANLOREA, receiver1, SLOT(create_highlowmap()));
	}
#else
	MENU_RENDER_CALC_NODE(out_put, receiver1);
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(tr("slice diagram"), this, SLOT(slot_transform_to_slice()));
	out_put->addAction(tr("shape render"), this, SLOT(slot_transform_to_shape_render()));
	out_put->addAction(tr("scatter diagram"), this, SLOT(slot_transform_to_scatter()));
	out_put->addAction(STRING_ACTION_ORTHOGRAPH, this, SLOT(slot_transform_to_orthogonal()));
	out_put->addAction(STRING_ACTION_ISOLINEDIAG, this, SLOT(slot_transform_to_isoline()));
	out_put->addAction(STRING_ACTION_ISOSURDIAG, this, SLOT(slot_transform_to_isosurface()));
	out_put->addAction(STRING_ACTION_SUFARENDER, this, SLOT(slot_transform_to_surface()));
	out_put->addAction(tr("axis"), this, SLOT(slot_transform_to_axis()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_transform_to_border()));
	//QMenu *normal = menu->addMenu(tr("normal module"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
#endif
	if (m_pre_widget->NodeType==STRING_CTYPE_GRIDDING)
	{
	}
	else
	{
		QMenu *calc = menu->addMenu(QString(STRING_MENU_CALC));
		calc->addAction(STRING_ACTION_TRANSFORM, receiver1, SLOT(create_transform()));
		//calc->addAction(STRING_ACTION_SUBSET, receiver1, SLOT(create_subset()));
		calc->addAction(STRING_ACTION_EXCLUFILTER, receiver1, SLOT(create_exclusion_filter()));
		calc->addAction(STRING_ACTION_REPEATFILT, receiver1, SLOT(create_repeat_filter()));
		calc->addAction(STRING_ACTION_GRIDDING, receiver1, SLOT(create_gridding()));
	}
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input data(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect input data(not connect)"), this));
	//menu->addAction(new QAction(tr("connect output data"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output geometry(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output geometry"), this));
	menu->addAction(STRING_ACTION_CONN_IN,		this,			SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_CONN_OUT,	this,			SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_EXPORT,		this,			SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY,			this,			SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME,		this,			SLOT(slot_rename_control()));
	menu->addAction( STRING_ACTION_DELETE,		receiver1,	SLOT(delete_transform()));
	menu->move(cursor().pos());
	menu->show();
}

void transform_property_widget::delete_self()
{
	emit sig_delete_transform();
}

void transform_property_widget::m_update_widget_property()
{
	C_Conversion* pTemp = this->return_transform_property_struct();
	std::shared_ptr<C_Conversion>ptr(std::make_shared<C_Conversion>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

C_Conversion* transform_property_widget::return_transform_property_struct()
{
	getBaseStruct();
	m_return_transform_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	int origin_index = m_origin_combox->currentIndex();
	if (origin_index == 0)
	{
		m_return_transform_struct->m_origin = CENTERPOINT;
	}
	else if (origin_index == 1)
	{
		m_return_transform_struct->m_origin = LEFT_BOTTOM;
	}
	else
	{
		m_return_transform_struct->m_origin = RIGHT_TOP;
	}

	m_return_transform_struct->m_pos.m_x = m_translale_x_edit->text().toFloat();
	m_return_transform_struct->m_pos.m_y = m_translale_y_edit->text().toFloat();
	m_return_transform_struct->m_pos.m_z = m_translale_z_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_scaleFactor.m_xScaleFactor = m_scale_x_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_scaleFactor.m_yScaleFactor = m_scale_y_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_scaleFactor.m_zScaleFactor = m_scale_z_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_rotate.m_xRotate = m_rotate_x_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_rotate.m_yRotate = m_rotate_y_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_rotate.m_zRotate = m_rotate_z_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_rotate.m_angle = m_angle_spin->value();
	m_return_transform_struct->m_conversionAttribute.m_exchange.m_xExchange = m_transform_x_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_exchange.m_yExchange = m_transform_y_edit->text().toFloat();
	m_return_transform_struct->m_conversionAttribute.m_exchange.m_zExchange = m_transform_z_edit->text().toFloat();
	return m_return_transform_struct;
}

void transform_property_widget::slot_transform_to_slice()
{
	emit sig_transform_to_slice();
}

void transform_property_widget::slot_transform_to_shape_render()
{
	emit sig_transform_to_shape_render();
}

void transform_property_widget::slot_transform_to_scatter()
{
	emit sig_transform_to_scatter();
}

void transform_property_widget::slot_transform_to_orthogonal()
{
	emit sig_transform_to_orthogonal();
}

void transform_property_widget::slot_transform_to_isoline()
{
	emit sig_transform_to_isoline();
}

void transform_property_widget::slot_transform_to_isosurface()
{
	emit sig_transform_to_isosurface();
}

void transform_property_widget::slot_transform_to_surface()
{
	emit sig_transform_to_surface();
}

void transform_property_widget::slot_transform_to_axis()
{
	emit sig_transform_to_axis();
}

void transform_property_widget::slot_transform_to_border()
{
	emit sig_transform_to_border();
}

void transform_property_widget::slot_transform_to_transform()
{
	emit sig_transform_to_transform();
}

void transform_property_widget::slot_transform_to_subset()
{
	emit sig_transform_to_subset();
}

void transform_property_widget::slot_transform_to_exclusion_filter()
{
	emit sig_transform_to_exclusion_filter();
}

void transform_property_widget::slot_create_gridding()
{
	emit sig_transform_to_gridding();
}

void transform_property_widget::slot_create_repeat_filter()
{
	emit sig_transform_to_repeat_filter();
}

void transform_property_widget::slot_origin_combox()
{
	m_update_widget_property();
}

void transform_property_widget::slot_scale_x()
{
	m_update_widget_property();
}

void transform_property_widget::slot_scale_y()
{
	m_update_widget_property();
}

void transform_property_widget::slot_scale_z()
{
	m_update_widget_property();
}

void transform_property_widget::slot_rotate_x()
{
	m_update_widget_property();
}

void transform_property_widget::slot_rotate_y()
{
	m_update_widget_property();
}

void transform_property_widget::slot_rotate_z()
{
	m_update_widget_property();
}

void transform_property_widget::slot_angle_slider(double a)
{
	disconnect(m_angle_slider, 0, 0, 0);
	int value = (int)(a * 100) + 36000;
	m_angle_slider->setValue(value);
	m_update_widget_property();
	connect(m_angle_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_angle_spin(int)));

}

void transform_property_widget::slot_angle_spin(int a)
{
	disconnect(m_angle_spin, 0, 0, 0);
	double value = (a - 36000)*0.01;
	m_angle_spin->setValue(value);
	m_update_widget_property();
	connect(m_angle_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_angle_slider(double)));
}

void transform_property_widget::slot_transform_x()
{
	m_update_widget_property();
}

void transform_property_widget::slot_transform_y()
{
	m_update_widget_property();
}

void transform_property_widget::slot_transform_z()
{
	m_update_widget_property();
}
