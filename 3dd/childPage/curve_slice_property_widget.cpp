#include "curve_slice_property_widget.h"
//#include "myqwidget.h"
//#include "GSliceMapNode.h"

curve_slice_property_widget::curve_slice_property_widget(int num, SuperNodeStruct* param)
{
	QString label = (param) ? QString::fromStdString(param->m_name) : ( (num == 0)? STRING_LABEL_CURVE_SLICE : STRING_LABEL_CURVE_SLICE + QString::number(num + 1));
	 create_2buttons(this, label);
	 

	m_sliderPressed = false;
	//常规属性
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);
	//QString strtext = m_input_treeitem->text(1); 获取第二列文字

	m_textureFile_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_CURVE_FILE));
	m_texturePath_treeitem = new QTreeWidgetItem(m_textureFile_treeitem, QStringList(STRING_ITEM_PIC_FILE));
	m_textureFile_edit = new QLineEdit;
//	m_Normal_TreeWidget->setItemWidget(m_textureFile_treeitem, 1, m_textureFile_edit);
	m_file_button = new QPushButton;
	m_file_button->setText("...");

	QWidget *file_widget = new QWidget;
	QHBoxLayout *file_layout = new QHBoxLayout;
	file_layout->addWidget(m_textureFile_edit);
	file_layout->addWidget(m_file_button);
	file_widget->setLayout(file_layout);
	m_normal_property->setItemWidget(m_texturePath_treeitem, 1, file_widget);

	m_render_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_RENDER));
	m_lighting_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_LIGHTING));
	m_opacity_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_OPACITY));
	m_color_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_COLORMETHOD)));
	m_lighting = new QCheckBox;
	m_lighting->setChecked(true);
	//m_lighting->isChecked();返回qchckbox选中状态，bool类型
	m_normal_property->setItemWidget(m_lighting_treeitem, 1,m_lighting);
	m_opacity_spin = new QDoubleSpinBox;
	m_opacity_spin->setDecimals(4);
	m_opacity_spin->setRange(0, 1);
	m_opacity_spin->setSingleStep(0.1);
	m_opacity_spin->setValue(1);
	m_opacity_slider = new QSlider(Qt::Horizontal);
	m_opacity_slider->setRange(0, 10000);
	m_opacity_slider->setValue(10000);
	QWidget *opacity_widget = new QWidget;
	QHBoxLayout *opacity_layout = new QHBoxLayout;
	opacity_layout->addWidget(m_opacity_spin);
	opacity_layout->addSpacing(10);
	opacity_layout->addWidget(m_opacity_slider);
	opacity_widget->setLayout(opacity_layout);
	m_normal_property->setItemWidget(m_opacity_treeitem,1,opacity_widget);
	m_color_method_combox = new QComboBox;
	gradient_color(m_color_method_combox);
	m_normal_property->setItemWidget(m_color_method_treeitem, 1, m_color_method_combox);
	//m_normal_property->insertTopLevelItem(0, m_slice_treeitem);
	//m_normal_property->insertTopLevelItem(1,m_render_treeitem);
	m_normal_property->expandAll();

	//切割平面
	m_cut_plane = new cut_plane_property();
	m_cut_plane->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
#if 0
	m_cut_plane->m_offset_from_center_edit->setText("0");
	m_cut_plane->m_offset_from_center_slider->setRange(0, 1200);//
	m_cut_plane->m_offset_from_center_slider->setValue(600);
#endif

	//色标刻度尺
	m_color_scale = new color_scale_property();
	m_color_scale->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_return_slice_property_struct = new G_Curve_Slice;
	baseStruct = m_return_slice_property_struct;
	
	if (!param) {
		GCurveSliceNode::getDefault(baseStruct);
	}
	else
	{
		*m_return_slice_property_struct = *(G_Curve_Slice *)param;
	}
	G_Curve_Slice *slice= m_return_slice_property_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(slice->m_putIn));
		m_textureFile_edit->setText(slice->picFilePath.c_str());//"2.jpg"
		m_lighting->setChecked(slice->m_lightOn);
		m_opacity_spin->setValue(slice->m_opacityValue);
		m_opacity_slider->setValue((int)(slice->m_opacityValue * 10000));
		m_color_method_combox->setCurrentIndex(slice->m_colorMethod);

		m_cut_plane->set_data(&slice->m_cutPlane, 1000, 500);
		m_color_scale->set_data(&slice->m_colorBar);
	}
