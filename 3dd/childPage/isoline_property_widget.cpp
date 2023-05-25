#include "isoline_property_widget.h"
#include "GIsoLineNode.h"
#include "myqwidget.h"

isoline_property_widget::isoline_property_widget(int num,G_IsoLine* isoline)
{
	m_return_isoline_property_struct = new G_IsoLine;
	baseStruct = m_return_isoline_property_struct;
	QString label = (isoline) ? QString::fromStdString(isoline->m_name) : ( (num == 0)? STRING_LABEL_ISOLINE : STRING_LABEL_ISOLINE + QString::number(num + 1));
	create_2buttons(this, label);
	 
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	
	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	//m_isoline_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(QString("isoline")));
	//m_input_data_limit_treeitem = new QTreeWidgetItem(m_isoline_treeitem, QStringList(QString("inout data limit")));
	m_level_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(QString(STRING_ITEM_LEVEL)));//level
	m_level_calculate_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(QString(STRING_ITEM_LEVELCAL)));
	m_level_calculate = new QComboBox;
	m_level_calculate->insertItem(0, STRING_COMBOX_AUTO);
	m_level_calculate->insertItem(1, STRING_COMBOX_MINMAXCOUN);
	m_level_calculate->insertItem(2, STRING_COMBOX_MINMAXINTR);
	m_level_calculate->insertItem(3, STRING_COMBOX_EXPLICIT);
	m_normal_property->setItemWidget(m_level_calculate_treeitem, 1, m_level_calculate);
	m_min_level_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(STRING_ITEM_MINLEV));
	m_min_level_edit = new QLineEdit;
	m_normal_property->setItemWidget(m_min_level_treeitem, 1, m_min_level_edit);
	m_min_level_treeitem->setDisabled(true); m_min_level_edit->setDisabled(true);
	m_max_level_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(STRING_ITEM_MAXLEV));
	m_max_level_edit = new QLineEdit;
	m_normal_property->setItemWidget(m_max_level_treeitem, 1, m_max_level_edit);
	m_max_level_treeitem->setDisabled(true); m_max_level_edit->setDisabled(true);
	m_level_number_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(STRING_ITEM_LEVNUM));
	m_level_number_spinbox = new QSpinBox;
	m_normal_property->setItemWidget(m_level_number_treeitem, 1, m_level_number_spinbox);
	m_level_interval_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(STRING_ITEM_LEVINT));
	m_level_interval_edit = new QLineEdit;
	m_normal_property->setItemWidget(m_level_interval_treeitem, 1, m_level_interval_edit);
	m_level_interval_treeitem->setHidden(true);
	m_classify_treeitem = new QTreeWidgetItem(m_level_treeitem, QStringList(STRING_ITEM_CLASSIFY));
	m_classify_edit = new QLineEdit;
	m_normal_property->setItemWidget(m_classify_treeitem, 1, m_classify_edit);
	m_classify_treeitem->setHidden(true);
	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(QString(STRING_ITEM_RENDER)));
	m_line_width_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_LINEWIDTH)));
	m_line_width_spin = new QSpinBox;
	m_line_width_spin->setRange(0, 10);
	m_line_width_spin->setValue(2);
	m_line_width_slider = new QSlider(Qt::Horizontal);
	m_line_width_slider->setRange(0, 10);
	m_line_width_slider->setValue(2);
	QWidget *line_width_widget = new QWidget;
	QHBoxLayout *line_width_layout = new QHBoxLayout;
	line_width_layout->addWidget(m_line_width_spin);
	line_width_layout->addWidget(m_line_width_slider);
	line_width_widget->setLayout(line_width_layout);
	m_normal_property->setItemWidget(m_line_width_treeitem, 1, line_width_widget);
	m_gradient_chro_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_GRADCHRO)));
	m_gradient_chromatography = new QComboBox;
	gradient_color(m_gradient_chromatography);
	m_normal_property->setItemWidget(m_gradient_chro_treeitem, 1, m_gradient_chromatography);
	m_show_border_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_SHOWSLICE)));
	m_show_border = new QCheckBox;
	m_show_border->setChecked(true);
	m_normal_property->setItemWidget(m_show_border_treeitem, 1, m_show_border);
	m_border_width_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_BORDWIDTH)));
	m_border_width_spin = new QSpinBox;
	m_border_width_spin->setValue(1);
	m_border_width_spin->setRange(0, 10);
	m_border_width_slider = new QSlider(Qt::Horizontal);
	m_border_width_slider->setRange(0, 10);
	m_border_width_slider->setValue(1);
	QWidget *border_width_widget = new QWidget;
	QHBoxLayout *border_width_layout = new QHBoxLayout;
	border_width_layout->addWidget(m_border_width_spin);
	border_width_layout->addWidget(m_border_width_slider);
	border_width_widget->setLayout(border_width_layout);
	m_normal_property->setItemWidget(m_border_width_treeitem, 1, border_width_widget);
	m_border_width_treeitem->setHidden(1);
	m_border_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_BORDCOLOR)));
	m_border_color = new QComboBox;
	color_choose(m_border_color);
	m_normal_property->setItemWidget(m_border_color_treeitem, 1, m_border_color);
	m_border_color_treeitem->setHidden(1);
	m_normal_property->expandAll();

	m_cut_plane = new cut_plane_property;
	m_cut_plane->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_color_scale = new color_scale_property;
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	if (!isoline)
	{
		GIsoLineNode::getDefault(m_return_isoline_property_struct);
	}
	else {
		*m_return_isoline_property_struct = *isoline;
	}
		isoline = m_return_isoline_property_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(isoline->m_putIn));
		//m_input_data_limit_treeitem->setText(1,isoline->m_)
		m_level_calculate->setCurrentIndex(isoline->m_level);
		m_level_number_spinbox->setValue(isoline->m_type.m_typeAuto.m_count);
		m_min_level_edit->setText(QString("%1").arg(isoline->m_type.m_typeInterval.m_minLevel));
		m_max_level_edit->setText(QString("%1").arg(isoline->m_type.m_typeInterval.m_maxLevel));
		m_level_interval_edit->setText(QString("%1").arg(isoline->m_type.m_typeInterval.m_interval));
		m_classify_edit->setText(QString("%1").arg(isoline->m_type.m_typeExplicit.m_levelValue));
		m_line_width_spin->setValue(isoline->m_render.m_lineWidth);
		m_line_width_slider->setValue(isoline->m_render.m_lineWidth);
		m_gradient_chromatography->setCurrentIndex(isoline->m_render.m_gradientMap);
		m_show_border->setChecked(isoline->m_render.display_bound);
		m_border_width_spin->setValue(isoline->m_render.m_boundWidth);
		m_border_width_slider->setValue(isoline->m_render.m_boundWidth);
		m_border_color->setCurrentText(crgb2name(isoline->m_render.m_boundRgb));
