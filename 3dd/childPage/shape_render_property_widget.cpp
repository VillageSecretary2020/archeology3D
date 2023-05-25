#include "shape_render_property_widget.h"


shape_render_property_widget::shape_render_property_widget(int num, G_VolumeRender *volume)
{
	QString label = (volume) ? QString::fromStdString(volume->m_name) : ((num == 0) ? STRING_LABEL_VOLUME : STRING_LABEL_VOLUME + QString::number(num + 1));
	create_2buttons(this, label);

	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	//m_shape_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(tr("shape render")));
	//m_input_component_treeitem = new QTreeWidgetItem(m_shape_render_treeitem, QStringList(QString("input component")));
	//m_input_component_treeitem->setText(1, "1");
	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_RENDER));
	m_color_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_COMPOCOLOR));
	m_interpolation_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_INTERMETHOD));
	m_render_mode_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_RENDERMODE));
	m_sample_distance_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_SAMPDISTANCE));
	m_averange_range_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_SCALARANGE));
//	m_color_type_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_COLORTYPE));


	m_gradient_chro_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_GRADCHRO));
	m_opacity_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_OPACITY));
	m_composition_color_method = new QComboBox;
	m_composition_color_method->insertItem(0, STRING_COMBOX_ALPHAMETHOD);
	m_composition_color_method->insertItem(1, STRING_COMBOX_MAXMETHOD);
	m_composition_color_method->insertItem(2, STRING_COMBOX_MINMETHOD);
	m_composition_color_method->insertItem(3, STRING_COMBOX_AVERGMETHOD);
	m_composition_color_method->insertItem(4, STRING_COMBOX_ADDMETHOD);
	m_composition_color_method->setCurrentIndex(2);
	m_normal_property->setItemWidget(m_color_method_treeitem, 1, m_composition_color_method);

	m_render_mode_combox = new QComboBox;
	m_render_mode_combox->insertItem(0, STRING_COMBOX_DEFAULT);
	m_render_mode_combox->insertItem(1, STRING_COMBOX_RAYCAST);
	m_render_mode_combox->insertItem(2, STRING_COMBOX_GPU);
	//m_render_mode_combox->insertItem(3, STRING_COMBOX_OSPRAY);
	m_normal_property->setItemWidget(m_render_mode_treeitem, 1, m_render_mode_combox);
	m_sample_distance_spin = new QDoubleSpinBox;
	m_sample_distance_spin->setValue(1.0);
	m_sample_distance_spin->setRange(-100, 100);
	m_normal_property->setItemWidget(m_sample_distance_treeitem, 1, m_sample_distance_spin);
	m_range_0 = new QLineEdit; m_range_1 = new QLineEdit;
	m_range_0->setText("100");
	m_range_1->setText("200");
	QHBoxLayout *m_layout = new QHBoxLayout;
	QWidget *m_widget = new QWidget;
	m_layout->addWidget(m_range_0); m_layout->addWidget(m_range_1);
	m_widget->setLayout(m_layout);
	m_normal_property->setItemWidget(m_averange_range_treeitem, 1, m_widget);
	m_averange_range_treeitem->setHidden(true);

	m_color_type_combox = new QComboBox;
	m_color_type_combox->insertItem(0, STRING_COMBOX_GRADIENT);
	m_color_type_combox->insertItem(1, STRING_COMBOX_CUSTOM);
