#include "isosurface_property_widget.h"
#include "myqwidget.h"

isosurface_property_widget::isosurface_property_widget(int num,G_IsoSurface* isosurface)
{
	m_return_isosurface_property_struct = new G_IsoSurface;
	baseStruct = m_return_isosurface_property_struct;
	QString label = (isosurface) ? QString::fromStdString(isosurface->m_name) : ( (num == 0)? STRING_LABEL_ISOSURFACE : STRING_LABEL_ISOSURFACE + QString::number(num + 1));
	 create_2buttons(this, label);
	
	m_sliderPressed = false;
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	//m_normal_property->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_isosurface_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_SUFARENDER));
	//m_input_component_treeitem = new QTreeWidgetItem(m_isosurface_treeitem, QStringList("input component"));
	m_critical_value_treeitem = new QTreeWidgetItem(m_isosurface_treeitem, QStringList(STRING_ITEM_CRITICALVAL));
	m_critical_edit = new QLineEdit;
	m_critical_edit->setText("100");
	m_critical_slider = new QSlider(Qt::Horizontal);
	m_critical_slider->setRange(0, 20000);
	m_critical_slider->setValue(10000);
	QWidget *m_critical_widget = new QWidget;
	QHBoxLayout *m_critical_layout = new QHBoxLayout;
	m_critical_layout->addWidget(m_critical_edit);
	m_critical_layout->addWidget(m_critical_slider);
	m_critical_widget->setLayout(m_critical_layout);
	m_normal_property->setItemWidget(m_critical_value_treeitem, 1, m_critical_widget);
	m_calculate_volume_treeitem = new QTreeWidgetItem(m_isosurface_treeitem,QStringList(STRING_ITEM_CALCULATEVOL));
	m_calculate_check = new QCheckBox;
	m_normal_property->setItemWidget(m_calculate_volume_treeitem, 1, m_calculate_check);
	m_volume_bigger_critical_treeitem = new QTreeWidgetItem(m_isosurface_treeitem, QStringList(STRING_ITEM_VOLTOCRITICAL));
	m_volume_bigger_critical_treeitem->setDisabled(true);
	m_volume_bigger_edit = new QLineEdit; m_volume_bigger_edit->setDisabled(true);
	m_normal_property->setItemWidget(m_volume_bigger_critical_treeitem, 1, m_volume_bigger_edit);
	m_volume_smaller_critical_treeitem = new QTreeWidgetItem(m_isosurface_treeitem, QStringList(STRING_ITEM_CRITICALTOVOL));
	m_volume_smaller_critical_treeitem->setDisabled(true);
	m_volume_smaller_edit = new QLineEdit;
	m_volume_smaller_edit->setDisabled(true);
	m_normal_property->setItemWidget(m_volume_smaller_critical_treeitem, 1, m_volume_smaller_edit);
	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_RENDER));
	m_style_sheet_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_STYLESHEET));
	//m_color_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList("color method"));
	m_color_method_combox = new QComboBox;
	m_color_method_combox->insertItem(0, STRING_COMBOX_GRADIENT);
	m_color_method_combox->insertItem(1, STRING_COMBOX_CUSTOM);
	//m_normal_property->setItemWidget(m_color_method_treeitem, 1, m_color_method_combox);
	m_gradient_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_GRADIENTCOLOR));
	m_gradient_color_combox = new QComboBox;
	gradient_color(m_gradient_color_combox);
	m_normal_property->setItemWidget(m_gradient_color_treeitem, 1, m_gradient_color_combox);
	//m_render_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList("color"));
	m_render_color_combox = new QComboBox;
	color_choose(m_render_color_combox);
	//m_normal_property->setItemWidget(m_render_color_treeitem, 1, m_render_color_combox);
	//m_render_color_treeitem->setHidden(true);
	m_material_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_MATERIAL));
	m_highlight_intense_treeitem = new QTreeWidgetItem(m_material_treeitem, QStringList(STRING_ITEM_HIGHINITENSE));
	m_highlight_color_treeitem = new QTreeWidgetItem(m_material_treeitem, QStringList(STRING_ITEM_HIGHCOLOR));
	 m_highlight_color_combox = new QComboBox(this);
	color_choose(m_highlight_color_combox);
	m_normal_property->setItemWidget(m_highlight_color_treeitem, 1, m_highlight_color_combox);
	m_highlight_intense_edit = new QLineEdit;
	m_highlight_intense_slider = new QSlider(Qt::Horizontal);
	m_highlight_intense_slider->setRange(0, 100);
	m_highlight_intense_slider->setValue(50);
	m_highlight_intense_edit->setText("0.5");

	QWidget *m_highlight_intense_widget = new QWidget;
	QHBoxLayout *m_highlight_intense_layout = new QHBoxLayout;
	m_highlight_intense_layout->addWidget(m_highlight_intense_edit);
	m_highlight_intense_layout->addWidget(m_highlight_intense_slider);
	m_highlight_intense_widget->setLayout(m_highlight_intense_layout);
	m_normal_property->setItemWidget(m_highlight_intense_treeitem, 1, m_highlight_intense_widget);
	m_gloss_treeitem = new QTreeWidgetItem(m_material_treeitem, QStringList(STRING_ITEM_GLOSS));
	m_gloss_edit = new QLineEdit;
	m_gloss_edit->setText("1");
	m_gloss_slider = new QSlider(Qt::Horizontal);
	m_gloss_slider->setRange(0, 100);
	m_gloss_slider->setValue(100); m_gloss_edit->setText("1");
	QWidget *m_gloss_widget = new QWidget;
	QHBoxLayout *m_gloss_layout = new QHBoxLayout;
	m_gloss_layout->addWidget(m_gloss_edit);
	m_gloss_layout->addWidget(m_gloss_slider);
	m_gloss_widget->setLayout(m_gloss_layout);
	m_normal_property->setItemWidget(m_gloss_treeitem, 1, m_gloss_widget);

	m_opacity_treeitem = new QTreeWidgetItem(m_material_treeitem, QStringList(STRING_ITEM_OPACITY));
	m_opacity_edit = new QLineEdit;
	m_opacity_edit->setText("1");
	m_opacity_slider = new QSlider(Qt::Horizontal);
	m_opacity_slider->setRange(0, 100);
	m_opacity_slider->setValue(100);
	QWidget *m_opacity_widget = new QWidget;
	QHBoxLayout *m_opacity_layout = new QHBoxLayout;
	m_opacity_layout->addWidget(m_opacity_edit);
	m_opacity_layout->addWidget(m_opacity_slider);
	m_opacity_widget->setLayout(m_opacity_layout);
	m_normal_property->setItemWidget(m_opacity_treeitem, 1, m_opacity_widget);
	m_normal_property->expandAll();
