#include "picture_property_widget.h"
#include "myqwidget.h"

picture_property_widget::picture_property_widget(int num, QString filename, G_Picture *data)
{
	m_return_picture_property_struct = new G_Picture();
	baseStruct = m_return_picture_property_struct;
	QString label = (data) ? QString::fromStdString(data->m_name) : ( (num == 0)? STRING_LABEL_PICTURE : STRING_LABEL_PICTURE + QString::number(num + 1));
	 create_2buttons(this, label);
	//常规属性
	m_Normal_TreeWidget = new QTreeWidget;
	m_Normal_TreeWidget->setColumnCount(2);
	m_Normal_TreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_Normal_TreeWidget->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_pre_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	
	m_normal_property_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_PICTURE));

	m_textureFile_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_PIC_FILE));
	m_textureFile_edit = new QLineEdit;
	m_textureFile_edit->setText(filename);//"2.jpg"
//	m_Normal_TreeWidget->setItemWidget(m_textureFile_treeitem, 1, m_textureFile_edit);
	m_file_button = new QPushButton;
	m_file_button->setText("...");

	m_drag_treeitem=new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_PIC_DRAG));
	m_drag = new QCheckBox;
	m_drag->setChecked(false);
	m_Normal_TreeWidget->setItemWidget(m_drag_treeitem, 1, m_drag);
	m_drag_treeitem->setHidden(true);

	QWidget *file_widget = new QWidget;
	QHBoxLayout *file_layout = new QHBoxLayout;
	file_layout->addWidget(m_textureFile_edit);
	file_layout->addWidget(m_file_button);
	file_widget->setLayout(file_layout);
	m_Normal_TreeWidget->setItemWidget(m_textureFile_treeitem, 1, file_widget);

	m_pic_x_start_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(QString(STRING_ITEM_PIC_XSTART)));
	m_x_start_slider = new QSlider(Qt::Horizontal);
	m_x_start_slider->setRange(0, 1000);
	m_x_start_slider->setValue(333);
	//QWidget *x_start_widget = new QWidget;
	//QHBoxLayout *x_start_layout = new QHBoxLayout;
	//x_start_layout->addWidget(m_x_start_slider);
	//x_start_widget->setLayout(x_start_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_x_start_treeitem, 1, m_x_start_slider);//x_start_widget

	m_pic_x_end_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(QString(STRING_ITEM_PIC_XEND)));
	m_x_end_slider = new QSlider(Qt::Horizontal);
	m_x_end_slider->setRange(0, 1000 - 1);	//-1 防止超范围造成图像失步
	m_x_end_slider->setValue(666);
	m_Normal_TreeWidget->setItemWidget(m_pic_x_end_treeitem, 1, m_x_end_slider);

	m_pic_y_start_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(QString(STRING_ITEM_PIC_YSTART)));
	m_y_start_slider = new QSlider(Qt::Horizontal);
	m_y_start_slider->setRange(0, 1000 - 1);
	m_y_start_slider->setValue(333);
	m_Normal_TreeWidget->setItemWidget(m_pic_y_start_treeitem, 1, m_y_start_slider);

	m_pic_y_end_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(QString(STRING_ITEM_PIC_YEND)));
	m_y_end_slider = new QSlider(Qt::Horizontal);
	m_y_end_slider->setRange(0, 1000 - 1);
	m_y_end_slider->setValue(666);
	m_Normal_TreeWidget->setItemWidget(m_pic_y_end_treeitem, 1, m_y_end_slider);

	 m_pic_adopt_treeitem= new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(QString(STRING_ITEM_PIC_ADOPT)));

	 m_pic_operate_treeitem = new QTreeWidgetItem(m_pic_adopt_treeitem, QStringList(QString(STRING_ITEM_PIC_OPERATE)));
	QHBoxLayout *operate_layou = new QHBoxLayout;
	QWidget *operate_widget = new QWidget;
	m_apply = new QCheckBox;		m_apply->setText(STRING_ITEM_PIC_APPLY);
	m_reset_button = new QPushButton;	m_reset_button->setText(STRING_ITEM_PIC_RESET);
	 m_adopt_combo = new QComboBox;
	 m_adopt_combo->insertItem(0, STRING_ITEM_PIC_ADJUST_POS);
	 m_adopt_combo->insertItem(1, STRING_ITEM_PIC_RECT_SEL);	//弃用
	 m_adopt_combo->setCurrentIndex(0);
	//m_Normal_TreeWidget->setItemWidget(m_pic_adopt_treeitem, 1, m_adopt_combo);
	//operate_layou->addWidget(m_adopt_combo, 40);
	operate_layou->addWidget(m_reset_button, 30);
	operate_layou->addWidget(m_apply, 20);
	operate_widget->setLayout(operate_layou);
	m_Normal_TreeWidget->setItemWidget(m_pic_operate_treeitem, 1, operate_widget);


	m_pic_adjust_pos_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(QString(STRING_ITEM_PIC_ADJUST_POS)));

	m_pic_x_offset_treeitem = new QTreeWidgetItem(m_pic_adjust_pos_treeitem, QStringList(QString(STRING_ITEM_PIC_XOFFSET)));
	m_pic_x_offset = new QSlider(Qt::Horizontal);
	m_pic_x_offset->setRange(0, SLIDER_OFFSET_MAX-1);
	m_pic_x_offset->setValue(SLIDER_OFFSET_MID);
	QHBoxLayout *x_offset_layout = new QHBoxLayout;
	QWidget *x_offset_widget = new QWidget;
	m_x_offset_edit = new QLineEdit;
	x_offset_layout->addWidget(m_x_offset_edit, 20);
	x_offset_layout->addWidget(m_pic_x_offset, 80);
	x_offset_widget->setLayout(x_offset_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_x_offset_treeitem, 1, x_offset_widget);

	m_pic_y_offset_treeitem = new QTreeWidgetItem(m_pic_adjust_pos_treeitem, QStringList(QString(STRING_ITEM_PIC_YOFFSET)));
	m_pic_y_offset = new QSlider(Qt::Horizontal);
	m_pic_y_offset->setRange(0, SLIDER_OFFSET_MAX-1);
	m_pic_y_offset->setValue(SLIDER_OFFSET_MID);
	QHBoxLayout *y_offset_layout = new QHBoxLayout;
	QWidget *y_offset_widget = new QWidget;
	m_y_offset_edit = new QLineEdit;
	y_offset_layout->addWidget(m_y_offset_edit, 20);
	y_offset_layout->addWidget(m_pic_y_offset, 80);
	y_offset_widget->setLayout(y_offset_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_y_offset_treeitem, 1, y_offset_widget);

	m_pic_z_offset_treeitem = new QTreeWidgetItem(m_pic_adjust_pos_treeitem, QStringList(QString(STRING_ITEM_PIC_ZOFFSET)));
	m_pic_z_offset = new QSlider(Qt::Horizontal);
	m_pic_z_offset->setRange(0, SLIDER_OFFSET_MAX-1);
	m_pic_z_offset->setValue(SLIDER_OFFSET_MID);
	QHBoxLayout *z_offset_layout = new QHBoxLayout;
	QWidget *z_offset_widget = new QWidget;
	m_z_offset_edit = new QLineEdit;
	z_offset_layout->addWidget(m_z_offset_edit, 20);
	z_offset_layout->addWidget(m_pic_z_offset, 80);
	z_offset_widget->setLayout(z_offset_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_z_offset_treeitem, 1, z_offset_widget);

	m_pic_scale_treeitem= new QTreeWidgetItem(m_pic_adjust_pos_treeitem, QStringList(QString(STRING_ITEM_PIC_SCALE)));
	m_pic_scale = new QSlider(Qt::Horizontal);
	m_pic_scale->setRange(200, 13300);
	m_pic_scale->setValue(3010);
	QHBoxLayout *pic_scale_layout = new QHBoxLayout;
	QWidget *pic_scale_widget = new QWidget;
	m_pic_scale_edit= new QLineEdit;
	pic_scale_layout->addWidget(m_pic_scale_edit, 20);
	pic_scale_layout->addWidget(m_pic_scale, 80);
	pic_scale_widget->setLayout(pic_scale_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_scale_treeitem, 1, pic_scale_widget);
	m_scalePressed = false;

	m_pic_z_rotate_treeitem = new QTreeWidgetItem(m_pic_adjust_pos_treeitem, QStringList(QString(STRING_ITEM_PIC_ZROTATE)));
	m_pic_z_rotate = new QSlider(Qt::Horizontal);
	m_pic_z_rotate->setRange(0, 3600);
	m_pic_z_rotate->setValue(1800);

	QHBoxLayout *z_rotate_layout = new QHBoxLayout;
	QWidget *z_rotate_widget = new QWidget;
	m_z_rotate_edit = new QLineEdit;
	z_rotate_layout->addWidget(m_z_rotate_edit, 20);
	z_rotate_layout->addWidget(m_pic_z_rotate, 80);
	z_rotate_widget->setLayout(z_rotate_layout);
	m_Normal_TreeWidget->setItemWidget(m_pic_z_rotate_treeitem, 1, z_rotate_widget);

	m_pic_rect_sel_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(QString(STRING_ITEM_PIC_RECT_SEL)));
	m_pic_vertex_treeitem[0]= new QTreeWidgetItem(m_pic_rect_sel_treeitem, QStringList(QString(STRING_ITEM_PIC_VERTEX1)));
	m_pic_vertex_treeitem[1] = new QTreeWidgetItem(m_pic_rect_sel_treeitem, QStringList(QString(STRING_ITEM_PIC_VERTEX2)));
	m_pic_vertex_treeitem[2] = new QTreeWidgetItem(m_pic_rect_sel_treeitem, QStringList(QString(STRING_ITEM_PIC_VERTEX3)));
	QHBoxLayout *point_layout[3];
	QWidget *select_point_widget[3];
	int ic;
	for (ic = 0; ic < 3; ic++) {
		m_select_point[ic] = new QCheckBox;
		m_select_point[ic]->setChecked(false);

		m_label_point[ic] = new QLabel;

		point_layout[ic] = new QHBoxLayout;

		point_layout[ic]->addWidget(m_select_point[ic], 15);
		point_layout[ic]->addWidget(m_label_point[ic], 85);
		select_point_widget[ic] = new QWidget;
		select_point_widget[ic]->setLayout(point_layout[ic]);
		m_Normal_TreeWidget->setItemWidget(m_pic_vertex_treeitem[ic], 1, select_point_widget[ic]);
	}
	m_pic_rect_sel_treeitem->setHidden(true);
	m_Normal_TreeWidget->expandAll();

	if (!data) {
		GPictureNode::getDefault(baseStruct);
		data = (G_Picture*)baseStruct;
	}
	{
		m_input_treeitem->setText(1, QString::fromStdString(data->m_putIn));
		float ftmp = log10f(data->xyz_scale[0] + 1.0) *PIC_SCALE_BASE;
		int itmp = ftmp;
		m_pic_scale_edit->setText(QString("%1").arg( data->xyz_scale[0]));
		m_pic_scale->setValue(itmp);
		m_x_start_slider->setValue(data->xy_range[0]);
		m_x_end_slider->setValue(data->xy_range[1]);
		m_y_start_slider->setValue(data->xy_range[2]);
		m_y_end_slider->setValue(data->xy_range[3]);
		m_adopt_combo->setDisabled(true);
		ftmp = data->xyz_offset[0];
		itmp = (ftmp + 1.0)* SLIDER_OFFSET_MID;
		m_x_offset_edit->setText(QString("%1").arg(ftmp));
		m_pic_x_offset->setValue(itmp );
		ftmp = data->xyz_offset[1];
		itmp = (ftmp + 1.0)* SLIDER_OFFSET_MID;
		m_y_offset_edit->setText(QString("%1").arg(ftmp));
		m_pic_y_offset->setValue(itmp);
		ftmp = data->xyz_offset[2];
		itmp = (ftmp + 1.0)* SLIDER_OFFSET_MID;
		m_z_offset_edit->setText(QString("%1").arg(ftmp));
		m_pic_z_offset->setValue(itmp);
		ftmp = data->xyz_offset[3];
		itmp = (ftmp * 10)+ 1800;
		m_z_rotate_edit->setText(QString("%1").arg(ftmp));
		m_pic_z_rotate->setValue(itmp);
		m_apply->setChecked(data->apply);
		m_reset_button->setDisabled(data->apply);
		m_pic_x_offset->setDisabled(data->apply);
		m_pic_y_offset->setDisabled(data->apply);
		m_pic_z_offset->setDisabled(data->apply);
		m_pic_scale->setDisabled(data->apply);
		m_pic_z_rotate->setDisabled(data->apply);
	}

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_Normal_TreeWidget, QString(STRING_TAB_GENERAL));
	
	this->NodeType = STRING_GTYPE_PICTURE;
	m_can_append_widget.push_back(STRING_WIDGET_AXIS);

	connect(m_textureFile_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property()));
	connect(m_file_button, SIGNAL(clicked(bool)), this, SLOT(slot_select_file()));

	//connect(m_pic_scale, SIGNAL(sliderPressed()), this, SLOT(slot_pic_scale_pressed()));
	//connect(m_pic_scale, SIGNAL(sliderReleased()), this, SLOT(slot_pic_scale_relesed()));
	//connect(m_adopt_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_adopt_combo()));

	connect(m_x_start_slider, SIGNAL(valueChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_x_end_slider, SIGNAL(valueChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_y_start_slider, SIGNAL(valueChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_y_end_slider, SIGNAL(valueChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_reset_button, SIGNAL(clicked(bool)), this, SLOT(slot_reset_offset()));
	connect(m_apply, SIGNAL(stateChanged(int)), this, SLOT(slot_pic_apply(int)));
	connect(m_x_offset_edit, SIGNAL(editingFinished()), this, SLOT(slot_x_offset_edit()));
	connect(m_y_offset_edit, SIGNAL(editingFinished()), this, SLOT(slot_y_offset_edit()));
	connect(m_z_offset_edit, SIGNAL(editingFinished()), this, SLOT(slot_z_offset_edit()));
	connect(m_z_rotate_edit, SIGNAL(editingFinished()), this, SLOT(slot_z_rotate_edit()));
	connect(m_pic_scale_edit, SIGNAL(editingFinished()), this, SLOT(slot_scale_edit()));
	connect(m_pic_x_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_x_offset_slider()));
	connect(m_pic_y_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_y_offset_slider()));
	connect(m_pic_z_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_z_offset_slider()));
	connect(m_pic_scale, SIGNAL(valueChanged(int)), this, SLOT(slot_scale_slider()));
	connect(m_pic_z_rotate, SIGNAL(valueChanged(int)), this, SLOT(slot_z_rotate_slider()));
	//connect(m_drag, SIGNAL(stateChanged(int)), this, SLOT(slot_drag_enable()));
	//connect(m_select_point[0], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point1()));
	//connect(m_select_point[1], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point2()));
	//connect(m_select_point[2], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point3()));
}

