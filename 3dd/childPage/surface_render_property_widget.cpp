// surface_render_property_widget.cpp

#include "surface_render_property_widget.h"
#include "myqwidget.h"

surface_render_property_widget::surface_render_property_widget(int num,G_SurfaceRender* surface_render)
{
	QString label = (surface_render) ? QString::fromStdString(surface_render->m_name) : ( (num == 0)? STRING_LABEL_SUFARENDER : STRING_LABEL_SUFARENDER + QString::number(num + 1));
	create_2buttons(this, label);
	 
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_normal_property->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_surface_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_SUFARENDER));
	m_normal_property->insertTopLevelItem(0, m_surface_render_treeitem);
	//m_input_component_treeitem = new QTreeWidgetItem(m_surface_render_treeitem, QStringList(QString("input component")));
	m_block_number_treeitem = new QTreeWidgetItem(m_surface_render_treeitem, QStringList(QString(STRING_ITEM_BLOCKNUM)));
	m_block_number_edit = new QLineEdit;
	m_block_number_edit->setText("0");
	m_normal_property->setItemWidget(m_block_number_treeitem, 1, m_block_number_edit);
	m_average_block_treeitem = new QTreeWidgetItem(m_surface_render_treeitem, QStringList(QString(STRING_ITEM_AVERBLOCK)));
	m_average_block_edit = new QLineEdit;
	m_average_block_edit->setText("0");
	m_normal_property->setItemWidget(m_average_block_treeitem, 1, m_average_block_edit);
	m_calculate_volume_treeitem = new QTreeWidgetItem(m_surface_render_treeitem, QStringList(QString(STRING_ITEM_CALCULATEVOL)));
	m_calculate_volume = new QCheckBox;
	m_normal_property->setItemWidget(m_calculate_volume_treeitem, 1, m_calculate_volume);
	m_volume_treeitem = new QTreeWidgetItem(m_surface_render_treeitem, QStringList(STRING_ITEM_VOLUME));
	m_volume_edit = new QLineEdit;
	m_volume_edit->setText(0);
	m_normal_property->setItemWidget(m_volume_treeitem, 1, m_volume_edit);
	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(QString(STRING_ITEM_RENDER)));
	m_normal_property->insertTopLevelItem(1, m_render_treeitem);
	m_show_border_treeitem = new QTreeWidgetItem(m_render_treeitem,QStringList(QString(STRING_ITEM_SHOWBORD)));
	m_show_border = new QCheckBox;
	m_normal_property->setItemWidget(m_show_border_treeitem, 1, m_show_border);
	m_border_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_BORDCOLOR)));
	m_border_color = new QComboBox;
	color_choose(m_border_color);
	m_normal_property->setItemWidget(m_border_color_treeitem, 1, m_border_color);
	m_gradient_chro_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_GRADCHRO)));
	m_gradient_chro = new QComboBox;
	gradient_color(m_gradient_chro);
	m_normal_property->setItemWidget(m_gradient_chro_treeitem, 1, m_gradient_chro);
	m_normal_property->expandAll();

	m_geometry_property = new QTreeWidget;
	m_geometry_property->setColumnCount(2);
	m_geometry_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_geometry_property->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_geometry_property_treeitem = new QTreeWidgetItem(m_geometry_property, QStringList(QString(STRING_ITEM_GEOMPROTY)));
	m_x_coordinate_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(QString(STRING_ITEM_XCOORLIM)));
	m_x_start_node_treeitem = new QTreeWidgetItem(m_x_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_XCOORNODE)));
	m_x_start_node_spin = new QSpinBox;
	m_x_start_node_spin->setRange(0, 50);
	m_x_start_node_spin->setValue(0);
	m_x_start_node_slider = new QSlider(Qt::Horizontal);
	m_x_start_node_slider->setRange(0, 50);
	m_x_start_node_slider->setValue(0);
	QWidget *x_start_node_widget = new QWidget;
	QHBoxLayout *x_start_node_layout = new QHBoxLayout;
	x_start_node_layout->addWidget(m_x_start_node_spin);
	x_start_node_layout->addWidget(m_x_start_node_slider);
	x_start_node_widget->setLayout(x_start_node_layout);
	m_geometry_property->setItemWidget(m_x_start_node_treeitem, 1, x_start_node_widget);
	m_x_end_node_treeitem = new QTreeWidgetItem(m_x_coordinate_limit_treeitem , QStringList(QString(STRING_ITEM_XCOORLNODE)));
	m_x_end_node_spin = new QSpinBox;
	m_x_end_node_spin->setRange(0, 50);
	m_x_end_node_spin->setValue(0);
	m_x_end_node_slider = new QSlider(Qt::Horizontal);
	m_x_end_node_slider->setRange(0, 50);
	m_x_end_node_slider->setValue(0);
	QWidget *x_end_node_widget = new QWidget;
	QHBoxLayout *x_end_node_layout = new QHBoxLayout;
	x_end_node_layout->addWidget(m_x_end_node_spin);
	x_end_node_layout->addWidget(m_x_end_node_slider);
	x_end_node_widget->setLayout(x_end_node_layout);
	m_geometry_property->setItemWidget(m_x_end_node_treeitem, 1,x_end_node_widget);
	m_x_single_slice_treeitem = new QTreeWidgetItem(m_x_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_SINGLSLICE)));
	m_x_single_slice = new QCheckBox;
	m_geometry_property->setItemWidget(m_x_single_slice_treeitem, 1, m_x_single_slice);
	m_y_coordinate_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(QString(STRING_ITEM_YCOORLIM)));
	m_y_start_node_treeitem = new QTreeWidgetItem(m_y_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_YCOORFNODE)));
	m_y_start_node_spin = new QSpinBox;
	m_y_start_node_spin->setRange(0, 50);
	m_y_start_node_spin->setValue(0);
	m_y_start_node_slider = new QSlider(Qt::Horizontal);
	m_y_start_node_slider->setRange(0, 50);
	m_y_start_node_slider->setValue(0);
	QWidget *y_start_node_widget = new QWidget;
	QHBoxLayout *y_start_node_layout = new QHBoxLayout;
	y_start_node_layout->addWidget(m_y_start_node_spin);
	y_start_node_layout->addWidget(m_y_start_node_slider);
	y_start_node_widget->setLayout(y_start_node_layout);
	m_geometry_property->setItemWidget(m_y_start_node_treeitem, 1, y_start_node_widget);
	m_y_end_node_treeitem = new QTreeWidgetItem(m_y_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_YCOORLNODE)));
	m_y_end_node_spin = new QSpinBox;
	m_y_end_node_spin->setRange(0, 50);
	m_y_end_node_spin->setValue(0);
	m_y_end_node_slider = new QSlider(Qt::Horizontal);
	m_y_end_node_slider->setRange(0, 50);
	m_y_end_node_slider->setValue(0);
	QWidget *y_end_node_widget = new QWidget;
	QHBoxLayout *y_end_node_layout = new QHBoxLayout;
	y_end_node_layout->addWidget(m_y_end_node_spin);
	y_end_node_layout->addWidget(m_y_end_node_slider);
	y_end_node_widget->setLayout(y_end_node_layout);
	m_geometry_property->setItemWidget(m_y_end_node_treeitem, 1, y_end_node_widget);
	m_y_single_slice_treeitem = new QTreeWidgetItem(m_y_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_SINGLSLICE)));
	m_y_single_slice = new QCheckBox;
	m_geometry_property->setItemWidget(m_y_single_slice_treeitem, 1, m_y_single_slice);
	m_z_coordinate_limit_treeitem = new QTreeWidgetItem(m_geometry_property_treeitem, QStringList(QString(STRING_ITEM_ZCOORLIM)));
	m_z_start_node_treeitem = new QTreeWidgetItem(m_z_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_ZCOORFNODE)));
	m_z_start_node_spin = new QSpinBox;
	m_z_start_node_spin->setRange(0, 50);
	m_z_start_node_spin->setValue(0);
	m_z_start_node_slider = new QSlider(Qt::Horizontal);
	m_z_start_node_slider->setRange(0, 50);
	m_z_start_node_slider->setValue(0);
	QWidget *z_start_node_widget = new QWidget;
	QHBoxLayout *z_start_node_layout = new QHBoxLayout;
	z_start_node_layout->addWidget(m_z_start_node_spin);
	z_start_node_layout->addWidget(m_z_start_node_slider);
	z_start_node_widget->setLayout(z_start_node_layout);
	m_geometry_property->setItemWidget(m_z_start_node_treeitem, 1, z_start_node_widget);
	m_z_end_node_treeitem = new QTreeWidgetItem(m_z_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_ZCOORLNODE)));
	m_z_end_node_spin = new QSpinBox;
	m_z_end_node_spin->setRange(0, 50);
	m_z_end_node_spin->setValue(0);
	m_z_end_node_slider = new QSlider(Qt::Horizontal);
	m_z_end_node_slider->setRange(0, 50);
	m_z_end_node_slider->setValue(0);
	QWidget *z_end_node_widget = new QWidget;
	QHBoxLayout *z_end_node_layout = new QHBoxLayout;
	z_end_node_layout->addWidget(m_z_end_node_spin);
	z_end_node_layout->addWidget(m_z_end_node_slider);
	z_end_node_widget->setLayout(z_end_node_layout);
	m_geometry_property->setItemWidget(m_z_end_node_treeitem, 1, z_end_node_widget);
	m_z_single_slice_treeitem = new QTreeWidgetItem(m_z_coordinate_limit_treeitem, QStringList(QString(STRING_ITEM_SINGLSLICE)));
	m_z_single_slice = new QCheckBox;
	m_geometry_property->setItemWidget(m_z_single_slice_treeitem, 1, m_z_single_slice);
	m_geometry_property->expandAll();

	m_color_scale = new color_scale_property;

	m_return_surface_render_struct = new G_SurfaceRender;
	baseStruct = m_return_surface_render_struct;
	if (!surface_render)
	{
		GSurfaceNode::getDefault(m_return_surface_render_struct);
	}
	else
		*m_return_surface_render_struct = *surface_render;
	surface_render = m_return_surface_render_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(surface_render->m_putIn));
		//m_input_component_treeitem->setText(1, QString::number(surface_render->m_putIndex));
		m_average_block_edit->setText(QString::number(surface_render->m_blockNum));
		m_calculate_volume->setChecked(surface_render->m_isCalculateVolume);
		m_show_border->setChecked(surface_render->m_render_surface.m_displayEdge);
		m_border_color->setCurrentText(crgb2name(surface_render->m_render_surface.m_edgeColor));
		m_gradient_chro->setCurrentIndex(surface_render->m_render_surface.m_gradientMap);
		m_x_start_node_spin->setValue(surface_render->m_geometric.m_xAxis.m_firstNodeOfAxis);
		m_x_start_node_slider->setValue(surface_render->m_geometric.m_xAxis.m_firstNodeOfAxis);
		m_x_end_node_spin->setValue(surface_render->m_geometric.m_xAxis.m_lastNodeOfAxis);
		m_x_end_node_slider->setValue(surface_render->m_geometric.m_xAxis.m_lastNodeOfAxis);
		m_x_single_slice->setChecked(surface_render->m_geometric.m_xAxis.m_singleSlice);
		m_y_start_node_spin->setValue(surface_render->m_geometric.m_yAxis.m_firstNodeOfAxis);
		m_y_start_node_slider->setValue(surface_render->m_geometric.m_yAxis.m_firstNodeOfAxis);
		m_y_end_node_spin->setValue(surface_render->m_geometric.m_yAxis.m_lastNodeOfAxis);
		m_y_end_node_slider->setValue(surface_render->m_geometric.m_yAxis.m_lastNodeOfAxis);
		m_y_single_slice->setChecked(surface_render->m_geometric.m_yAxis.m_singleSlice);
		m_z_start_node_spin->setValue(surface_render->m_geometric.m_zAxis.m_firstNodeOfAxis);
		m_z_start_node_slider->setValue(surface_render->m_geometric.m_zAxis.m_firstNodeOfAxis);
		m_z_end_node_spin->setValue(surface_render->m_geometric.m_zAxis.m_lastNodeOfAxis);
		m_z_end_node_slider->setValue(surface_render->m_geometric.m_zAxis.m_lastNodeOfAxis);
		m_z_single_slice->setChecked(surface_render->m_geometric.m_zAxis.m_singleSlice);

		m_color_scale->set_data(&surface_render->m_colorBar);
	}
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_geometry_property, STRING_TAB_GEOMETRY);
	m_form1_tab->addTab(m_color_scale, QString(STRING_TAB_COLOR_SCALE));

	this->NodeType = STRING_WIDGET_SURFACE;
	connect(m_btn_right, SIGNAL(clicked(bool)), this, SLOT(slot_ClickButtonDrawLine()));
	connect(m_block_number_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_block_number_edit()));
	connect(m_average_block_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_average_block_edit()));
	connect(m_calculate_volume, SIGNAL(stateChanged(int)), this, SLOT(slot_calculate_volume()));
	connect(m_show_border, SIGNAL(stateChanged(int)), this, SLOT(slot_show_border()));
	connect(m_border_color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_border_color()));
	connect(m_gradient_chro, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_gradient_color()));
	connect(m_x_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_x_start_slider(int)));
	connect(m_x_start_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_x_start_spin(int)));
	connect(m_x_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_x_end_slider(int)));
	connect(m_x_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_x_end_spin(int)));
	connect(m_x_single_slice, SIGNAL(stateChanged(int)), this, SLOT(slot_x_single_slice()));
	connect(m_y_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_y_start_slider(int)));
	connect(m_y_start_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_y_start_spin(int)));
	connect(m_y_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_y_end_slider(int)));
	connect(m_y_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_y_end_spin(int)));
	connect(m_y_single_slice, SIGNAL(stateChanged(int)), this, SLOT(slot_y_single_slice()));
	connect(m_z_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_z_start_slider(int)));
	connect(m_z_start_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_z_start_spin(int)));
	connect(m_z_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_z_end_slider(int)));
	connect(m_z_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_z_end_spin(int)));
	connect(m_z_single_slice, SIGNAL(stateChanged(int)), this, SLOT(slot_z_single_slice()));

	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar*, int)), this, SLOT(slot_color_scale_property_changed(Child_ColorBar*, int)));
}