#if 0
#else
		m_cut_plane->set_data(&isoline->m_cutPlane, 1000, 500);
		m_color_scale->set_data(&isoline->m_render.m_colorBar);
#endif
	}
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_cut_plane, STRING_TAB_CUT_PLANE);
	m_form1_tab->addTab(m_color_scale, STRING_TAB_COLOR_SCALE);
	
	connect(m_level_calculate, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_level_calculate()));
	connect(m_min_level_edit, SIGNAL(editingFinished()), this, SLOT(slot_update_widget_property()));
	connect(m_max_level_edit, SIGNAL(editingFinished()), this, SLOT(slot_update_widget_property()));
	connect(m_level_interval_edit, SIGNAL(editingFinished()), this, SLOT(slot_update_widget_property()));
	connect(m_classify_edit, SIGNAL(editingFinished()), this, SLOT(slot_update_widget_property()));
	connect(m_level_number_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_line_width_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_line_width_slider(int)));
	connect(m_line_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_line_width_spin(int)));
	connect(m_gradient_chromatography, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));//slot_gradient_chro()
	connect(m_show_border, SIGNAL(stateChanged(int)), this, SLOT(slot_show_border()));
	connect(m_border_width_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_border_width_slider(int)));
	connect(m_border_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_border_width_spin(int)));
	connect(m_border_color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(&ia_tmr, SIGNAL(timeout()), this, SLOT(slot_cut_point_timeout()));
#if 0
#else
	m_cut_plane->connect_items();
	connect(m_cut_plane, SIGNAL(cut_plane_property_changed(Child_CutingPlane*, int)), this, SLOT(slot_cut_plane_property_changed(Child_CutingPlane*, int)));

	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar*, int)), this, SLOT(slot_color_scale_property_changed(Child_ColorBar*, int)));
#endif

	this->NodeType = STRING_WIDGET_ISOLINE;
	this->m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);

}

isoline_property_widget::~isoline_property_widget()
{
	delete m_form1_tab;
}

void isoline_property_widget::setCutPlaneRange(float* dataRange)
{
	m_cut_plane->set_range(dataRange);
}

void isoline_property_widget::setCutPoint(double* dataRange)
{
	ia_tmr.stop();
	ia_tmr.start();
	m_cut_plane->set_normal(dataRange);
}

void isoline_property_widget::paintEvent(QPaintEvent *e)
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

void isoline_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *menu = new QMenu(this);
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));
	//out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