picture_property_widget::~picture_property_widget()
{
	delete m_form1_tab;
}

void picture_property_widget::paintEvent(QPaintEvent *e)
{
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

void picture_property_widget::contextMenuEvent(QContextMenuEvent  *event){
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(tr("graphic output"));
	out_put->addAction(tr("axis"), this, SLOT(slot_border_to_axis()));
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
	menu->addAction(new QAction(tr("save data"), this));
#endif
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_picture()));
	menu->move(cursor().pos());
	menu->show();
}

void picture_property_widget::delete_self()
{
	emit sig_delete_picture();
}

G_Picture* picture_property_widget::return_picture_form_property_struct()
{
	getBaseStruct();
	m_return_picture_property_struct->picFilePath = m_textureFile_edit->text().toStdString();
	m_return_picture_property_struct->xy_range[0] = m_x_start_slider->value();
	m_return_picture_property_struct->xy_range[1] = m_x_end_slider->value() ;
	m_return_picture_property_struct->xy_range[2] = m_y_start_slider->value() ;
	m_return_picture_property_struct->xy_range[3] = m_y_end_slider->value();
	m_return_picture_property_struct->xyz_offset[0] = m_x_offset_edit->text().toFloat();
	m_return_picture_property_struct->xyz_offset[1] = m_y_offset_edit->text().toFloat();
	m_return_picture_property_struct->xyz_offset[2] = m_z_offset_edit->text().toFloat();
	m_return_picture_property_struct->xyz_offset[3] = m_z_rotate_edit->text().toFloat();
	m_return_picture_property_struct->apply = m_apply->isChecked();
	m_return_picture_property_struct->xyz_scale[0]= m_pic_scale_edit->text().toFloat();
	m_return_picture_property_struct->xyz_scale[1]= m_return_picture_property_struct->xyz_scale[0];
	m_return_picture_property_struct->xyz_scale[2]= m_return_picture_property_struct->xyz_scale[0];
	return m_return_picture_property_struct;
}