surface_render_property_widget::~surface_render_property_widget()
{
	delete m_form1_tab;
}

void surface_render_property_widget::paintEvent(QPaintEvent *e)
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

G_SurfaceRender *surface_render_property_widget::return_surface_render_property_struct()
{
	getBaseStruct();
	m_return_surface_render_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	//m_return_surface_render_struct->m_putIndex = m_input_component_treeitem->text(1).toInt();
	m_return_surface_render_struct->m_blockNum = m_block_number_edit->text().toInt();
	m_return_surface_render_struct->m_meanBlock = m_average_block_edit->text().toFloat();
	m_return_surface_render_struct->m_isCalculateVolume = m_calculate_volume->isChecked();
	m_return_surface_render_struct->m_render_surface.m_displayEdge = m_show_border->isChecked();
	QColor color = QColor(m_border_color->currentText());
	m_return_surface_render_struct->m_render_surface.m_edgeColor.m_r = color.red();
	m_return_surface_render_struct->m_render_surface.m_edgeColor.m_g = color.green();
	m_return_surface_render_struct->m_render_surface.m_edgeColor.m_b = color.blue();
	m_return_surface_render_struct->m_render_surface.m_gradientMap = m_gradient_chro->currentIndex();
	m_return_surface_render_struct->m_geometric.m_xAxis.m_firstNodeOfAxis = m_x_start_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_xAxis.m_lastNodeOfAxis = m_x_end_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_xAxis.m_singleSlice = m_x_single_slice->isChecked();
	m_return_surface_render_struct->m_geometric.m_yAxis.m_firstNodeOfAxis = m_y_start_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_yAxis.m_lastNodeOfAxis = m_y_end_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_yAxis.m_singleSlice = m_y_single_slice->isChecked();
	m_return_surface_render_struct->m_geometric.m_zAxis.m_firstNodeOfAxis = m_z_start_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_zAxis.m_lastNodeOfAxis = m_z_end_node_spin->value();
	m_return_surface_render_struct->m_geometric.m_zAxis.m_singleSlice = m_z_single_slice->isChecked();

	return m_return_surface_render_struct;
}

