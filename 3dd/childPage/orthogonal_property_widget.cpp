#include "orthogonal_property_widget.h"
#include "GOrthogonal_factor.h"
orthogonal_property_widget::orthogonal_property_widget(int num,G_Orthogonal_Factor* orthogonal)
{
	QString label = (orthogonal) ? QString::fromStdString(orthogonal->m_name) : ( (num == 0)? STRING_LABEL_ORTHOGONAL : STRING_LABEL_ORTHOGONAL + QString::number(num + 1));
	create_2buttons(this, label);
	 
	m_NormalProperty = new QTreeWidget;
	m_NormalProperty->setColumnCount(2);
	m_NormalProperty->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_orthogonal_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString(STRING_ITEM_ORTHOGDIA)));
	//m_input_component_treeitem = new QTreeWidgetItem(m_orthogonal_treeitem, QStringList(QString("input component")));
	//m_input_component_edit = new QLineEdit(this);
	//m_input_component_edit->setText("1");
	//connect(m_input_component_edit, SIGNAL(editingFinished()), this, SLOT(slot_input_component()));
	//m_NormalProperty->setItemWidget(m_input_component_treeitem, 1, m_input_component_edit);
	m_place_direction_treeitem = new QTreeWidgetItem(m_orthogonal_treeitem, QStringList(QString(STRING_ITEM_PLACEDIRTION)));
	m_PlaceDirection = new QComboBox;
	m_PlaceDirection->insertItem(0, "xy");
	m_PlaceDirection->insertItem(1, "xz");
	m_PlaceDirection->insertItem(2, "yz");
	m_NormalProperty->setItemWidget(m_place_direction_treeitem, 1, m_PlaceDirection);
	m_slice_number_treeitem = new QTreeWidgetItem(m_orthogonal_treeitem, QStringList(QString(STRING_ITEM_SLICENUM)));
	m_slice_number_spin = new QSpinBox;
	m_slice_number_spin->setRange(1, 40);
	m_slice_number_spin->setValue(1);
	m_slice_number_slider = new QSlider(Qt::Horizontal);
	m_slice_number_slider->setRange(1, 50);
	m_slice_number_slider->setValue(1);
	QWidget *m_slice_number_widget = new QWidget;
	QHBoxLayout *m_slice_number_layout = new QHBoxLayout;
	m_slice_number_layout->addWidget(m_slice_number_spin);
	m_slice_number_layout->addWidget(m_slice_number_slider);
	m_slice_number_widget->setLayout(m_slice_number_layout);
	m_NormalProperty->setItemWidget(m_slice_number_treeitem, 1, m_slice_number_widget);

	m_render_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString(STRING_ITEM_RENDER)));
	//m_lighting_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("lighing")));
	//m_Lighting = new QCheckBox;
	//m_Lighting->setChecked(true);
	//connect(m_Lighting, SIGNAL(stateChanged(int)), this, SLOT(slot_lighting()));
	//m_NormalProperty->setItemWidget(m_lighting_treeitem, 1, m_Lighting);
	//m_opacity_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("opacity")));
	//m_Opacity_spin = new QDoubleSpinBox;
	//m_Opacity_spin->setRange(0, 1);
	//m_Opacity_spin->setDecimals(3);
	//m_Opacity_spin->setSingleStep(0.001);
	//m_Opacity_spin->setValue(1);
	//m_Opacity_slider = new QSlider(Qt::Horizontal);
	//m_Opacity_slider->setRange(0, 1000);
	//m_Opacity_slider->setValue(1000);
	//connect(m_Opacity_spin, SIGNAL(valueChanged(double)), this, SLOT(m_opacity_slider_setValue(double)));
	//connect(m_Opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_opacity_spin_setValue(int)));
	//QWidget* opacity_widget = new QWidget;
	//QHBoxLayout*  opacity_layout = new QHBoxLayout;
	//opacity_layout->addWidget(m_Opacity_spin);
	//opacity_layout->addWidget(m_Opacity_slider);
	//opacity_widget->setLayout(opacity_layout);
	//m_NormalProperty->setItemWidget(m_opacity_treeitem, 1, opacity_widget);
	m_color_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_COLORMETHOD)));
	m_color_method_combox = new QComboBox;
	gradient_color(m_color_method_combox);
	m_NormalProperty->setItemWidget(m_color_method_treeitem, 1, m_color_method_combox);
	m_NormalProperty->insertTopLevelItem(0, m_orthogonal_treeitem);
	m_NormalProperty->insertTopLevelItem(1, m_render_treeitem);
	m_NormalProperty->expandAll();

	m_color_scale = new color_scale_property;
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_return_orthogonal_struct = new G_Orthogonal_Factor;
	baseStruct = m_return_orthogonal_struct;
	if (!orthogonal)
	{	//	默认参数
		GOrthonalFactorNode::getDefault(m_return_orthogonal_struct);
	}
	else
		*m_return_orthogonal_struct = *orthogonal;
	orthogonal = m_return_orthogonal_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(orthogonal->m_putIn));
		//m_input_component_treeitem->setText(1, QString::number(orthogonal->m_putIndex));
		if (orthogonal->m_direction == D_XY)
			m_PlaceDirection->setCurrentIndex(0);
		else if (orthogonal->m_direction == D_XZ)
			m_PlaceDirection->setCurrentIndex(1);
		else
			m_PlaceDirection->setCurrentIndex(2);
		//m_Lighting->setChecked(orthogo>m)
		//m_Opacity_spin->setValue(orthogonal->m_)
		//m_color_method_treeitem->setText(1,)