void picture_property_widget::m_update_widget_property()
{
	G_Picture* pTemp = this->return_picture_form_property_struct();
	std::shared_ptr<G_Picture>ptr(std::make_shared<G_Picture>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
	baseStruct->m_changetype = 0;
}

void picture_property_widget::picture_change(int type)
{
	baseStruct->m_changetype = G_Picture_MN_CT + type;
	m_update_widget_property();
}

void picture_property_widget::slot_scale_edit()
{
	float sc = log10f(m_pic_scale_edit->text().toFloat() + 1.0) *PIC_SCALE_BASE;
	int picScale = sc;
	disconnect(m_pic_scale, 0, 0, 0);
	m_pic_scale->setValue(picScale);
	connect(m_pic_scale, SIGNAL(valueChanged(int)), this, SLOT(slot_scale_slider()));
	picture_change( 0x80);
}

void picture_property_widget::slot_scale_slider()
{
	int	picScale = m_pic_scale->sliderPosition();
	float sc= pow(10.0, (picScale / PIC_SCALE_BASE)) - 1;
	//disconnect(m_pic_scale_edit, 0, 0, 0);
	m_pic_scale_edit->setText(QString("%1").arg(sc ));
	//connect(m_pic_scale_edit, SIGNAL(editingFinished()), this, SLOT(slot_scale_edit()));
	picture_change(0x80);
}

void picture_property_widget::slot_pic_scale_pressed()
{
	//m_scalePressed= true;
}

void picture_property_widget::slot_pic_scale_relesed()
{
	//m_scalePressed= false;
	//picture_change(0xf0);
}

void picture_property_widget::slot_adopt_combo()
{
	bool adj = m_adopt_combo->currentIndex() == 0;
	m_pic_adjust_pos_treeitem->setHidden(!adj);
	m_pic_rect_sel_treeitem->setHidden(adj);
	//picture_change(20 + (adj ? 0 : 1)); 
}

void picture_property_widget::slot_pic_apply(int s)
{
	bool ap = s;
	m_reset_button->setDisabled(ap);
	m_pic_x_offset->setDisabled(ap);
	m_pic_y_offset->setDisabled(ap);
	m_pic_z_offset->setDisabled(ap);
	m_pic_scale->setDisabled(ap);
	m_pic_z_rotate->setDisabled(ap);

	picture_change(s?0xa0:0x80);
}

void picture_property_widget::slot_x_offset_edit()
{
	float ftmp = m_x_offset_edit->text().toFloat();
	disconnect(m_pic_x_offset, 0, 0, 0);
	m_pic_x_offset->setValue(ftmp * SLIDER_OFFSET_MID);
	connect(m_pic_x_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_x_offset_slider()));
	picture_change(0x80);
}