//	m_normal_property->setItemWidget(m_color_type_treeitem, 1, m_color_type_combox);

	m_interpolation_method = new QComboBox;
	m_interpolation_method->insertItem(0, STRING_COMBOX_NEARMETHOD);
	m_interpolation_method->insertItem(1, STRING_COMBOX_LINERMETHOD);
	m_interpolation_method->insertItem(2, STRING_COMBOX_THREELINEAR);
	m_interpolation_method->setCurrentIndex(1);
	m_normal_property->setItemWidget(m_interpolation_treeitem, 1, m_interpolation_method);
	m_gradient_combox = new QComboBox;
	gradient_color(m_gradient_combox);
	m_normal_property->setItemWidget(m_gradient_chro_treeitem, 1, m_gradient_combox);
	//m_gradient_chro_treeitem->setHidden(true);
	m_opacity_spinbox = new QDoubleSpinBox;
	m_opacity_spinbox->setRange(0, 1);
	m_opacity_spinbox->setValue(1);
	m_opacity_spinbox->setDecimals(4);
	m_opacity_spinbox->setSingleStep(0.1);
	m_opacity_slider = new QSlider(Qt::Horizontal);
	m_opacity_slider->setRange(0, 10000);
	m_opacity_slider->setValue(10000);
	QWidget *render_widget = new QWidget;
	QHBoxLayout *render_layout = new QHBoxLayout;
	render_layout->addWidget(m_opacity_spinbox);
	render_layout->addWidget(m_opacity_slider);
	render_widget->setLayout(render_layout);
	m_normal_property->setItemWidget(m_opacity_treeitem, 1, render_widget);
	//m_normal_property->insertTopLevelItem(0, m_shape_render_treeitem);
	m_normal_property->insertTopLevelItem(1, m_render_treeitem);
	m_normal_property->expandAll();

	m_color_scale = new color_scale_property();
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_color_scale, STRING_TAB_COLOR_SCALE);

	m_return_shape_render_property_struct = new G_VolumeRender;
	baseStruct = m_return_shape_render_property_struct;

	if (!volume) {
		GVolumeRenderNode::getDefault(m_return_shape_render_property_struct);
	}
	else
		*m_return_shape_render_property_struct = *volume;

	volume = m_return_shape_render_property_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(volume->m_putIn));
		//m_input_component_treeitem->setText(1, QString::number(volume->m_putIndex));

		m_composition_color_method->setCurrentIndex(volume->m_method);
		{
			m_interpolation_method->setCurrentIndex(volume->renderMethod.m_insertMethod);
			m_render_mode_combox->setCurrentIndex(volume->renderMethod.m_renderMode);

			/*if (volume->renderMethod.m_colormethod == CUSTORM_OF_COLOR)//GRADIENT_OF_COLOR
			{
				m_color_type_combox->setCurrentIndex(1);
				m_gradient_chro_treeitem->setHidden(true);
			}
			else*/
			{
				m_gradient_combox->setHidden(false);
				//m_color_type_combox->setCurrentIndex(0);
				m_gradient_combox->setCurrentIndex(volume->renderMethod.m_gradientMap);
			}
			m_range_0->setText(QString("%1").arg(volume->renderMethod.m_averageIPScalarRange[0]));
			m_range_1->setText(QString("%1").arg(volume->renderMethod.m_averageIPScalarRange[1]));
			m_sample_distance_spin->setValue(volume->renderMethod.m_sampleDistance);
			m_opacity_spinbox->setValue(volume->renderMethod.m_opacity);
			m_opacity_slider->setValue(m_opacity_spinbox->value() * 10000);

			m_color_scale->set_data(&volume->renderMethod.m_colorBar);
		}
	}
	connect(m_composition_color_method, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_composition_color_method()));
	connect(m_render_mode_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_render_mode()));
	connect(m_sample_distance_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_sample_distance()));
	connect(m_range_0, SIGNAL(editingFinished()), this, SLOT(slot_range_min()));
	connect(m_range_1, SIGNAL(editingFinished()), this, SLOT(slot_range_max()));
	//connect(m_color_type_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_colot_type()));
	connect(m_interpolation_method, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_interpolation_method()));
	connect(m_gradient_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_gradient_combox()));
	connect(m_opacity_spinbox, SIGNAL(valueChanged(double)), this, SLOT(m_opacity_slider_setValue(double)));
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_opacity_spin_setValue(int)));

	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar* , int )), this, SLOT(slot_color_scale_property_changed(Child_ColorBar* , int )));

	this->NodeType = STRING_WIDGET_SHAPE;
	this->m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);
}

shape_render_property_widget::~shape_render_property_widget()
{
	delete m_form1_tab;
}

void shape_render_property_widget::paintEvent(QPaintEvent *e)
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

void shape_render_property_widget::m_update_widget_property()
{
	//myqwidget* widget = (myqwidget*)this->parentWidget();
	G_VolumeRender* pTemp = this->return_shape_render_struct();
	std::shared_ptr<G_VolumeRender>ptr(std::make_shared<G_VolumeRender>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}


G_VolumeRender *shape_render_property_widget::return_shape_render_struct()
{
	getBaseStruct();	// get name, and pos from widget
	m_return_shape_render_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_shape_render_property_struct->m_putIndex = 0;// m_input_component_treeitem->text(1).toInt();
	
	m_return_shape_render_property_struct->m_method  = (RenderMethod)m_composition_color_method->currentIndex();
	{
	 m_return_shape_render_property_struct->renderMethod.m_insertMethod= (Child_InsertMethod_VRender)m_interpolation_method->currentIndex();

	m_return_shape_render_property_struct->renderMethod.m_renderMode	= (RenderMode)m_render_mode_combox->currentIndex();

	m_return_shape_render_property_struct->renderMethod.m_sampleDistance = m_sample_distance_spin->value();
	m_return_shape_render_property_struct->renderMethod.m_averageIPScalarRange[0] = m_range_0->text().toFloat();
	m_return_shape_render_property_struct->renderMethod.m_averageIPScalarRange[1] = m_range_1->text().toFloat();
	
	int color_type_value = 0;// m_color_type_combox->currentIndex();
	if (color_type_value == 0)
		m_return_shape_render_property_struct->renderMethod.m_colormethod = GRADIENT_OF_COLOR;
	else
		m_return_shape_render_property_struct->renderMethod.m_colormethod = CUSTORM_OF_COLOR;

	m_return_shape_render_property_struct->renderMethod.m_gradientMap = m_gradient_combox->currentIndex();
	m_return_shape_render_property_struct->renderMethod.m_opacity = m_opacity_spinbox->value();
	}

	return m_return_shape_render_property_struct;
}

void shape_render_property_widget::contextMenuEvent(QContextMenuEvent *event)
{
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
	//QMenu  *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	if (this->m_pre_widget)
	{
		menu->addAction(QString("connect input module(") + QString::fromStdString(this->m_pre_widget->get_label_text()) + QString(")"), this,SLOT(slot_connect_input()));
	}
	else
	{
		menu->addAction(QString("connect input module(not connect)"), this,SLOT(slot_connect_input()));
	}
	menu->addAction(tr("connect output module"), this,SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
#endif
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_shape_render())); 

	menu->move(cursor().pos());
	menu->show();
}

