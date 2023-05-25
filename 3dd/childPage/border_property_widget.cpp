#include "border_property_widget.h"

border_property_widget::border_property_widget(int num,G_BoundingBox *bound)
{
	 baseStruct= new G_BoundingBox();
	QString label = (bound) ? QString::fromStdString(bound->m_name) : ( (num == 0)? STRING_LABEL_BORDER : STRING_LABEL_BORDER + QString::number(num + 1));
	 create_2buttons(this, label);

	//常规属性
	m_Normal_TreeWidget = new QTreeWidget;
	m_Normal_TreeWidget->setColumnCount(2);
	m_Normal_TreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_Normal_TreeWidget->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_pre_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));

	m_normal_property_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_BOUNDBOX));
	m_left_down_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_LEFTDOWN));
	m_left_down_treeitem->setText(1, "");
	m_right_upon_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_RIGHTUPON));
	m_right_upon_treeitem->setText(1, "");
	m_render_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_RENDER));
	m_line_width_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_LINEWIDTH));
	m_LineWidth_spin = new QSpinBox;
	m_LineWidth_spin->setRange(0, 4);
	m_LineWidth_spin->setValue(1);
	m_LineWidth_slider = new QSlider(Qt::Horizontal);
	m_LineWidth_slider->setRange(0, 4);
	m_LineWidth_slider->setValue(1);
	QWidget *linewidth_widget = new QWidget;
	QHBoxLayout *linewidth_layout = new QHBoxLayout;
	linewidth_layout->addWidget(m_LineWidth_spin);
	linewidth_layout->addWidget(m_LineWidth_slider);
	linewidth_widget->setLayout(linewidth_layout);
	m_Normal_TreeWidget->setItemWidget(m_line_width_treeitem,1,linewidth_widget);
	m_normal_color_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(STRING_ITEM_COLOR));
	m_Color = new QComboBox;
	color_choose(m_Color);
	m_Normal_TreeWidget->setItemWidget(m_normal_color_treeitem, 1, m_Color);
	m_Normal_TreeWidget->expandAll();

	//标注
	m_Notation_TreeWidget = new QTreeWidget;
	m_Notation_TreeWidget->setColumnCount(2);
	m_Notation_TreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_Notation_TreeWidget->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_note_treeitem = new QTreeWidgetItem(m_Notation_TreeWidget, QStringList(STRING_ITEM_NOTATE));
	m_show_note_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_SHOWNOTATE));
	m_ShowNotation = new QCheckBox;
	m_Notation_TreeWidget->setItemWidget(m_show_note_treeitem, 1, m_ShowNotation);
	m_font_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_FONT));
	//m_Font = new QComboBox;
	//m_Notation_TreeWidget->setItemWidget(m_font_treeitem, 1, m_Font);
	m_font_size_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_FONTSIZE));
	m_FontSize_spin = new QSpinBox;
	m_FontSize_spin->setRange(0, 100);
	m_FontSize_spin->setValue(12);
	m_FontSize_slider = new QSlider(Qt::Horizontal);
	m_FontSize_slider->setRange(0, 100);
	m_FontSize_slider->setValue(12);
	QWidget *widget_fontsize = new QWidget;
	QHBoxLayout *layout_fontsize = new QHBoxLayout;
	layout_fontsize->addWidget(m_FontSize_spin);
	layout_fontsize->addWidget(m_FontSize_slider);
	widget_fontsize->setLayout(layout_fontsize);
	m_Notation_TreeWidget->setItemWidget(m_font_size_treeitem, 1, widget_fontsize);
	//m_note_color_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(tr("color")));
	m_Notate_color = new QComboBox;
	color_choose(m_Notate_color);
	//m_Notation_TreeWidget->setItemWidget(m_note_color_treeitem, 1, m_Notate_color);
	m_note_format_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(STRING_ITEM_NOTATEFORMAT));
#if 0
	m_note_type_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(STRING_ITEM_TYPE));
	m_Type = new QComboBox;
	m_Type->insertItem(0, STRING_COMBOX_FIXVALUE);
	m_Type->insertItem(1, STRING_COMBOX_EXPONFOR);
	m_Type->insertItem(2, STRING_COMBOX_NORMALPRO);
	m_Notation_TreeWidget->setItemWidget(m_note_type_treeitem,1,m_Type);