void picture_property_widget::slot_y_offset_edit()
{
	float ftmp = m_y_offset_edit->text().toFloat();
	disconnect(m_pic_y_offset, 0, 0, 0);
	m_pic_y_offset->setValue(ftmp * SLIDER_OFFSET_MID);
	connect(m_pic_y_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_y_offset_slider()));
	picture_change(0x80);
}

void picture_property_widget::slot_z_offset_edit()
{
	float ftmp = m_z_offset_edit->text().toFloat();
	disconnect(m_pic_z_offset, 0, 0, 0);
	m_pic_z_offset->setValue(ftmp * SLIDER_OFFSET_MID);
	connect(m_pic_z_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_z_offset_slider()));
	picture_change(0x80);
}

void picture_property_widget::slot_z_rotate_edit()
{
	float ftmp = m_z_rotate_edit->text().toFloat();
	disconnect(m_pic_z_rotate, 0, 0, 0);
	m_pic_z_rotate->setValue(ftmp * SLIDER_OFFSET_MID);
	connect(m_pic_z_rotate, SIGNAL(valueChanged(int)), this, SLOT(slot_z_rotate_slider()));
	picture_change(0x80);
}

void picture_property_widget::slot_x_offset_slider()
{
	float ftmp = 1.0* m_pic_x_offset->value()  / SLIDER_OFFSET_MID - 1.0;
	m_x_offset_edit->setText(QString("%1").arg(ftmp));
	picture_change(0x80);
}