void surface_render_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
	//QMenu *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input grid(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect input grid"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output geomery(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output geometry"), this));
	if (this->m_pre_widget)
	{
		menu->addAction(tr("connect input module(") + QString::fromStdString(this->m_pre_widget->get_label_text()) + QString(")"), this, SLOT(slot_connect_input()));
	}
	else
	{
		menu->addAction(tr("connect input module(") + QString("not connect") + QString(")"), this, SLOT(slot_connect_input()));
	}
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
#endif
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE,receiver1 , SLOT(delete_surface_render()));
	menu->move(cursor().pos());
	menu->show();
}

void surface_render_property_widget::delete_self()
{
	emit sig_delete_surface_render();
}

void surface_render_property_widget::m_update_widget_property()
{
	G_SurfaceRender* pTemp = this->return_surface_render_property_struct();
	std::shared_ptr<G_SurfaceRender>ptr(std::make_shared<G_SurfaceRender>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void surface_render_property_widget::slot_color_scale_property_changed(Child_ColorBar* ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*else if (1 == type)
		set_color_bar_actor(ccb->m_displayOn); */
	else{
		m_return_surface_render_struct->m_changetype = 5;
		G_SurfaceRender* pTemp = this->return_surface_render_property_struct();
		std::shared_ptr<G_SurfaceRender>ptr(std::make_shared<G_SurfaceRender>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}

void surface_render_property_widget::slot_block_number_edit()
{
	m_update_widget_property();
}

void surface_render_property_widget::slot_average_block_edit()
{
	m_update_widget_property();
}

void surface_render_property_widget::slot_calculate_volume()
{
	if (m_calculate_volume->isChecked())
	{
		m_return_surface_render_struct->m_changetype = 3;
		m_volume_treeitem->setDisabled(false);
		m_volume_edit->setEnabled(true);
		m_update_widget_property();
	}
	else
	{
		m_volume_treeitem->setDisabled(true);
		m_volume_edit->setText("0");
		m_volume_edit->setEnabled(false);
	}
}

void surface_render_property_widget::slot_show_border()
{
	m_return_surface_render_struct->m_changetype = 2;
	m_update_widget_property();
}

void surface_render_property_widget::slot_border_color()
{
	m_return_surface_render_struct->m_changetype = 2;
	m_update_widget_property();
}

void surface_render_property_widget::slot_gradient_color()
{
	m_update_widget_property();
}

void surface_render_property_widget::slot_x_start_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_x_start_node_slider, 0, 0, 0);
	if (a + 1 >= m_x_end_node_spin->value())
	{
		m_x_start_node_spin->setValue(m_x_end_node_spin->value() - 1);
		m_x_start_node_slider->setValue(m_x_end_node_spin->value() - 1);
	}
	else
		m_x_start_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_x_start_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_x_start_spin(int)));
}