#if 1
	m_color_scale = new color_scale_property;
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
#endif

	if (!isosurface)
	{
		GIsoSurfaceNode::getDefault(m_return_isosurface_property_struct);
		isosurface = m_return_isosurface_property_struct;
	}
	{
		m_input_treeitem->setText(1, QString::fromStdString(isosurface->m_putIn));
		//m_input_component_treeitem->setText(1, QString::number(isosurface->m_putIndex));
		m_critical_edit->setText(QString("%1").arg(isosurface->m_CriticalValue));
		m_critical_slider->setValue(isosurface->m_CriticalValue * 100);
		m_calculate_check->setChecked(isosurface->m_isCalculateVolume);
		if (m_calculate_check->isChecked())
		{
			m_volume_bigger_critical_treeitem->setDisabled(false);
			m_volume_smaller_critical_treeitem->setDisabled(false);
			m_volume_bigger_edit->setDisabled(false); m_volume_smaller_edit->setDisabled(false);
			m_volume_bigger_edit->setText(QString("%1").arg(isosurface->m_volumeBigerThanCriticalValue));
			m_volume_smaller_edit->setText(QString("%1").arg(isosurface->m_volumeSmallerThanCriticalValue));
		}
		//if (isosurface->m_render.m_colorMethod == GRADIENT_OF_COLOR)
			m_color_method_combox->setCurrentIndex(0);
		//else
		//	m_color_method_combox->setCurrentIndex(1);
		m_color_scale->set_data(&isosurface->m_materials.m_colorBar);
		m_highlight_color_combox->setCurrentText(crgb2name(isosurface->m_materials.m_HighLight_rgb));
		//m_highlight_intense_treeitem->setText(1,isosurface->m_materials.m_HighLight_strength)
		//m_gloss_treeitem->setText(1, isosurface->m_materials.m_glossiness);
		m_opacity_edit->setText(QString("%1").arg(isosurface->m_materials.m_opacity));
		m_opacity_slider->setValue((int)isosurface->m_materials.m_opacity * 100);
	}

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black;}");
	m_form1_tab->addTab(m_normal_property, QString(STRING_TAB_GENERAL));
	m_form1_tab->addTab(m_color_scale, QString(STRING_TAB_COLOR_SCALE));
	//m_return_isosurface_property_struct = new G_IsoSurface;
	this->NodeType = STRING_WIDGET_ISOSURF;
	this->m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);

	connect(m_critical_edit, SIGNAL(editingFinished()), this, SLOT(slot_critical_edit()));
	connect(m_critical_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_critical_slider(int)));
	connect(m_critical_slider, SIGNAL(sliderPressed()), this, SLOT(slot_critical_slider_pressed()));
	connect(m_critical_slider, SIGNAL(sliderReleased()), this, SLOT(slot_critical_slider_relesed()));
	connect(m_calculate_check, SIGNAL(stateChanged(int)), this, SLOT(slot_calculate_volume()));
	connect(m_color_method_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_method()));
	connect(m_gradient_color_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_gradient_color()));
	connect(m_highlight_color_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_choose()));
	connect(m_highlight_intense_edit, SIGNAL(editingFinished()), this, SLOT(slot_high_intense_edit()));
	connect(m_highlight_intense_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_high_intense_slider(int)));
	connect(m_highlight_intense_slider, SIGNAL(sliderPressed()), this, SLOT(slot_highlight_slider_pressed()));
	connect(m_highlight_intense_slider, SIGNAL(sliderReleased()), this, SLOT(slot_highlight_slider_relesed()));
	connect(m_gloss_edit, SIGNAL(editingFinished()), this, SLOT(slot_gloss_edit()));
	connect(m_gloss_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_gloss_slider(int)));
	connect(m_gloss_slider, SIGNAL(sliderPressed()), this, SLOT(slot_gloss_slider_pressed()));
	connect(m_gloss_slider, SIGNAL(sliderReleased()), this, SLOT(slot_gloss_slider_relesed()));
	connect(m_opacity_edit, SIGNAL(editingFinished()), this, SLOT(slot_opacity_slider()));
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_opacity_spin(int)));
	connect(m_opacity_slider, SIGNAL(sliderPressed()), this, SLOT(slot_slider_pressed()));
	connect(m_opacity_slider, SIGNAL(sliderReleased()), this, SLOT(slot_slider_relesed()));