#endif
	m_note_significant_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(STRING_ITEM_SIGNNUM));
	m_significant_spin = new QSpinBox;
	m_significant_spin->setValue(2);
	m_Notation_TreeWidget->setItemWidget(m_note_significant_treeitem, 1, m_significant_spin);
	m_Notation_TreeWidget->expandAll();


	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_Normal_TreeWidget, QString(STRING_TAB_GENERAL));
	m_form1_tab->addTab(m_Notation_TreeWidget, QString(STRING_TAB_NOTATE));
	if (bound)
	{
		m_input_treeitem->setText(1, QString::fromStdString(bound->m_putIn));
		QString left_down = QString("");
		left_down += QString("(");
		left_down += QString("% 1").arg(bound->m_leftBottom.m_x);
		left_down += QString(",");
		left_down += QString("% 1").arg(bound->m_leftBottom.m_y);
		left_down += QString(",");
		left_down += QString("% 1").arg(bound->m_leftBottom.m_z);
		left_down += QString(")");
		m_left_down_treeitem->setText(1, left_down);
		QString right_upon = QString("");
		right_upon += QString("(");
		right_upon+= QString("% 1").arg(bound->m_rightTop.m_x);
		right_upon += QString(",");
		right_upon += QString("% 1").arg(bound->m_rightTop.m_y);
		right_upon += QString(",");
		right_upon += QString("% 1").arg(bound->m_rightTop.m_z);
		right_upon += QString(")");
		m_right_upon_treeitem->setText(1, right_upon);
		m_LineWidth_spin->setValue(bound->m_render.m_lineWidth);
		m_LineWidth_slider->setValue(bound->m_render.m_lineWidth);
		m_Color->setCurrentText(crgb2name(bound->m_render.m_rgb));
		m_ShowNotation->setChecked(bound->m_labelMark.m_customOn);
		//m_Font->setCurrentIndex(bound->m_labelMark)
		m_FontSize_spin->setValue(bound->m_labelMark.m_fontSize);
		m_FontSize_slider->setValue(bound->m_labelMark.m_fontSize);
		//m_Notate_color->setCurrentIndex(bound->m_labelMark.m_);
		//m_note_format_treeitem->setText(1,bound->m_labelMark.m_)
		//m_Type->setCurrentIndex(bound->m_labelMark.m_type);
		m_significant_spin->setValue(bound->m_labelMark.m_decimalDigits);
	}
	connect(m_LineWidth_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_slider_setValue(int)));
	connect(m_LineWidth_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_spin_setValue(int)));
	connect(m_Color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_change_border_color()));
	connect(m_ShowNotation, SIGNAL(stateChanged(int)), this, SLOT(slot_show_note()));
	//connect(m_Font, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_note_font()));
	connect(m_FontSize_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_font_slider_setvalue(int)));
	connect(m_FontSize_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_font_spin_setvalue(int)));
	//connect(m_Notate_color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_note_color_change()));
	//connect(m_Type, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_note_type_change()));
	connect(m_significant_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_note_significant_change()));
	this->NodeType = STRING_WIDGET_BORDER;
	m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	//m_return_border_property_struct = new G_BoundingBox();
}

border_property_widget::~border_property_widget()
{
	delete m_form1_tab;
}

void border_property_widget::paintEvent(QPaintEvent *e)
{
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

void border_property_widget::contextMenuEvent(QContextMenuEvent  *event){
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
	/*
	QMenu  *out_put = menu->addMenu(STRING_MENU_GRAPHIC);
	out_put->addAction(STRING_ACTION_AXIS, this, SLOT(slot_border_to_axis()));
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
	menu->addAction(tr("connect output module"), this, SLOT(slot_connect_output()));*/
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));

//	menu->addAction(new QAction(tr("save data"), this));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_border()));	// this,SLOT(slot_delete_self()));	// yzw 0610_widget
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->move(cursor().pos());
	menu->show();
}

void border_property_widget::delete_self()
{
	emit sig_delete_border();
}