void surface_render_property_widget::slot_x_start_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_x_start_node_spin, 0, 0, 0);
	if (a + 1 >= m_x_end_node_spin->value())
	{
		m_x_start_node_spin->setValue(m_x_end_node_spin->value() - 1);
		m_x_start_node_slider->setValue(m_x_end_node_spin->value() - 1);
	}
	else
		m_x_start_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_x_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_x_start_slider(int)));
}

void surface_render_property_widget::slot_x_end_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_x_end_node_slider, 0, 0, 0);
	if (a - 1 <= m_x_start_node_spin->value())
	{
		m_x_end_node_spin->setValue(m_x_start_node_spin->value() + 1);
		m_x_end_node_slider->setValue(m_x_start_node_spin->value() + 1);
	}
	else
		m_x_end_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_x_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_x_end_spin(int)));
}

void surface_render_property_widget::slot_x_end_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_x_end_node_spin, 0, 0, 0);
	if (a - 1 <= m_x_start_node_spin->value())
	{
		m_x_end_node_spin->setValue(m_x_start_node_spin->value() + 1);
		m_x_end_node_slider->setValue(m_x_start_node_spin->value() + 1);
	}
	else
		m_x_end_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_x_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_x_end_slider(int)));
}

void surface_render_property_widget::slot_x_single_slice()
{
	if (m_x_single_slice->isChecked())
	{
		m_x_end_node_spin->setEnabled(false);
		m_x_end_node_slider->setEnabled(false);
	}
	else
	{
		m_x_end_node_spin->setEnabled(true);
		m_x_end_node_slider->setEnabled(true);
	}
	m_return_surface_render_struct->m_changetype = 3;
	m_update_widget_property();
}