#if 1
	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar*, int)), this, SLOT(slot_color_scale_property_changed(Child_ColorBar*, int)));
#endif
}

isosurface_property_widget::~isosurface_property_widget()
{
	delete m_form1_tab;
}

void isosurface_property_widget::paintEvent(QPaintEvent *e)
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

G_IsoSurface* isosurface_property_widget::return_isosurface_property_struct()
{
	getBaseStruct();
	m_return_isosurface_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_isosurface_property_struct->m_putIndex = 0;// m_input_component_treeitem->text(1).toInt();
	m_return_isosurface_property_struct->m_CriticalValue = m_critical_edit->text().toFloat();
	m_return_isosurface_property_struct->m_isCalculateVolume = m_calculate_check->isChecked();
	if (m_calculate_check->isChecked())
	{
		m_return_isosurface_property_struct->m_volumeBigerThanCriticalValue=m_volume_bigger_edit->text().toFloat();
		m_return_isosurface_property_struct->m_volumeSmallerThanCriticalValue = m_volume_smaller_edit->text().toFloat();
	}
	int color_method_index = m_color_method_combox->currentIndex();
	if (color_method_index == 0)
	{
		m_return_isosurface_property_struct->m_render.m_colorMethod = GRADIENT_OF_COLOR;
		m_return_isosurface_property_struct->m_render.m_gradientMap = m_gradient_color_combox->currentIndex();
	}
	else
		m_return_isosurface_property_struct->m_render.m_colorMethod = CUSTORM_OF_COLOR;
	
	m_return_isosurface_property_struct->m_materials.m_HighLight_rgb = name2crgb(m_highlight_color_combox->currentText());

	m_return_isosurface_property_struct->m_materials.m_HighLight_strength = m_highlight_intense_edit->text().toFloat();
	m_return_isosurface_property_struct->m_materials.m_glossiness = m_gloss_edit->text().toFloat();
	m_return_isosurface_property_struct->m_materials.m_opacity = m_opacity_edit->text().toFloat();
	return m_return_isosurface_property_struct;
}