#if 0
	//out_put->addAction(tr("high and low area map"), this, SLOT(slot_create_border()));
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
	menu->addAction(tr("connect output module"), this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
#endif
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_isoline())); //this,SLOT(slot_delete_self()));	// _widget
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->move(cursor().pos());
	menu->show();
}

void isoline_property_widget::delete_self() 
{
	emit sig_delete_isoline();
}

G_IsoLine* isoline_property_widget::return_isoline_property_struct()
{
	getBaseStruct();
	m_return_isoline_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_isoline_property_struct->m_level = (Level_Calculate)m_level_calculate->currentIndex();
	m_return_isoline_property_struct->m_type.m_typeAuto.m_count= m_level_number_spinbox->text().toInt();

	m_return_isoline_property_struct->m_type.m_typeInterval.m_minLevel= m_min_level_edit->text().toFloat();
	m_return_isoline_property_struct->m_type.m_typeInterval.m_maxLevel= m_max_level_edit->text().toFloat();;
	m_return_isoline_property_struct->m_type.m_typeInterval.m_interval = m_level_interval_edit->text().toFloat();;

	m_return_isoline_property_struct->m_type.m_typeCount.m_minLevel= m_min_level_edit->text().toFloat();
	m_return_isoline_property_struct->m_type.m_typeCount.m_maxLevel= m_max_level_edit->text().toFloat();
	m_return_isoline_property_struct->m_type.m_typeCount.m_count= m_return_isoline_property_struct->m_type.m_typeAuto.m_count;

	m_return_isoline_property_struct->m_type.m_typeExplicit.m_levelValue= m_classify_edit->text().toFloat();

	m_return_isoline_property_struct->m_render.m_lineWidth = m_line_width_spin->value();
	//m_return_isoline_property_struct.m_render.m_colorMehtod=
	m_return_isoline_property_struct->m_render.m_gradientMap = m_gradient_chromatography->currentIndex();
	m_return_isoline_property_struct->m_render.display_bound = m_show_border->isChecked();
	m_return_isoline_property_struct->m_render.m_boundWidth = m_border_width_slider->value();
	m_return_isoline_property_struct->m_render.m_boundRgb = name2crgb(m_border_color->currentText());

	return m_return_isoline_property_struct;
}

void isoline_property_widget::m_update_widget_property()
{
	G_IsoLine* pTemp = this->return_isoline_property_struct();
	std::shared_ptr<G_IsoLine>ptr(std::make_shared<G_IsoLine>(*pTemp));
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void isoline_property_widget::slot_update_widget_property()
{
	m_update_widget_property();
}

void isoline_property_widget::slot_isoline_to_axis()
{
	emit sig_isoline_to_axis();
}

void isoline_property_widget::slot_isoline_to_border()
{
	emit sig_isoline_to_border();
}

void isoline_property_widget::slot_level_calculate()
{
	int current_index = m_level_calculate->currentIndex();
	if (current_index == 0)
	{
		m_min_level_treeitem->setDisabled(true);m_min_level_treeitem->setHidden(false);
		m_min_level_edit->setDisabled(true);
		m_max_level_treeitem->setDisabled(true);m_max_level_treeitem->setHidden(false);
		m_max_level_edit->setDisabled(true);
		m_level_number_treeitem->setHidden(false);
		m_level_interval_treeitem->setHidden(true);
		m_classify_treeitem->setHidden(true);
	}
	else if (current_index == 1)
	{
		m_min_level_treeitem->setHidden(false); m_min_level_treeitem->setDisabled(false);
		m_min_level_edit->setDisabled(false);
		m_max_level_treeitem->setHidden(false); m_max_level_treeitem->setDisabled(false);
		m_max_level_edit->setDisabled(false);
		m_level_number_treeitem->setHidden(false);
		m_level_interval_treeitem->setHidden(true);
		m_classify_treeitem->setHidden(true);
	}
	else if (current_index == 2)
	{
		m_min_level_treeitem->setHidden(false); m_min_level_treeitem->setDisabled(false);
		m_min_level_edit->setDisabled(false);
		m_max_level_treeitem->setHidden(false); m_max_level_treeitem->setDisabled(false);
		m_max_level_edit->setDisabled(false);
		m_level_number_treeitem->setHidden(true);
		m_level_interval_treeitem->setHidden(false);
		m_classify_treeitem->setHidden(true);
	}
	else
	{
		m_min_level_treeitem->setHidden(true);
		m_max_level_treeitem->setHidden(true);
		m_level_number_treeitem->setHidden(true);
		m_level_interval_treeitem->setHidden(true);
		m_classify_treeitem->setHidden(false);
	}
	m_update_widget_property();
}

void isoline_property_widget::slot_line_width_slider(int a)
{
	disconnect(m_line_width_slider, 0, 0, 0);
	m_line_width_slider->setValue(a);
	m_update_widget_property();
	connect(m_line_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_line_width_spin(int)));
}