void surface_render_property_widget::slot_y_start_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_y_start_node_slider, 0, 0, 0);
	if (a + 1 >= m_y_end_node_spin->value())
	{
		m_y_start_node_spin->setValue(m_y_end_node_spin->value() - 1);
		m_y_start_node_slider->setValue(m_y_end_node_spin->value() - 1);
	}
	else
		m_y_start_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_y_start_node_slider, SIGNAL(valueChanged(int)), this, SIGNAL(slot_y_start_spin(int)));
}

void surface_render_property_widget::slot_y_start_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_y_start_node_spin, 0, 0, 0);
	if (a + 1 >= m_y_end_node_spin->value())
	{
		m_y_start_node_spin->setValue(m_y_end_node_spin->value() - 1);
		m_y_start_node_slider->setValue(m_y_end_node_spin->value() - 1);
	}
	else
		m_y_start_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_y_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_y_start_slider(int)));
}

void surface_render_property_widget::slot_y_end_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_y_end_node_slider, 0, 0, 0);
	if (a - 1 <= m_y_start_node_spin->value())
	{
		m_y_end_node_spin->setValue(m_y_start_node_spin->value() + 1);
		m_y_end_node_slider->setValue(m_y_start_node_spin->value() + 1);
	}
	else
		m_y_end_node_slider->setValue(a);
	m_y_start_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_y_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_y_end_spin(int)));
}