void isosurface_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
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
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_isosurface()));
	menu->move(cursor().pos());
	menu->show();
}

void isosurface_property_widget::delete_self()
{
	emit sig_delete_isosurface();
}

void isosurface_property_widget::m_update_widget_property()
{
	G_IsoSurface* pTemp = this->return_isosurface_property_struct();
	std::shared_ptr<G_IsoSurface>ptr(std::make_shared<G_IsoSurface>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void isosurface_property_widget::slot_critical_edit()
{
	m_return_isosurface_property_struct->m_changetype = 1;
	disconnect(m_critical_slider, 0, 0, 0);
	int  a = m_critical_edit->text().toFloat() * 100;
	m_critical_slider->setValue(a);
	m_update_widget_property();
	connect(m_critical_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_critical_slider(int)));
}

void isosurface_property_widget::slot_critical_slider(int a)
{
	if (!m_sliderPressed)
	{
		m_return_isosurface_property_struct->m_changetype = 1;
		float b = a * 0.01;
		m_critical_edit->setText(QString("%1").arg(b));
		m_update_widget_property();
	}
}

void isosurface_property_widget::slot_critical_slider_pressed()
{
	m_sliderPressed = true;
}

void isosurface_property_widget::slot_critical_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_critical_slider->value();
	slot_critical_slider(value);
}

void isosurface_property_widget::slot_calculate_volume()
{
	if (!m_calculate_check->isChecked())
	{
		m_volume_bigger_critical_treeitem->setDisabled(true);
		m_volume_smaller_critical_treeitem->setDisabled(true);
		m_volume_bigger_edit->setText("0"); m_volume_bigger_edit->setDisabled(true);
		m_volume_smaller_edit->setText("0"); m_volume_smaller_edit->setDisabled(true);
		m_critical_slider->setRange(0, 20000);
	}
	else
	{
		m_volume_bigger_critical_treeitem->setDisabled(false);
		m_volume_smaller_critical_treeitem->setDisabled(false);
		m_volume_bigger_edit->setDisabled(false);
		m_volume_smaller_edit->setDisabled(false);
		m_critical_slider->setRange(0, 30000);
	}
	m_update_widget_property();
}

void isosurface_property_widget::slot_color_method()
{
	if (m_color_method_combox->currentIndex() == 0)
	{
		m_gradient_color_treeitem->setHidden(false);
		m_render_color_treeitem->setHidden(true);
	}
	else
	{
		m_gradient_color_treeitem->setHidden(true);
		m_render_color_treeitem->setHidden(false);
	}
	m_update_widget_property();
}

void isosurface_property_widget::slot_gradient_color()
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_color_choose()
{
	m_return_isosurface_property_struct->m_changetype = 3;
	m_update_widget_property();
}

void isosurface_property_widget::slot_high_intense_edit()
{
	m_return_isosurface_property_struct->m_changetype = 3;
	disconnect(m_highlight_intense_slider, 0, 0, 0);
	int a = (m_highlight_intense_edit->text().toFloat())* 100;
	m_highlight_intense_slider->setValue(a);
	connect(m_highlight_intense_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_high_intense_slider(int)));
	m_update_widget_property();
}

void isosurface_property_widget::slot_high_intense_slider(int a)
{
	if (!m_sliderPressed)
	{
		m_return_isosurface_property_struct->m_changetype = 3;
		float b = a * 0.01;
		m_highlight_intense_edit->setText(QString("%1").arg(b));
		m_update_widget_property();
	}
}

void isosurface_property_widget::slot_highlight_slider_pressed()
{
	m_sliderPressed = true;
}

void isosurface_property_widget::slot_highlight_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_highlight_intense_slider->value();
	slot_high_intense_slider(value);
}

void isosurface_property_widget::slot_gloss_edit()
{
	m_return_isosurface_property_struct->m_changetype = 3;
	disconnect(m_gloss_slider, 0, 0, 0);
	int a = (m_gloss_edit->text().toFloat()) * 100;
	m_gloss_slider->setValue(a);
	connect(m_gloss_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_gloss_slider(int)));
	m_update_widget_property();
}

