// scatter_property_widget.cpp

#include "scatter_property_widget.h"

#define SCATTER_COLOR_METHOD_FIXED			2
#define SCATTER_COLOR_METHOD_GRADIENT	0
#define SCATTER_COLOR_METHOD_RGB			1

scatter_property_widget::scatter_property_widget(int num,G_ScatterMap* pData)
{
	QString label = (pData) ? QString::fromStdString(pData->m_name) : ( (num == 0)? STRING_LABEL_SCATTER : STRING_LABEL_SCATTER + QString::number(num + 1));
	create_2buttons(this, label);
	m_sliderPressed = false;
	//常规属性
	m_normal_property = new QTreeWidget();
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);
	m_input_data_count_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_DATA_COUNT));
	m_input_data_count_treeitem->setText(1, "0");

	m_scatter_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_SCATDIAG));
	m_density_treeitem = new QTreeWidgetItem(m_scatter_treeitem, QStringList(STRING_ITEM_DENSITY));
	m_density = new QComboBox;
	m_density->insertItem(0, "100");
	m_density->insertItem(1, "50");
	m_density->insertItem(2, "33");
	m_density->insertItem(3, "25");
	m_density->insertItem(4, "20");
	m_density->insertItem(5, "10");
	m_density->insertItem(6, "5");
	m_density->insertItem(7, "1");
	m_density->setCurrentIndex(0);
	m_normal_property->setItemWidget(m_density_treeitem, 1, m_density);
	m_normal_property->insertTopLevelItem(0, m_scatter_treeitem);
	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_RENDER));
	m_symbol_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_SYMBOL));
	m_symbol = new QComboBox;
	m_symbol->insertItem(0, STRING_COMBOX_DIAMONDS);
	m_symbol->insertItem(1, STRING_COMBOX_BALL);
	m_symbol->setCurrentIndex(0);
	m_normal_property->setItemWidget(m_symbol_treeitem, 1, m_symbol);
	m_symbol_treeitem->setHidden(true);
	m_size_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_SIZE));
	m_size_edit = new QLineEdit;
	m_size_edit->setText("0.2");
	m_size_slider = new QSlider(Qt::Horizontal);
	m_size_slider->setRange(1, 1000);
	m_size_slider->setValue(20);

	QWidget *size_widget = new QWidget;
	QHBoxLayout *size_layout = new QHBoxLayout;
	size_layout->addWidget(m_size_edit);
	size_layout->addWidget(m_size_slider);
	size_widget->setLayout(size_layout);
	m_normal_property->setItemWidget(m_size_treeitem, 1, size_widget);
	QTreeWidgetItem *color_method = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_COLORMETHOD));
	m_color_method = new QComboBox;
	m_color_method->insertItem(SCATTER_COLOR_METHOD_GRADIENT, STRING_COMBOX_GRADIENTMAP);
	//m_color_method->insertItem(SCATTER_COLOR_METHOD_RGB, "RGB");
	//m_color_method->insertItem(SCATTER_COLOR_METHOD_FIXED, "fixed");
	m_color_method->setCurrentIndex(SCATTER_COLOR_METHOD_GRADIENT);
	m_normal_property->setItemWidget(color_method, 1, m_color_method);
	color_method->setHidden(true);
	m_normal_property->insertTopLevelItem(1,m_render_treeitem);
	m_color_treeitem = new QTreeWidgetItem(m_render_treeitem,QStringList(STRING_ITEM_COLOR));
	m_color_combox = new QComboBox;
	color_choose(m_color_combox);//comboBox控件填充颜色列表
	m_normal_property->setItemWidget(m_color_treeitem, 1, m_color_combox);
	m_color_treeitem->setHidden(true);
	m_gradient_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_GRADIENTCOLOR));
	m_gradient_color_combox = new QComboBox;
	gradient_color(m_gradient_color_combox);
	m_normal_property->setItemWidget(m_gradient_color_treeitem, 1, m_gradient_color_combox);
	m_color_range_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_COLORRANGE));
	m_color_range_combox = new QComboBox;
	m_normal_property->setItemWidget(m_color_range_treeitem, 1, m_color_range_combox);
	m_red_component_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_REDCOMPON));
	m_red_component_combox = new QComboBox;
	m_normal_property->setItemWidget(m_red_component_treeitem, 1, m_red_component_combox);
	m_green_component_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_GREENCOMPON));
	m_green_component_combox = new QComboBox;
	m_normal_property->setItemWidget(m_green_component_treeitem, 1, m_green_component_combox);
	m_blue_component_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_BLUECOMPON));
	m_blue_component_combox = new QComboBox;
	m_normal_property->setItemWidget(m_blue_component_treeitem, 1, m_blue_component_combox);
	m_color_range_treeitem->setHidden(true);
	m_red_component_treeitem->setHidden(true);
	m_green_component_treeitem->setHidden(true);
	m_blue_component_treeitem->setHidden(true);
	//m_normal_property->expandItem(m_scatter_treeitem);
	m_normal_property->expandAll();

	//标注
	m_note_tree = new QTreeWidget;
	m_note_tree->setColumnCount(2);
	m_note_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_note_treeitem = new QTreeWidgetItem(m_note_tree, QStringList(STRING_ITEM_NOTE));
	m_show_note_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_SHOWNOTATE));
	m_note_font_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_NOTEFONT));
	m_note_density_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_DENSITY));
	m_show_line_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList("show line"));
	m_note_type_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList("type"));
	m_note_significant_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList("significant"));
	m_show_note_check = new QCheckBox;
	m_note_tree->setItemWidget(m_show_note_treeitem, 1, m_show_note_check);
	m_note_font_combox = new QComboBox;
	m_note_font_combox->insertItem(0, "X");
	m_note_font_combox->insertItem(1, "Y");
	m_note_font_combox->insertItem(2, "Z");
	m_note_font_combox->insertItem(3, "XYZ");
	m_note_tree->setItemWidget(m_note_font_treeitem, 1, m_note_font_combox);
	m_density_combox = new QComboBox;
	m_density_combox->insertItem(0, "100%");
	m_density_combox->insertItem(1, "50%");
	m_density_combox->insertItem(2, "33%");
	m_density_combox->insertItem(3, "25%");
	m_density_combox->insertItem(4, "20%");
	m_density_combox->insertItem(5, "10%");
	m_density_combox->insertItem(6, "1%");
	m_note_tree->setItemWidget(m_note_density_treeitem, 1, m_density_combox);
	m_show_line_check = new QCheckBox;
	m_note_tree->setItemWidget(m_show_line_treeitem, 1, m_show_line_check);
	m_type_combox = new QComboBox;
	m_type_combox->insertItem(0, "fixed");
	m_type_combox->insertItem(1, "exp");
	m_type_combox->insertItem(1, "normal");
	m_note_tree->setItemWidget(m_note_type_treeitem, 1, m_type_combox);
	m_note_significant_spin = new QSpinBox;
	m_note_tree->setItemWidget(m_note_significant_treeitem, 1, m_note_significant_spin);
	m_note_tree->expandItem(m_note_treeitem);
	m_color_scale = new color_scale_property;
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	//m_form1_tab->addTab(m_note_tree, QString("note"));
	m_form1_tab->addTab(m_color_scale, QString(STRING_TAB_COLOR_SCALE));

	//init m_return_scatter_struct
	m_return_scatter_struct = new G_ScatterMap;
	baseStruct = m_return_scatter_struct;

	if (!pData)
	{
		GScatterMapNode::getDefault(m_return_scatter_struct);
	}
	else
	{
		*m_return_scatter_struct = *(G_ScatterMap *)pData;
	}
	pData= m_return_scatter_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(pData->m_putIn));
		m_density->setCurrentText(QString("%1").arg(pData->m_density *100));
		if (pData->m_render.m_symbol == DIAMONDS)
		{
			m_symbol->setCurrentIndex(0);
		}
		else
		{
			m_symbol->setCurrentIndex(1);
		}
		m_size_edit->setText(QString("%1").arg(pData->m_render.m_Symbol_size));
		m_size_slider->setValue(pData->m_render.m_Symbol_size*1000);

		m_show_note_check->setChecked(pData->m_render.m_labelMark.m_showOn);
		m_note_font_combox->setCurrentIndex(pData->m_render.m_labelMark.m_labelMarkIndex);
		m_density_combox->setCurrentIndex(pData->m_render.m_labelMark.m_densityIndex);
		m_show_line_check->setChecked(pData->m_render.m_labelMark.m_showLine);
		m_type_combox->setCurrentIndex(pData->m_render.m_labelMark.m_markType);
		m_note_significant_spin->setValue(pData->m_render.m_labelMark.m_numCount);