void isoline_property_widget::slot_line_width_spin(int a)
{
	disconnect(m_line_width_spin, 0, 0, 0);
	m_line_width_spin->setValue(a); 
	m_update_widget_property();
	connect(m_line_width_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_line_width_slider(int)));
}

void isoline_property_widget::slot_gradient_chro()
{

}

void isoline_property_widget::slot_show_border()
{
	m_update_widget_property();
}

void isoline_property_widget::slot_border_width_slider(int a)
{
	disconnect(m_border_width_slider, 0, 0, 0);
	m_border_width_slider->setValue(a);
	m_update_widget_property();
	connect(m_border_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_border_width_spin(int)));
}

void isoline_property_widget::slot_border_width_spin(int a)
{
	disconnect(m_border_width_spin, 0, 0, 0);
	m_border_width_spin->setValue(a);
	m_update_widget_property();
	connect(m_border_width_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_border_width_slider(int)));
}

void isoline_property_widget::slot_place_direction()
{

}

void isoline_property_widget::slot_x_normal()
{
	QString normal_direction = "(";
	normal_direction += m_cut_plane->m_x_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_y_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_z_normal_edit->text();
	normal_direction += ")";
	m_cut_plane->m_normal_direction_treeitem->setText(1, normal_direction);

}

void isoline_property_widget::slot_y_normal()
{
	QString normal_direction = "(";
	normal_direction += m_cut_plane->m_x_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_y_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_z_normal_edit->text();
	normal_direction += ")";
	m_cut_plane->m_normal_direction_treeitem->setText(1, normal_direction);
}

void isoline_property_widget::slot_z_normal()
{
	QString normal_direction = "(";
	normal_direction += m_cut_plane->m_x_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_y_normal_edit->text();
	normal_direction += ",";
	normal_direction += m_cut_plane->m_z_normal_edit->text();
	normal_direction += ")";
	m_cut_plane->m_normal_direction_treeitem->setText(1, normal_direction);
}

void isoline_property_widget::slot_offset_from_center_slider(double a)
{
	disconnect(m_cut_plane->m_offset_from_center_slider, 0, 0, 0);
	int value = (int)a * 100 + 100;
	m_cut_plane->m_offset_from_center_slider->setValue(value);
	connect(m_cut_plane->m_offset_from_center_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_offset_from_center_spin(int)));
}

void isoline_property_widget::slot_offset_from_center_spin(int a)
{
	disconnect(m_cut_plane->m_offset_from_center_edit, 0, 0, 0);
	float value = (a - 100)*0.01;
	m_cut_plane->m_offset_from_center_edit->setText(QString("%1").arg(value));
	connect(m_cut_plane->m_offset_from_center_edit, SIGNAL(editingFinished()), this, SLOT(slot_offset_from_center_slider(double)));
}

void isoline_property_widget::slot_cut_plane_property_changed(Child_CutingPlane*, int type)
{
	baseStruct->m_changetype = G_IsoLine_MN_CT + type;
	m_update_widget_property();
	//baseStruct->m_changetype = 0;
}

void isoline_property_widget::slot_cut_point_timeout()
{
	ia_tmr.stop();
	baseStruct->m_changetype = G_IsoLine_MN_CT + 40;
	m_update_widget_property();
	//baseStruct->m_changetype = 0;
}

void isoline_property_widget::slot_m_x_position_slider(double a)
{
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void isoline_property_widget::slot_m_x_position_spin(int a)
{
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void isoline_property_widget::slot_m_y_position_slider(double a)
{
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void isoline_property_widget::slot_m_y_position_spin(int a)
{
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void isoline_property_widget::slot_m_width_slider(double a)
{
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void isoline_property_widget::slot_m_width_spin(int a)
{
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void isoline_property_widget::slot_m_length_slider(double a)
{
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void isoline_property_widget::slot_m_length_spin(int a)
{
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void isoline_property_widget::slot_m_font_size_slider(int a)
{
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void isoline_property_widget::slot_m_font_size_spin(int a)
{
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void isoline_property_widget::slot_m_note_font_size_slider(double a)
{
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void isoline_property_widget::slot_m_note_font_size_spin(int a)
{
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_note_font_size_slider(double)));
}

void isoline_property_widget::slot_color_scale_property_changed(Child_ColorBar*ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*else if (1 == type) {
		set_color_bar_actor(ccb->m_displayOn);
	}*/
	else{
		m_return_isoline_property_struct->m_changetype = 5;
		G_IsoLine* pTemp = this->return_isoline_property_struct();
		std::shared_ptr<G_IsoLine>ptr(std::make_shared<G_IsoLine>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}