void isosurface_property_widget::slot_gloss_slider(int a)
{
	if (!m_sliderPressed)
	{
		m_return_isosurface_property_struct->m_changetype = 3;
		float b = a * 0.01;
		m_gloss_edit->setText(QString("%1").arg(b));
		m_update_widget_property();
	}
}

void isosurface_property_widget::slot_gloss_slider_pressed()
{
	m_sliderPressed = true;
}

void isosurface_property_widget::slot_gloss_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_gloss_slider->value();
	slot_gloss_slider(value);
}

void isosurface_property_widget::slot_opacity_slider()
{
	m_return_isosurface_property_struct->m_changetype = 3;
	disconnect(m_opacity_slider, 0, 0, 0);
	int a = (m_opacity_edit->text().toFloat()) * 100;
	m_opacity_slider->setValue(a);
	m_update_widget_property();
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_opacity_spin(int)));
}

void isosurface_property_widget::slot_opacity_spin(int a)
{
	if (!m_sliderPressed)
	{
		m_return_isosurface_property_struct->m_changetype = 3;
		float value = a * 0.01;
		m_opacity_edit->setText(QString("%1").arg(value));
		m_update_widget_property();
/*
		m_return_isosurface_property_struct->m_materials.m_opacity = value;
		G_IsoSurface* pTemp = m_return_isosurface_property_struct;
		std::shared_ptr<G_IsoSurface>ptr(std::make_shared<G_IsoSurface>());
		memcpy_s(ptr.get(), sizeof(G_IsoSurface), pTemp, sizeof(G_IsoSurface));
		ptr.get()->m_info.m_leftTopPosX = this->pos().x();
		ptr.get()->m_info.m_leftTopPosY = this->pos().y();
		ptr.get()->m_info.m_length = this->width();
		ptr.get()->m_info.m_width = this->height();
		//ptr.get()->m_name = "slice";
		OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
*/
	}
}

void isosurface_property_widget::slot_slider_pressed()
{
	m_sliderPressed = true;
}

void isosurface_property_widget::slot_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_opacity_slider->value();
	slot_opacity_spin(value);
}

void isosurface_property_widget::slot_show_color_scale(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_place_direction(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_m_x_position_slider(double a)
{
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void isosurface_property_widget::slot_m_x_position_spin(int a)
{
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void isosurface_property_widget::slot_m_y_position_slider(double a)
{
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void isosurface_property_widget::slot_m_y_position_spin(int a)
{
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void isosurface_property_widget::slot_m_width_slider(double a)
{
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void isosurface_property_widget::slot_m_width_spin(int a)
{
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void isosurface_property_widget::slot_m_length_slider(double a)
{
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void isosurface_property_widget::slot_m_length_spin(int a)
{
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void isosurface_property_widget::slot_m_font_size_slider(int a)
{
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void isosurface_property_widget::slot_m_font_size_spin(int a)
{
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void isosurface_property_widget::slot_m_note_number(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_m_note_font_size_slider(double a)
{
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void isosurface_property_widget::slot_m_note_font_size_spin(int a)
{
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_note_font_size_slider(double)));
}

void isosurface_property_widget::slot_m_adopt_self_note(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_color_type(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::slot_significant(int a)
{
	m_update_widget_property();
}

void isosurface_property_widget::setScalarRange(double* dataRange)
{
	m_critical_slider->setRange(dataRange[0]*100, dataRange[1]*100);
}

void isosurface_property_widget::displayVolume(double v1, double v2)
{
	m_volume_bigger_edit->setText(QString("%1").arg(v1<v2?v1:v2));
	m_volume_smaller_edit->setText(QString("%1").arg(v1<v2?v2:v1));
}

void isosurface_property_widget::slot_color_scale_property_changed(Child_ColorBar*ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*	else if (1 == type){
			set_color_bar_actor(ccb->m_displayOn);
		}*/
	else {
		G_IsoSurface* pTemp = (G_IsoSurface*)this->return_isosurface_property_struct();
		pTemp->m_changetype = 5;

		std::shared_ptr<G_IsoSurface>ptr(std::make_shared<G_IsoSurface>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}