#if 0
#else
		m_color_scale->set_data(&orthogonal->m_colorBar);
#endif
	}


	//m_return_orthogonal_struct = new G_Orthogonal_Factor;
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black;}");
	m_form1_tab->addTab(m_NormalProperty, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_color_scale, STRING_TAB_COLOR_SCALE);

	connect(m_PlaceDirection, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_place_direction()));
	connect(m_slice_number_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_slider(int)));
	connect(m_slice_number_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_spin(int)));
	connect(m_color_method_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_method()));

	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar*, int)), this, SLOT(slot_color_scale_property_changed(Child_ColorBar*, int)));

	this->NodeType = STRING_WIDGET_ORTHOGO;
	this->m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);
}

orthogonal_property_widget::~orthogonal_property_widget()
{
	delete m_form1_tab;
}


void orthogonal_property_widget::paintEvent(QPaintEvent *e)
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

void orthogonal_property_widget::setResolution(int * slt)
{
	memcpy_s(resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	place_direction();
}

void orthogonal_property_widget::place_direction()
{
	int a = resolution[2 - m_PlaceDirection->currentIndex()];
	disconnect(m_slice_number_spin, 0, 0, 0);
	m_slice_number_spin->setRange(1, a);
	connect(m_slice_number_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_slider(int)));

	disconnect(m_slice_number_slider, 0, 0, 0);
	m_slice_number_slider->setRange(1, a);
	connect(m_slice_number_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_spin(int)));
}

G_Orthogonal_Factor* orthogonal_property_widget::return_orthogonal_struct()
{
	getBaseStruct();
	m_return_orthogonal_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_orthogonal_struct->m_putIndex = 0;// m_input_component_edit->text().toInt();
	//m_return_orthogonal_struct->m_gradientMap = m_color_method_combox->currentIndex();
	if (m_PlaceDirection->currentIndex() == 0)
		m_return_orthogonal_struct->m_direction = D_XY;
	else if (m_PlaceDirection->currentIndex() == 1)
		m_return_orthogonal_struct->m_direction = D_XZ;
	else
		m_return_orthogonal_struct->m_direction = D_YZ;
	m_return_orthogonal_struct->m_gradientMap = m_color_method_combox->currentIndex();
	m_return_orthogonal_struct->m_setId = m_slice_number_spin->value();

	return m_return_orthogonal_struct;
}