/*
使用QTabWidget的正常方法是执行以下操作：

创建一个QTabWidget。
为每一个标签页创建一个QWidget作为页控件，但不要为它们指定父类。
向页控件中插入子控件，使用使用布局工具来对它们进行布局。
调用addTab()或者insertTab()将页控件插入标签控件，并为每一个标签分配一个标签字符串，快捷键可加可不加。
*/
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
//	m_form1_tab->addTab(m_cut_plane,	STRING_ITEM_CUTPLANE);
	m_form1_tab->addTab(m_color_scale,	STRING_TAB_COLOR_SCALE);

	this->NodeType = STRING_GTYPE_CURVE_SLICE;
	m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	m_can_append_widget.push_back(STRING_WIDGET_BORDER);
	connect(m_textureFile_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property()));
	connect(m_file_button, SIGNAL(clicked(bool)), this, SLOT(slot_select_file()));
	connect(m_lighting, SIGNAL(stateChanged(int)), this, SLOT(slot_lighting()));
	connect(m_opacity_spin, SIGNAL(valueChanged(double)), this, SLOT(m_slider_setValue(double)));
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_spin_setValue(int)));
	connect(m_opacity_slider, SIGNAL(sliderPressed()), this, SLOT(slot_slider_pressed()));
	connect(m_opacity_slider, SIGNAL(sliderReleased()), this, SLOT(slot_slider_relesed()));
	connect(m_color_method_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_method()));

	m_cut_plane->connect_items();
	connect(m_cut_plane, SIGNAL(cut_plane_property_changed(Child_CutingPlane* , int )), this, SLOT(slot_cut_plane_property_changed(Child_CutingPlane* , int )));

	m_color_scale->connect_items();
	connect(m_color_scale, SIGNAL(color_scale_property_changed(Child_ColorBar* , int )), this, SLOT(slot_color_scale_property_changed(Child_ColorBar* , int )));
}

curve_slice_property_widget::~curve_slice_property_widget()
{
	delete m_form1_tab;
}

void curve_slice_property_widget::setCutPlaneRange(float* dataRange)
{
		m_cut_plane->set_range(dataRange);
}

void curve_slice_property_widget::setCutPoint(double* dataRange)
{
	ia_tmr.stop();
	ia_tmr.start();
	m_cut_plane->set_normal(dataRange);
}

void curve_slice_property_widget::paintEvent(QPaintEvent *e)
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

SuperNodeStruct* curve_slice_property_widget::return_slice_property_struct()
{
	getBaseStruct();
	//G_SliceMap *m_return_slice_property_struct= (G_SliceMap *)baseStruct;
	m_return_slice_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_slice_property_struct->m_putIndex = 0;//m_input_component_treeitem->text(1).toInt();
	m_return_slice_property_struct->picFilePath = m_textureFile_edit->text().toStdString();
	m_return_slice_property_struct->m_lightOn = m_lighting->isChecked();
	m_return_slice_property_struct->m_opacityValue = m_opacity_spin->value();
	m_return_slice_property_struct->m_colorMethod = GRADIENT_OF_COLOR;
	m_return_slice_property_struct->m_gradientMap = m_color_method_combox->currentIndex();

	return m_return_slice_property_struct;
}