void picture_property_widget::slot_y_offset_slider()
{
	float ftmp = 1.0* m_pic_y_offset->value()  / SLIDER_OFFSET_MID - 1.0;
	m_y_offset_edit->setText(QString("%1").arg(ftmp));
	picture_change(0x80);
}

void picture_property_widget::slot_z_offset_slider()
{
	float ftmp = 1.0* m_pic_z_offset->value()  / SLIDER_OFFSET_MID - 1.0;
	m_z_offset_edit->setText(QString("%1").arg(ftmp));
	picture_change(0x80);
}

void picture_property_widget::slot_z_rotate_slider()
{
	float ftmp = ( m_pic_z_rotate->value() -1800)  /  10.0;
	m_z_rotate_edit->setText(QString("%1").arg(ftmp));
	picture_change(0x80);
}

void picture_property_widget::slot_reset_offset()
{
	float ftmp = 0.0;
	int itmp = SLIDER_OFFSET_MID;
	disconnect(m_pic_x_offset, 0, 0, 0);
	disconnect(m_pic_y_offset, 0, 0, 0);
	disconnect(m_pic_z_offset, 0, 0, 0);
	disconnect(m_pic_scale, 0, 0, 0);
	disconnect(m_pic_z_rotate, 0, 0, 0);
	m_x_offset_edit->setText(QString("%1").arg(ftmp));
	m_pic_x_offset->setValue(itmp);
	m_y_offset_edit->setText(QString("%1").arg(ftmp));
	m_pic_y_offset->setValue(itmp);
	m_z_offset_edit->setText(QString("%1").arg(ftmp));
	m_pic_z_offset->setValue(itmp);
	m_z_rotate_edit->setText(QString("%1").arg(ftmp));
	m_pic_z_rotate->setValue(1800);
	ftmp = 1.0;
	m_pic_scale_edit->setText(QString("%1").arg(ftmp));
	itmp = log10f(1.0+ 1.0) *PIC_SCALE_BASE;
	m_pic_scale->setValue(itmp);
	connect(m_pic_x_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_x_offset_slider()));
	connect(m_pic_y_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_y_offset_slider()));
	connect(m_pic_z_offset, SIGNAL(valueChanged(int)), this, SLOT(slot_z_offset_slider()));
	connect(m_pic_scale, SIGNAL(valueChanged(int)), this, SLOT(slot_scale_slider()));
	connect(m_pic_z_rotate, SIGNAL(valueChanged(int)), this, SLOT(slot_z_rotate_slider()));
	picture_change(0x80);
}