#if 0
#else
		m_color_scale->set_data(&pData->m_render.m_colorBar);
#endif
	}
	
	//m_return_scatter_struct = new G_ScatterMap;
	this->NodeType = STRING_WIDGET_SCATTER;
	m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	m_can_append_widget.push_back(STRING_WIDGET_BORDER);
//	connect(m_normal_property, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_status_show_input(QTreeWidgetItem*, int)));
	connect(m_density, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_density_change(int)));
	connect(m_symbol, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_symbol_change(int)));
	connect(m_size_edit, SIGNAL(editingFinished()), this, SLOT(m_size_slider_setValue()));
	connect(m_size_slider, SIGNAL(valueChanged(int)), this, SLOT(m_size_spin_setValue(int)));
	connect(m_size_slider, SIGNAL(sliderPressed()), this, SLOT(slot_size_pressed()));
	connect(m_size_slider, SIGNAL(sliderReleased()), this, SLOT(slot_size_released()));
	connect(m_color_method, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_method_change(int)));
	connect(m_color_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_choose()));
	connect(m_gradient_color_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_gradient_color()));
//	connect(m_note_tree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_status_show_note(QTreeWidgetItem*, int)));
	connect(m_show_note_check, SIGNAL(stateChanged(int)), this, SLOT(slot_show_note()));
	connect(m_note_font_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_note_font()));
	//connect(m_density, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_density()));
	connect(m_show_line_check, SIGNAL(stateChanged(int)), this, SLOT(slot_show_line()));
	connect(m_type_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_type()));
	connect(m_note_significant_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_sig()));