void orthogonal_property_widget::contextMenuEvent(QContextMenuEvent *event)
{
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
	//QMenu  *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input module(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
		//menu->addAction(new QAction(tr("connect input module"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output module(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output module"), this));
	if (this->m_pre_widget)
	{
		menu->addAction(tr("connect input module(") + QString::fromStdString(this->m_pre_widget->get_label_text()) + QString(")"), this, SLOT(slot_connect_input()));
	}
	else
	{
		menu->addAction(tr("connect input module(") + QString("not connect") + QString(")"), this, SLOT(slot_connect_input()));
	}
	menu->addAction(tr("connect output module"), this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
#endif
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	//menu->addAction(new QAction(tr("delete"), this，SLOT(delete_self_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_orthogonal()));
	menu->move(cursor().pos());
	menu->show();
}

void orthogonal_property_widget::delete_self()
{
	emit sig_delete_orthogonal();
}

void orthogonal_property_widget::m_update_widget_property()
{
	//myqwidget* widget = (myqwidget*)this->parentWidget();
	G_Orthogonal_Factor* pTemp = this->return_orthogonal_struct();
	std::shared_ptr<G_Orthogonal_Factor>ptr(std::make_shared<G_Orthogonal_Factor>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void orthogonal_property_widget::slot_input_component()
{
	m_return_orthogonal_struct->m_changetype = 1;
	m_update_widget_property();
}

void orthogonal_property_widget::slot_place_direction()
{
	m_return_orthogonal_struct->m_changetype = 2;
	place_direction();
	m_update_widget_property();
}

void orthogonal_property_widget::slot_slice_number_slider(int a)
{
	m_return_orthogonal_struct->m_changetype = 3;
	disconnect(m_slice_number_slider, 0, 0, 0);
	m_slice_number_slider->setValue(a);
	m_update_widget_property();
	connect(m_slice_number_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_spin(int)));
}

void orthogonal_property_widget::slot_slice_number_spin(int a)
{
	m_return_orthogonal_struct->m_changetype = 3;
	disconnect(m_slice_number_spin, 0, 0, 0);
	m_slice_number_spin->setValue(a);
	m_update_widget_property();
	connect(m_slice_number_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_slice_number_slider(int)));
}

#if 0
void orthogonal_property_widget::slot_lighting()
{

}
#endif 

#if 0
void orthogonal_property_widget::m_opacity_slider_setValue(double a)
{
	disconnect(m_Opacity_slider, 0, 0, 0);
	int value = (int)(a * 1000);
	m_Opacity_slider->setValue(value);
	connect(m_Opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_opacity_spin_setValue(int)));
}
#endif 

#if 0
void orthogonal_property_widget::m_opacity_spin_setValue(int a)
{
	disconnect(m_Opacity_spin, 0, 0, 0);
	double value = a / 1000.0;
	m_Opacity_spin->setValue(value);
	connect(m_Opacity_spin, SIGNAL(valueChanged(double)), this, SLOT(m_opacity_slider_setValue(double)));
}
#endif

void orthogonal_property_widget::slot_color_method()
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_show_color_scale(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_place_direction(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_m_x_position_slider(double a)
{
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void orthogonal_property_widget::slot_m_x_position_spin(int a)
{
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void orthogonal_property_widget::slot_m_y_position_slider(double a)
{
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void orthogonal_property_widget::slot_m_y_position_spin(int a)
{
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void orthogonal_property_widget::slot_m_width_slider(double a)
{
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void orthogonal_property_widget::slot_m_width_spin(int a)
{
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void orthogonal_property_widget::slot_m_length_slider(double a)
{
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void orthogonal_property_widget::slot_m_length_spin(int a)
{
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void orthogonal_property_widget::slot_m_font_size_slider(int a)
{
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void orthogonal_property_widget::slot_m_font_size_spin(int a)
{
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void orthogonal_property_widget::slot_m_note_number(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_m_note_font_size_slider(double a)
{
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void orthogonal_property_widget::slot_m_note_font_size_spin(int a)
{
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_note_font_size_slider(double)));
}

void orthogonal_property_widget::slot_m_adopt_self_note(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_color_type(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_significant(int a)
{
	m_update_widget_property();
}

void orthogonal_property_widget::slot_color_scale_property_changed(Child_ColorBar* ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*else if (1 == type)
		set_color_bar_actor(ccb->m_displayOn); */
	else {
		m_return_orthogonal_struct->m_changetype = 5;
		G_Orthogonal_Factor* pTemp = this->return_orthogonal_struct();
		std::shared_ptr<G_Orthogonal_Factor>ptr(std::make_shared<G_Orthogonal_Factor>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}