void shape_render_property_widget::delete_self()
{
	emit sig_delete_shape_render();
}

void shape_render_property_widget::slot_range_max()
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_range_min()
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_sample_distance()
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_render_mode()
{
	m_return_shape_render_property_struct->m_changetype = 3;
	m_update_widget_property();
}

void shape_render_property_widget::slot_colot_type()
{
	if (m_color_type_combox->currentIndex() == CUSTORM_OF_COLOR)
	{
		m_gradient_chro_treeitem->setHidden(true);
	}
	else
		m_gradient_chro_treeitem->setHidden(false);
	m_update_widget_property();
}

void shape_render_property_widget::slot_shape_to_axis()
{
	emit sig_shape_to_axis();
}

void shape_render_property_widget::slot_shape_to_border()
{
	emit  sig_shape_to_border();
}


void shape_render_property_widget::slot_composition_color_method()
{
	m_return_shape_render_property_struct->m_changetype = 3;
	if (m_composition_color_method->currentIndex() == 3)
		m_averange_range_treeitem->setHidden(false);
	else
		m_averange_range_treeitem->setHidden(true);
	m_update_widget_property();
}

void shape_render_property_widget::slot_interpolation_method()
{
	m_update_widget_property();
}

void shape_render_property_widget::m_opacity_slider_setValue(double a)
{
	disconnect(m_opacity_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_opacity_slider->setValue(value);
	m_update_widget_property();
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_opacity_spin_setValue(int)));
}

void shape_render_property_widget::m_opacity_spin_setValue(int a)
{
	disconnect(m_opacity_spinbox, 0, 0, 0);
	double value = a / 10000.0;
	m_opacity_spinbox->setValue(value);
	m_update_widget_property();
	connect(m_opacity_spinbox, SIGNAL(valueChanged(double)), this, SLOT(m_opacity_slider_setValue(double)));
}

void shape_render_property_widget::slot_show_color_scale(int a)
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_place_direction(int a)
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_m_x_position_slider(double a)
{
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void shape_render_property_widget::slot_m_x_position_spin(int a)
{
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void shape_render_property_widget::slot_m_y_position_slider(double a)
{
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void shape_render_property_widget::slot_m_y_position_spin(int a)
{
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void shape_render_property_widget::slot_m_width_slider(double a)
{
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void shape_render_property_widget::slot_m_width_spin(int a)
{
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void shape_render_property_widget::slot_m_length_slider(double a)
{
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void shape_render_property_widget::slot_m_length_spin(int a)
{
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void shape_render_property_widget::slot_m_font_size_slider(int a)
{
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void shape_render_property_widget::slot_m_font_size_spin(int a)
{
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void shape_render_property_widget::slot_m_note_number(int a)
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_m_note_font_size_slider(int a)
{
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = a;
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void shape_render_property_widget::slot_m_note_font_size_spin(int a)
{
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	int value = a ;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_slider(int)));
}

void shape_render_property_widget::slot_m_adopt_self_note(int a)
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_color_type(int a)
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_significant(int a)
{
	m_update_widget_property();
}


void shape_render_property_widget::slot_gradient_combox()
{
	m_update_widget_property();
}

void shape_render_property_widget::slot_color_scale_property_changed(Child_ColorBar*ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*else if (1 == type) {
		set_color_bar_actor(ccb->m_displayOn);
	}*/
	else {
		m_return_shape_render_property_struct->m_changetype = 5;
		G_VolumeRender* pTemp = this->return_shape_render_struct();
		std::shared_ptr<G_VolumeRender>ptr(std::make_shared<G_VolumeRender>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}