#if 0
#else
	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar* , int )), this, SLOT(slot_color_scale_property_changed(Child_ColorBar* , int )));
#endif
}

void scatter_property_widget::paintEvent(QPaintEvent *e)
{
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

void scatter_property_widget::contextMenuEvent(QContextMenuEvent *event)
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
		menu->addAction(tr("connect input module(") + QString::fromStdString(this->m_pre_widget->get_label_text()) + QString(")"), this, SLOT(slot_connect_input()));
	}
	else
	{
		menu->addAction(tr("connect input module(") + QString("not connect") + QString(")"), this, SLOT(slot_connect_input()));
	}
	menu->addAction(tr("connect output module"), this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_CONN_OUT,	this,	SLOT(slot_connect_output()));
#endif
	menu->addAction(STRING_ACTION_CONN_IN,		this,	SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_scatter()));
	menu->move(cursor().pos());
	menu->show();
}

void scatter_property_widget::delete_self()
{
	emit sig_delete_scatter();
}

void scatter_property_widget::slot_create_cut()
{
	emit sig_scatter_to_cut();
}

void scatter_property_widget::slot_create_axis()
{
	emit sig_scatter_to_axis();
}

void scatter_property_widget::slot_create_border()
{
	emit sig_scatter_to_border();
}

scatter_property_widget::~scatter_property_widget()
{
	delete m_form1_tab;
}

 G_ScatterMap* scatter_property_widget::return_scatter_property_struct()
{
	 getBaseStruct();
	m_return_scatter_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_scatter_struct->m_density = m_density->currentText().toFloat();
	m_return_scatter_struct->m_density *= 0.01;
	m_return_scatter_struct->m_insertMethod = FIXED_VALUE;
	//QString symbol = m_symbol->currentText();
	//if (symbol == "diamonds")
	//	m_return_scatter_struct->m_render.m_symbol = DIAMONDS;
	//else
	m_return_scatter_struct->m_render.m_symbol = (Child_Symbol_ScatterMap)m_symbol->currentIndex();// BALL;
	m_return_scatter_struct->m_render.m_Symbol_size = (float)(m_size_edit->text().toFloat()*10);
	int n = m_color_method->currentIndex();
	if (n == SCATTER_COLOR_METHOD_FIXED) //固定值
	{
		m_return_scatter_struct->m_render.m_useRGB = true;
		m_return_scatter_struct->m_render.m_rgb = name2crgb(m_color_combox->currentText());
	}
	else if (n == SCATTER_COLOR_METHOD_GRADIENT)
	{
		m_return_scatter_struct->m_render.m_useRGB = false;
	}
	else
	{
		m_return_scatter_struct->m_render.m_useRGB = true;
		m_return_scatter_struct->m_render.m_rgb.m_r = m_red_component_combox->currentIndex();
		m_return_scatter_struct->m_render.m_rgb.m_g = m_green_component_combox->currentIndex();
		m_return_scatter_struct->m_render.m_rgb.m_b = m_blue_component_combox->currentIndex();
	}
		m_return_scatter_struct->m_render.m_gradientMap = m_gradient_color_combox->currentIndex();
	m_return_scatter_struct->m_render.m_labelMark.m_showOn = m_show_line_check->isChecked();
	m_return_scatter_struct->m_render.m_labelMark.m_labelMarkIndex = m_note_font_combox->currentIndex();
	m_return_scatter_struct->m_render.m_labelMark.m_densityIndex = m_density_combox->currentIndex();
	m_return_scatter_struct->m_render.m_labelMark.m_showLine = m_show_line_check->isChecked();
	m_return_scatter_struct->m_render.m_labelMark.m_markType = m_type_combox->currentIndex();
	m_return_scatter_struct->m_render.m_labelMark.m_numCount = m_note_significant_spin->value();
#if 0
#endif
	return m_return_scatter_struct;
}

 void scatter_property_widget::slot_color_method_change(int a) //着色方法
 {
	 m_return_scatter_struct->m_changetype = 4;
	 if (m_color_method->currentIndex() == SCATTER_COLOR_METHOD_FIXED)
	 {
		 m_color_treeitem->setHidden(false);
		 m_gradient_color_treeitem->setHidden(true);
		 m_color_range_treeitem->setHidden(true);
		 m_red_component_treeitem->setHidden(true);
		 m_green_component_treeitem->setHidden(true);
		 m_green_component_treeitem->setHidden(true);
	 }
	 else if (m_color_method->currentIndex() == SCATTER_COLOR_METHOD_GRADIENT)
	 {
		 m_color_treeitem->setHidden(true);
		 m_gradient_color_treeitem->setHidden(false);
		 m_color_range_treeitem->setHidden(true);
		 m_red_component_treeitem->setHidden(true);
		 m_green_component_treeitem->setHidden(true);
		 m_green_component_treeitem->setHidden(true);

	 }
	 else
	 {
		 m_color_treeitem->setHidden(true);
		 m_gradient_color_treeitem->setHidden(true);
		 m_color_range_treeitem->setHidden(false);
		 m_red_component_treeitem->setHidden(false);
		 m_green_component_treeitem->setHidden(false);
		 m_green_component_treeitem->setHidden(false);

	 }
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_density_change(int a)
 {
	 m_update_widget_property();
 }	 

 void scatter_property_widget::slot_symbol_change(int a)
 {
	 m_return_scatter_struct->m_changetype = 1;
	 m_update_widget_property();
 }

void scatter_property_widget::m_size_slider_setValue()
{
	int value =(m_size_edit->text().toFloat() * 1000);
	m_return_scatter_struct->m_changetype = 2;
	disconnect(m_size_slider, SIGNAL(valueChanged(int)), 0, 0);
	m_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_size_slider, SIGNAL(valueChanged(int)), this, SLOT(m_size_spin_setValue(int)));
}