G_BoundingBox* border_property_widget::return_border_form_property_struct()
{
	G_BoundingBox* m_return_border_property_struct=(G_BoundingBox*)baseStruct;
	getBaseStruct();
	m_return_border_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	QString str = m_left_down_treeitem->text(1);
	m_return_border_property_struct->m_labelMark.m_customOn = m_ShowNotation->checkState();
	if (str == "")
	{
		m_return_border_property_struct->m_leftBottom.m_x = 0;
		m_return_border_property_struct->m_leftBottom.m_y = 0;
		m_return_border_property_struct->m_leftBottom.m_z = 0;
	}
	else 
	{
		str = str.mid(1, str.length() - 1);
		QStringList list = str.split(",");
		m_return_border_property_struct->m_leftBottom.m_x = list[0].toFloat();
		m_return_border_property_struct->m_leftBottom.m_y = list[1].toFloat();
		m_return_border_property_struct->m_leftBottom.m_z = list[2].toFloat();
	}
	QString str1 = m_right_upon_treeitem->text(1);
	if(str1=="")
	{ 
		m_return_border_property_struct->m_rightTop.m_x = 0;
		m_return_border_property_struct->m_rightTop.m_y = 0;
		m_return_border_property_struct->m_rightTop.m_z = 0;
	}
	else
	{
		str1 = str1.mid(1, str1.length() - 1);
		QStringList list1 = str1.split(",");
		m_return_border_property_struct->m_rightTop.m_x = list1[0].toFloat();
		m_return_border_property_struct->m_rightTop.m_y = list1[1].toFloat();
		m_return_border_property_struct->m_rightTop.m_z = list1[2].toFloat();
	}
	m_return_border_property_struct->m_render.m_lineWidth = m_LineWidth_spin->value();
	m_return_border_property_struct->m_render.m_rgb= name2crgb(m_Color->currentText());
	m_return_border_property_struct->m_labelMark.m_fontSize = m_FontSize_spin->value();
	//m_return_border_property_struct->m_labelMark.m_type = m_Type->currentIndex();
	m_return_border_property_struct->m_labelMark.m_decimalDigits = m_significant_spin->value();
	m_return_border_property_struct->widget = this;
	return m_return_border_property_struct;
}

void border_property_widget::update_peak_from_node(PosXYZ & left_down, PosXYZ & right_upon)
{
	if (this == nullptr)
		return;
	int  digits = m_significant_spin->value();
	QString str_left_down = //QString("(%1, %2, %3)").arg(left_down.m_x, 0, 'g', 2).
		//arg(left_down.m_y, 0, 'g', 2).arg(left_down.m_z, 0, 'g', 2);
		QString::number(left_down.m_x, 'f', digits) + ", " +
		QString::number(left_down.m_y, 'f', digits) + ", " +
		QString::number(left_down.m_z, 'f', digits);
	m_left_down_treeitem->setText(1, str_left_down);
	QString str_right_upon = //QString("(%1, %2, %3)").arg(right_upon.m_x, 0, 'g', 2).
		//arg(right_upon.m_y, 0, 'g', 2).arg(right_upon.m_z, 0, 'g', 2);
		QString::number(right_upon.m_x, 'f', digits) + ", " +
		QString::number(right_upon.m_y, 'f', digits) + ", " +
		QString::number(right_upon.m_z, 'f', digits);
	m_right_upon_treeitem->setText(1, str_right_upon);
}

void border_property_widget::m_update_widget_property()
{
	G_BoundingBox* pTemp = this->return_border_form_property_struct();
	std::shared_ptr<G_BoundingBox>ptr(std::make_shared<G_BoundingBox>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	m_input_treeitem->setText(1, QString::fromStdString(pTemp->m_putIn));
	emit sig_vtk_camera_render();
}

void border_property_widget::slot_m_linewidth_slider_setValue(int a)
{
	disconnect(m_LineWidth_slider, 0, 0, 0);
	m_LineWidth_slider->setValue(a);
	m_update_widget_property();
	connect(m_LineWidth_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_spin_setValue(int)));
}

void border_property_widget::slot_m_linewidth_spin_setValue(int a)
{
	disconnect(m_LineWidth_spin, 0, 0, 0);
	m_LineWidth_spin->setValue(a);
	m_update_widget_property();
	connect(m_LineWidth_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_slider_setValue(int)));
}

void border_property_widget::slot_change_border_color()
{
	m_update_widget_property();
}

void border_property_widget::slot_show_note()
{
	m_update_widget_property();
}

void border_property_widget::slot_note_font()
{
	m_update_widget_property();
}

void border_property_widget::slot_font_slider_setvalue(int a)
{
	disconnect(m_FontSize_slider, 0, 0, 0);
	m_FontSize_slider->setValue(a);
	m_update_widget_property();
	connect(m_FontSize_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_font_spin_setvalue(int)));
}

void border_property_widget::slot_font_spin_setvalue(int a)
{
	disconnect(m_FontSize_spin, 0, 0, 0);
	m_FontSize_spin->setValue(a);
	m_update_widget_property();
	connect(m_FontSize_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_font_slider_setvalue(int)));
}

void border_property_widget::slot_note_color_change()
{
	m_update_widget_property();
}

void border_property_widget::slot_note_type_change()
{
	m_update_widget_property();
}

void border_property_widget::slot_note_significant_change()
{
	m_update_widget_property();
}

void border_property_widget::slot_border_to_axis()
{
	emit sig_border_to_axis();
}