void picture_property_widget::slot_select_file()
{
	QString file_name = QFileDialog::getOpenFileName(this, STRING_OPEN_TEXTUREFILE, ".", "filter (*.*)");
	if (file_name.isEmpty())
		return;
	baseStruct->m_changetype = G_Picture_MN_CT + 0xf0;
	m_textureFile_edit->setText(file_name);
}

void picture_property_widget::slot_drag_enable()
{
	//picture_change(20 +(m_drag->isChecked()?1:0));
}

void picture_property_widget::select_point(int ch)
{
	int ic;
	int type = 22;
	disconnect(m_select_point[0], 0, 0, 0);
	disconnect(m_select_point[1], 0, 0, 0);
	disconnect(m_select_point[2], 0, 0, 0);
	for (ic = 0; ic < 3; ic++)
		m_select_point[ic]->setChecked(false);
	if (0 < ch) {
		m_select_point[ch - 1]->setChecked(true);
		type = 22 + ch;
		m_point_n = ch;
	}
	else
		m_point_n = 0;
	connect(m_select_point[0], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point1()));
	connect(m_select_point[1], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point2()));
	connect(m_select_point[2], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point3()));
	picture_change(type);
}

void picture_property_widget::slot_select_point1()
{
	//select_point(m_select_point[0]->isChecked() ? 1 : -1);
}

void picture_property_widget::slot_select_point2()
{
	//select_point(m_select_point[1]->isChecked() ? 2 : -2);
}

void picture_property_widget::slot_select_point3()
{
	//select_point(m_select_point[2]->isChecked() ? 3 : -3);
}