void scatter_property_widget::m_update_widget_property()
{
	G_ScatterMap* pTemp = this->return_scatter_property_struct();
	std::shared_ptr<G_ScatterMap>ptr(std::make_shared<G_ScatterMap>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void scatter_property_widget::slot_update_widget_property()
{
	m_update_widget_property();
}

 void scatter_property_widget::m_size_spin_setValue(int a)
 {
	 if (!m_sliderPressed)
	 {
		 double value = a * 0.001;
		 m_return_scatter_struct->m_changetype = 2;
		 m_size_edit->setText(QString("%1").arg(value));
		 m_update_widget_property();
	 }
 }

 void scatter_property_widget::slot_size_pressed()
 {
	 m_sliderPressed = true;
 }

 void scatter_property_widget::slot_size_released()
 {
	 m_sliderPressed = false;
	 int value = m_size_slider->value();
	 m_size_spin_setValue(value);
 }

 void scatter_property_widget::slot_color_choose()
 {
	 m_return_scatter_struct->m_changetype = 4;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_gradient_color()
 {
	 m_return_scatter_struct->m_changetype = 4;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_show_note()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
	 //to do
 }

 void scatter_property_widget::slot_note_font()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_density()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_show_line()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_type()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
 }

 void scatter_property_widget::slot_sig()
 {
	 m_return_scatter_struct->m_changetype = 3;
	 m_update_widget_property();
 }

void scatter_property_widget::slot_show_color_scale(int a)
 {
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();//更新控件属性值
 }

void scatter_property_widget::slot_place_direction(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();
}

void scatter_property_widget::slot_m_x_position_slider(double a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void scatter_property_widget::slot_m_x_position_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void scatter_property_widget::slot_m_y_position_slider(double a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void scatter_property_widget::slot_m_y_position_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
	//to do
}

void scatter_property_widget::slot_m_width_slider(double a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void scatter_property_widget::slot_m_width_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void scatter_property_widget::slot_m_length_slider(double a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void scatter_property_widget::slot_m_length_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void scatter_property_widget::slot_m_font_size_slider(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void scatter_property_widget::slot_m_font_size_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void scatter_property_widget::slot_m_note_number(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();
}

void scatter_property_widget::slot_m_note_font_size_slider(double a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void scatter_property_widget::slot_m_note_font_size_spin(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_note_font_size_slider(double)));
}

void scatter_property_widget::slot_m_adopt_self_note(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();
}

void scatter_property_widget::slot_color_type(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();
}

void scatter_property_widget::slot_significant(int a)
{
	m_return_scatter_struct->m_changetype = 5;
	m_update_widget_property();
}

void scatter_property_widget::slot_color_scale_property_changed(Child_ColorBar* ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
	/*else if (1 == type)
		set_color_bar_actor(ccb->m_displayOn);*/
	else {
		m_return_scatter_struct->m_changetype = 5;
		G_ScatterMap* pTemp = this->return_scatter_property_struct();
		std::shared_ptr<G_ScatterMap>ptr(std::make_shared<G_ScatterMap>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}

void scatter_property_widget::slot_status_show_input(QTreeWidgetItem* item, int n)
{
	myqwidget  *p = (myqwidget*)parentWidget();
	QTreeWidgetItem *parent = item->parent();
	if (!parent)
	{
		if (item->text(0) == tr("scatter diagram"))
		{
			QString mess = "scatter diagram";
			p->send_signal_show_status_message(mess);
		}
		else
		{
			QString mess = "render";
			p->send_signal_show_status_message(mess);
		}
	}
	else
	{
		int col = parent->indexOfChild(item);
		if (parent->text(0) == tr("scatter diagram"))
		{
			if (col == 0)
			{
				QString mess = "input";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 1)
			{
				QString mess = "density";
				p->send_signal_show_status_message(mess);
			}
		}
		else
		{
			if (col == 0)
			{
				QString mess = "symbol";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 1)
			{
				QString mess = "size";
				p->send_signal_show_status_message(mess);
			}
			else if(col==2)
			{
				QString mess = "color method";
				p->send_signal_show_status_message(mess);
			}
			else
			{
				QString mess = "color";
				p->send_signal_show_status_message(mess);
			}
		}
	}
}

void scatter_property_widget::slot_status_show_note(QTreeWidgetItem *item, int n)
{
	myqwidget  *p = (myqwidget*)parentWidget();
	QTreeWidgetItem *parent = item->parent();
	if (!parent)
	{
		if (item->text(0) == tr("note"))
		{
			QString mess = "note";
			p->send_signal_show_status_message(mess);
		}
	}
	else
	{
		int col = parent->indexOfChild(item);
		if (parent->text(0) == tr("note"))
		{
			if (col == 0)
			{
				QString mess = "show note";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 1)
			{
				QString mess = "note font";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 2)
			{
				QString mess = "note density";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 3)
			{
				QString mess = "show line";
				p->send_signal_show_status_message(mess);
			}
			else if (col == 4)
			{
				QString mess = "note type";
				p->send_signal_show_status_message(mess);
			}
			else
			{
				QString mess = "note significant";
				p->send_signal_show_status_message(mess);
			}
		}
	}

}