void curve_slice_property_widget::contextMenuEvent(QContextMenuEvent *event)
{
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
	/*
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
	}
	menu->addAction(STRING_ACTION_CONN_OUT,	this,	SLOT(slot_connect_output()));
	*/
//	menu->addAction(new QAction(tr("connect output module"), this));
	menu->addAction(STRING_ACTION_CONN_IN,		this,	SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_EXPORT,		this,	SLOT(slot_save_widget()));	//menu->addAction(new QAction(tr("save data"), this));
	menu->addAction(STRING_ACTION_COPY,			this,	SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME,		this,	SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1,	SLOT(delete_slice()));	
	menu->move(cursor().pos());
	menu->show();
}

void curve_slice_property_widget::delete_self()
{
	emit sig_delete_curve_slice();
}

void curve_slice_property_widget::m_update_widget_property()
{
	G_Curve_Slice* pTemp = (G_Curve_Slice*)this->return_slice_property_struct();
	std::shared_ptr<G_Curve_Slice>ptr(std::make_shared<G_Curve_Slice>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void curve_slice_property_widget::slot_select_file()
{
	QString file_name = QFileDialog::getOpenFileName(this, STRING_CURVE_FILE, ".", "filter (*.*)");
	if (file_name.isEmpty())
		return;
	baseStruct->m_changetype = G_Picture_MN_CT + 0xf0;
	m_textureFile_edit->setText(file_name);
}

void curve_slice_property_widget::slot_slice_to_axis()
{
	emit sig_slice_to_axis();
}

void curve_slice_property_widget::slot_slice_to_border()
{
	emit sig_slice_to_border();
}

void curve_slice_property_widget::slot_lighting()
{
	m_update_widget_property();
}

void curve_slice_property_widget::m_slider_setValue(double a)
{
	baseStruct->m_changetype = 1;
	disconnect(m_opacity_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_opacity_slider->setValue(value);
	m_update_widget_property();
	connect(m_opacity_slider, SIGNAL(valueChanged(int)), this, SLOT(m_spin_setValue(int)));
}

void curve_slice_property_widget::m_spin_setValue(int a)
{
	//if (!m_sliderPressed)
	{
		baseStruct->m_changetype = 1;
		disconnect(m_opacity_spin, 0, 0, 0);
		double value = a / 10000.0;
		m_opacity_spin->setValue(value);
		m_update_widget_property();
		connect(m_opacity_spin, SIGNAL(valueChanged(double)), this, SLOT(m_slider_setValue(double)));
	}
}

void curve_slice_property_widget::slot_slider_pressed()
{
	m_sliderPressed = true;
}

void curve_slice_property_widget::slot_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_opacity_slider->value();
	m_spin_setValue(value);
}

void curve_slice_property_widget::slot_color_method()
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_place_direction()
{
	baseStruct->m_changetype = 4;
	m_update_widget_property();
}

void curve_slice_property_widget::slot_x_normal()
{
	baseStruct->m_changetype = 4;
	m_update_widget_property();
}

void curve_slice_property_widget::slot_y_normal()
{
	baseStruct->m_changetype = 4;
	m_update_widget_property();
}

void curve_slice_property_widget::slot_z_normal()
{
	baseStruct->m_changetype = 4;
	m_update_widget_property();
}

void curve_slice_property_widget::slot_offset_from_center_slider()
{
	baseStruct->m_changetype = 4;
	disconnect(m_cut_plane->m_offset_from_center_slider, 0, 0, 0);
	int value =(m_cut_plane->m_offset_from_center_edit->text().toFloat()) * 100 + 600;
	m_cut_plane->m_offset_from_center_slider->setValue(value);
	m_update_widget_property();
	connect(m_cut_plane->m_offset_from_center_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_offset_from_center_spin(int)));
}

void curve_slice_property_widget::slot_offset_from_center_spin(int a)
{
	if (!m_sliderPressed)
	{
		baseStruct->m_changetype = 4;
	disconnect(m_cut_plane->m_offset_from_center_edit, 0, 0, 0);
		float value = (a - 600)*0.01;
		m_cut_plane->m_offset_from_center_edit->setText(QString("%1").arg(value));
		m_update_widget_property();
	connect(m_cut_plane->m_offset_from_center_edit, SIGNAL(valueChanged(int)), this, SLOT(slot_offset_from_center_spin(int)));
	}
}

void curve_slice_property_widget::slot_offset_center_slider_pressed()
{
	m_sliderPressed = true;
}

void curve_slice_property_widget::slot_offset_center_slider_relesed()
{
	m_sliderPressed = false;
	int value = m_cut_plane->m_offset_from_center_slider->value();
	slot_offset_from_center_spin(value);
}

void curve_slice_property_widget::slot_cut_plane_property_changed(Child_CutingPlane*, int type)
{
	baseStruct->m_changetype = G_Slice_MN_CT + type;
	m_update_widget_property();
}

void curve_slice_property_widget::slot_show_color_scale(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_place_direction(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_m_x_position_slider(double a)
{
	disconnect(m_color_scale->m_x_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_x_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void curve_slice_property_widget::slot_m_x_position_spin(int a)
{
	disconnect(m_color_scale->m_x_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_x_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void curve_slice_property_widget::slot_m_y_position_slider(double a)
{
	disconnect(m_color_scale->m_y_position_slider, 0, 0, 0);
	int value = (int)(a * 10000);
	m_color_scale->m_y_position_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void curve_slice_property_widget::slot_m_y_position_spin(int a)
{
	disconnect(m_color_scale->m_y_position_spin, 0, 0, 0);
	double value = a / 10000.0;
	m_color_scale->m_y_position_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void curve_slice_property_widget::slot_m_width_slider(double a)
{
	disconnect(m_color_scale->m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_color_scale->m_width_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void curve_slice_property_widget::slot_m_width_spin(int a)
{
	disconnect(m_color_scale->m_width_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_width_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void curve_slice_property_widget::slot_m_length_slider(double a)
{
	disconnect(m_color_scale->m_length_slider, 0, 0, 0);
	int value = (int)(a * 100);
	m_color_scale->m_length_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void curve_slice_property_widget::slot_m_length_spin(int a)
{
	disconnect(m_color_scale->m_length_spin, 0, 0, 0);
	double value = a * 0.01;
	m_color_scale->m_length_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void curve_slice_property_widget::slot_m_font_size_slider(int a)
{
	disconnect(m_color_scale->m_font_size_slider, 0, 0, 0);
	m_color_scale->m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void curve_slice_property_widget::slot_m_font_size_spin(int a)
{
	disconnect(m_color_scale->m_font_size_spin, 0, 0, 0);
	m_color_scale->m_font_size_spin->setValue(a);
	m_update_widget_property();
	connect(m_color_scale->m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void curve_slice_property_widget::slot_m_note_number(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_m_note_font_size_slider(int a)
{
	disconnect(m_color_scale->m_note_font_size_slider, 0, 0, 0);
	int value = a;
	m_color_scale->m_note_font_size_slider->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void curve_slice_property_widget::slot_m_note_font_size_spin(int a)
{
	disconnect(m_color_scale->m_note_font_size_spin, 0, 0, 0);
	int value = a ;
	m_color_scale->m_note_font_size_spin->setValue(value);
	m_update_widget_property();
	connect(m_color_scale->m_note_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_slider(int)));
}

void curve_slice_property_widget::slot_m_adopt_self_note(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_color_type(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_significant(int a)
{
	m_update_widget_property();
}

void curve_slice_property_widget::slot_color_scale_property_changed(Child_ColorBar*ccb, int type)
{
	if (0 == type)
		m_update_widget_property();
/*	else if (1 == type){
		set_color_bar_actor(ccb->m_displayOn);
	}*/
	else{
		m_return_slice_property_struct->m_changetype = 5;
		G_Curve_Slice* pTemp = (G_Curve_Slice* )this->return_slice_property_struct();
		std::shared_ptr<G_Curve_Slice>ptr(std::make_shared<G_Curve_Slice>());
		*(ptr.get()) = *pTemp;
		update_display(ptr);
	}
}