void surface_render_property_widget::slot_y_end_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_y_end_node_spin, 0, 0, 0);
	if (a - 1 <= m_y_start_node_spin->value())
	{
		m_y_end_node_spin->setValue(m_y_start_node_spin->value() + 1);
		m_y_end_node_slider->setValue(m_y_start_node_spin->value() + 1);
	}
	else
		m_y_end_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_y_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_y_end_slider(int)));
}

void surface_render_property_widget::slot_y_single_slice()
{
	if (m_y_single_slice->isChecked())
	{
		m_y_end_node_spin->setEnabled(false);
		m_y_end_node_slider->setEnabled(false);
	}
	else
	{
		m_y_end_node_spin->setEnabled(true);
		m_y_end_node_slider->setEnabled(true);
	}
	m_return_surface_render_struct->m_changetype = 3;
	m_update_widget_property();
}

void surface_render_property_widget::slot_z_start_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_z_start_node_slider, 0, 0, 0);
	if (a + 1 >= m_z_end_node_spin->value())
	{
		m_z_start_node_spin->setValue(m_z_end_node_spin->value() - 1);
		m_z_start_node_slider->setValue(m_z_end_node_spin->value() - 1);
	}
	else
		m_z_start_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_z_start_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_z_start_spin(int)));
}

void surface_render_property_widget::slot_z_start_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_z_start_node_spin, 0, 0, 0);
	if (a + 1 >= m_z_end_node_spin->value())
	{
		m_z_start_node_spin->setValue(m_z_end_node_spin->value() - 1);
		m_z_start_node_slider->setValue(m_z_end_node_spin->value() - 1);
	}
	else
		m_z_start_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_z_start_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_z_start_slider(int)));
}
void surface_render_property_widget::slot_z_end_slider(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_z_end_node_slider, 0, 0, 0);
	if (a - 1 <= m_z_start_node_spin->value())
	{
		m_z_end_node_spin->setValue(m_z_start_node_spin->value() + 1);
		m_z_end_node_slider->setValue(m_z_start_node_spin->value() + 1);
	}
	else
		m_z_end_node_slider->setValue(a);
	m_update_widget_property();
	connect(m_z_end_node_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_z_end_spin(int)));
}

void surface_render_property_widget::slot_z_end_spin(int a)
{
	m_return_surface_render_struct->m_changetype = 3;
	disconnect(m_z_end_node_spin, 0, 0, 0);
	if (a - 1 <= m_z_start_node_spin->value())
	{
		m_z_end_node_spin->setValue(m_z_start_node_spin->value() + 1);
		m_z_end_node_slider->setValue(m_z_start_node_spin->value() + 1);
	}
	else
		m_z_end_node_spin->setValue(a);
	m_update_widget_property();
	connect(m_z_end_node_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_z_end_slider(int)));
}

void surface_render_property_widget::slot_z_single_slice()
{
	if (m_x_single_slice->isChecked())
	{
		m_z_end_node_spin->setEnabled(false);
		m_z_end_node_slider->setEnabled(false);
	}
	else
	{
		m_z_end_node_spin->setEnabled(true);
		m_z_end_node_slider->setEnabled(true);
	}
	m_return_surface_render_struct->m_changetype = 3;
	m_update_widget_property();
}

void surface_render_property_widget::xyz_range(int*a, void*p)
{
	surface_render_property_widget *b = (surface_render_property_widget*)p;
	disconnect(b->m_x_end_node_spin, 0, 0, 0); disconnect(b->m_x_end_node_slider,0,0,0);
	disconnect(b->m_y_end_node_spin, 0, 0, 0); disconnect(b->m_y_end_node_slider, 0, 0, 0);
	disconnect(b->m_z_end_node_spin, 0, 0, 0); disconnect(b->m_z_end_node_slider, 0, 0, 0);
	b->m_x_end_node_spin->setRange(0, a[0]-1); //b->m_x_end_node_spin->setValue(a[0]); b->m_x_end_node_slider->setValue(a[0]);
	b->m_y_end_node_spin->setRange(0, a[1]-1); //b->m_y_end_node_spin->setValue(a[1]); b->m_y_end_node_slider->setValue(a[1]);
	b->m_z_end_node_spin->setRange(0, a[2]-1); //b->m_z_end_node_spin->setValue(a[2]); b->m_z_end_node_slider->setValue(a[2]);
	connect(b->m_x_end_node_spin, SIGNAL(valueChanged(int)), b, SLOT(slot_x_end_slider(int)));
	connect(b->m_x_end_node_slider, SIGNAL(valueChanged(int)), b, SLOT(slot_x_end_spin(int)));
	connect(b->m_y_end_node_spin, SIGNAL(valueChanged(int)), b, SLOT(slot_y_end_slider(int)));
	connect(b->m_y_end_node_slider, SIGNAL(valueChanged(int)), b, SLOT(slot_y_end_spin(int)));
	connect(b->m_z_end_node_spin, SIGNAL(valueChanged(int)), b, SLOT(slot_z_end_slider(int)));
	connect(b->m_z_end_node_slider, SIGNAL(valueChanged(int)), b, SLOT(slot_z_end_spin(int)));

}

void surface_render_property_widget::update_property(int num_blocks, float average_blocks, float volume, void* widget)
{
	surface_render_property_widget *b = (surface_render_property_widget*)widget;
	disconnect(b->m_block_number_edit, 0, 0, 0);
	disconnect(b->m_average_block_edit, 0, 0, 0);
	b->m_block_number_edit->setText(QString::number(num_blocks));
	b->m_average_block_edit->setText(QString("%1").arg(average_blocks));
	if (b->m_calculate_volume->isChecked())
		b->m_volume_edit->setText(QString("%1").arg(volume));
	else
		b->m_calculate_volume->setText("0");
	connect(b->m_block_number_edit, SIGNAL(textChanged(QString)), b, SLOT(slot_block_number_edit()));
	connect(b->m_average_block_edit, SIGNAL(textChanged(QString)), b, SLOT(slot_average_block_edit()));

}
