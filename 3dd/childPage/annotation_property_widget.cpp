#include "annotation_property_widget.h"
#include "myqwidget.h"

annotation_property_widget::annotation_property_widget(int num,G_Comment* annotation)
{
	QString label = (annotation) ? QString::fromStdString(annotation->m_name) : ( (num == 0)? STRING_LABEL_ANNOTATION : STRING_LABEL_ANNOTATION + QString::number(num + 1));
	 create_rbuttons(this, label);

	m_NormalProperty = new QTreeWidget;
	m_NormalProperty->setColumnCount(2);
	m_NormalProperty->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	m_annotation_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString(STRING_ITEM_ANNOTATION)));
	m_text_treeitem = new QTreeWidgetItem(m_annotation_treeitem, QStringList(QString(STRING_ITEM_TEXT)));
	m_Text = new QLineEdit;
	m_Text->setText(STRING_ITEM_ANNO_SAMPLE);
	QWidget *m_text_widget = new QWidget;
	QHBoxLayout *m_text_layout = new QHBoxLayout;
	m_text_layout->addWidget(m_Text);
	m_text_widget->setLayout(m_text_layout);
	m_NormalProperty->setItemWidget(m_text_treeitem, 1, m_text_widget);
	m_font_treeitem = new QTreeWidgetItem(m_annotation_treeitem, QStringList(QString(STRING_ITEM_FONT)));
	m_Font = new QComboBox;		// QFontComboBox; // 
	m_Font->insertItem(0, STRING_ITEM_FONT_DEFAULT);
	//m_Font->insertItem(0, "verdana");
	//m_Font->insertItem(1, "vijaya");
	//m_Font->insertItem(2, "vrinda");
	//m_Font->insertItem(3, "Webdings");
	m_NormalProperty->setItemWidget(m_font_treeitem, 1, m_Font);
	m_font_size_treeitem = new QTreeWidgetItem(m_annotation_treeitem, QStringList(QString(STRING_ITEM_FONTSIZE)));
	m_FontSize_spin = new QSpinBox;
	m_FontSize_spin->setValue(18);
	m_FontSize_spin->setRange(4, 72);
	m_FontSize_slider = new QSlider(Qt::Horizontal);
	m_FontSize_slider->setRange(4, 72);
	m_FontSize_slider->setValue(18);
	QWidget* fontsize_widget = new QWidget;
	QHBoxLayout* fontsize_layout = new QHBoxLayout;
	fontsize_layout->addWidget(m_FontSize_spin);
	fontsize_layout->addWidget(m_FontSize_slider);
	fontsize_widget->setLayout(fontsize_layout);
	m_NormalProperty->setItemWidget(m_font_size_treeitem, 1, fontsize_widget);
	m_text_align_treeitem = new QTreeWidgetItem(m_annotation_treeitem, QStringList(QString(STRING_ITEM_TEXTALIGN)));
	m_TextAlign = new QComboBox;
	m_TextAlign->insertItem(0, STRING_COMBOX_LEFT);
	m_TextAlign->insertItem(1, STRING_COMBOX_RIGHT);
	m_TextAlign->insertItem(2, STRING_COMBOX_CENTER);
	m_TextAlign->setCurrentIndex(0);
	m_NormalProperty->setItemWidget(m_text_align_treeitem, 1, m_TextAlign);
	m_color_treeitem = new QTreeWidgetItem(m_annotation_treeitem, QStringList(QString(STRING_ITEM_COLOR)));
	m_Color = new QComboBox;
	color_choose(m_Color);
	m_NormalProperty->setItemWidget(m_color_treeitem, 1, m_Color);
	m_position_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString(STRING_ITEM_LOCATION)));
	m_origin_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(QString(STRING_ITEM_ORIGIN)));
	m_Origin = new QComboBox;
	m_Origin->insertItem(0, STRING_COMBOX_LEFTUPON);
	m_Origin->insertItem(1, STRING_COMBOX_RIGHTUPON);
	m_Origin->insertItem(2, STRING_COMBOX_LEFTDOWN);
	m_Origin->insertItem(3, STRING_COMBOX_RIGHTDOWN);
	m_NormalProperty->setItemWidget(m_origin_treeitem, 1, m_Origin);
	m_x_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(QString("x")));
	m_x_spin = new QDoubleSpinBox;
	m_x_spin->setDecimals(0);
	m_x_spin->setValue(1000);
	m_x_spin->setSingleStep(10);
	m_x_spin->setRange(0, 3000);
	m_x_slider = new QSlider(Qt::Horizontal);
	m_x_slider->setRange(0, 3000);
	m_x_slider->setValue(100);
	QWidget* x_widget = new QWidget;
	QHBoxLayout* x_layout = new QHBoxLayout;
	x_layout->addWidget(m_x_spin);
	x_layout->addWidget(m_x_slider);
	x_widget->setLayout(x_layout);
	m_NormalProperty->setItemWidget(m_x_treeitem, 1, x_widget);
	m_y_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(QString("y")));
	m_y_spin = new QDoubleSpinBox;
	m_y_spin->setDecimals(0);
	m_y_spin->setValue(0);
	m_y_spin->setSingleStep(10);
	m_y_spin->setRange(0, 3000);
	m_y_slider = new QSlider(Qt::Horizontal);
	m_y_slider->setRange(0, 3000);
	m_y_slider->setValue(100);
	QWidget* y_widget = new QWidget;
	QHBoxLayout* y_layout = new QHBoxLayout;
	y_layout->addWidget(m_y_spin);
	y_layout->addWidget(m_y_slider);
	y_widget->setLayout(y_layout);
	m_NormalProperty->setItemWidget(m_y_treeitem, 1, y_widget);
	m_NormalProperty->insertTopLevelItem(0, m_annotation_treeitem);
	m_NormalProperty->insertTopLevelItem(1, m_position_treeitem);
	m_NormalProperty->expandAll();

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_NormalProperty, QString(STRING_TAB_GENERAL));
	//init struct
	m_return_annotation_property_struct = new G_Comment;
	baseStruct = m_return_annotation_property_struct;
	if (!annotation)
	{
		GCommentNode::getDefault(baseStruct);
		annotation = (G_Comment*)baseStruct;
	}
	else {
		*m_return_annotation_property_struct = *annotation;
		annotation = m_return_annotation_property_struct;
	}
	{
		m_Text->setText( QString::fromStdString(annotation->m_text));
		m_Font->setCurrentIndex(annotation->m_fontType);
		m_FontSize_spin->setValue(annotation->m_fontSize);
		m_FontSize_slider->setValue(annotation->m_fontSize);
		if (annotation->m_align == CENTER)
		{
			m_TextAlign->setCurrentIndex(0);
		}
		else if (annotation->m_align == RIGHT)
		{
			m_TextAlign->setCurrentIndex(1);
		}
		else
			m_TextAlign->setCurrentIndex(2);
		//annotation->m_rgb
		m_x_spin->setValue(annotation->m_XPos);
		m_x_slider->setValue(annotation->m_XPos);
		m_y_spin->setValue(annotation->m_YPos);
		m_y_slider->setValue(annotation->m_YPos );
	}
	this->NodeType = STRING_WIDGET_ANNOTATION;
	connect(m_Text, SIGNAL(editingFinished()), this, SLOT(slot_input_text()));
	connect(m_Font, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_change_font()));
	connect(m_FontSize_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_fontsize_slider(int)));
	connect(m_FontSize_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_fontsize_spin(int)));
	connect(m_TextAlign, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_text_align()));
	connect(m_Color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_choose()));
	connect(m_x_spin, SIGNAL(valueChanged(double)), this, SLOT(m_x_slider_setValue(double)));
	connect(m_x_slider, SIGNAL(valueChanged(int)), this, SLOT(m_x_spin_setValue(int)));
	connect(m_y_spin, SIGNAL(valueChanged(double)), this, SLOT(m_y_slider_setValue(double)));
	connect(m_y_slider, SIGNAL(valueChanged(int)), this, SLOT(m_y_spin_setValue(int)));
	connect(m_Origin, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_origin_align()));
}

annotation_property_widget::~annotation_property_widget()
{
	delete m_form1_tab;
}

void annotation_property_widget::paintEvent(QPaintEvent *e)
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
	p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);
#endif 

	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 0) << QPoint(this->width() - 10, 0) << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15);
	polygon << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30) << QPoint(0, 30);
	setMask(polygon);
}

void annotation_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(tr("graphic output"));
	out_put->addAction(tr("axis"), this, SLOT(slot_note_to_axis()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_note_to_border()));
	QMenu  *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	menu->addAction(tr("connect output module"), this, SLOT(slot_create_output()));
	menu->addAction(new QAction(tr("save data"), this));
#endif
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_annotation()));
	menu->move(cursor().pos());
	menu->show();
}

void annotation_property_widget::delete_self()
{
	emit sig_delete_annotation();
}

void annotation_property_widget::m_update_widget_property()
{
	G_Comment* pTemp = this->return_annotation_property_struct();
	std::shared_ptr<G_Comment>ptr(std::make_shared<G_Comment>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void annotation_property_widget::slot_input_text()
{
	m_return_annotation_property_struct->m_changetype = 1;
	m_update_widget_property();
}
void annotation_property_widget::m_x_slider_setValue(double a)
{
	disconnect(m_x_slider, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 6;
	int value = a;// (int)(a * 10000);
	m_x_slider->setValue(value);
	m_update_widget_property();
	connect(m_x_slider, SIGNAL(valueChanged(int)), this, SLOT(m_x_spin_setValue(int)));
}

void annotation_property_widget::m_x_spin_setValue(int a)
{
	disconnect(m_x_spin, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 6;
	double value = a; // 10000.0;
	m_x_spin->setValue(value);
	m_update_widget_property();
	connect(m_x_spin, SIGNAL(valueChanged(double)), this, SLOT(m_x_slider_setValue(double)));
}

void annotation_property_widget::m_y_slider_setValue(double a)
{
	disconnect(m_y_slider, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 6;
	int value = a;// (int)(a * 10000);
	m_y_slider->setValue(value);
	m_update_widget_property();
	connect(m_y_slider, SIGNAL(valueChanged(int)), this, SLOT(m_y_spin_setValue(int)));
}

void annotation_property_widget::m_y_spin_setValue(int a)
{
	disconnect(m_y_spin, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 6;
	double value = a; // 10000.0;
	m_y_spin->setValue(value);
	m_update_widget_property();
	connect(m_y_spin, SIGNAL(valueChanged(double)), this, SLOT(m_y_slider_setValue(double)));
}

void annotation_property_widget::slot_color_choose()
{
	m_return_annotation_property_struct->m_changetype = 5;
	m_update_widget_property();
}

void annotation_property_widget::slot_fontsize_slider(int a)
{
	disconnect(m_FontSize_slider, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 3;
	m_FontSize_slider->setValue(m_FontSize_spin->value());
	m_update_widget_property();
	connect(m_FontSize_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_fontsize_spin(int)));
}

void annotation_property_widget::slot_change_font()
{
	m_return_annotation_property_struct->m_changetype = 2;
	m_update_widget_property();
}

void annotation_property_widget::slot_fontsize_spin(int a)
{
	disconnect(m_FontSize_spin, 0, 0, 0);
	m_return_annotation_property_struct->m_changetype = 3;
	m_FontSize_spin->setValue(m_FontSize_slider->value());
	m_update_widget_property();
	connect(m_FontSize_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_fontsize_slider(int)));
}

void annotation_property_widget::slot_text_align()
{
	m_return_annotation_property_struct->m_changetype = 4;
	m_update_widget_property();
}

void annotation_property_widget::slot_origin_align()
{
	int x, y;
	int index = m_Origin->currentIndex();
	if (0 == index) {
		x = 100; y = 800;
	}
	else if (1 == index) {
		x = 800; y = 800;
	}
	else if (2 == index) {
		x = 100; y = 100;
	}
	else
	{
		x = 800; y = 100;
	}
	m_x_spin_setValue(x);	m_x_slider_setValue(x);
	m_y_spin_setValue(y); m_y_slider_setValue(y);
	m_update_widget_property();
}

G_Comment* annotation_property_widget::return_annotation_property_struct()
{
	getBaseStruct();
	m_return_annotation_property_struct->m_text = m_Text->text().toStdString();
	m_return_annotation_property_struct->m_fontType = 1;//m_Font->currentIndex();
	m_return_annotation_property_struct->m_fontSize = m_FontSize_spin->value();
	//m_TextAlign
	//CENTER,
		//RIGHT,
		//LEFT
	int text_align_index = m_TextAlign->currentIndex();
	if (text_align_index == 0)
		m_return_annotation_property_struct->m_align = LEFT;
	else if (text_align_index == 1)
		m_return_annotation_property_struct->m_align = RIGHT;
	else
		m_return_annotation_property_struct->m_align = CENTER;
	m_return_annotation_property_struct->m_rgb = name2crgb(m_Color->currentText());
	//m_Origin   无原点属性字段
	m_return_annotation_property_struct->m_XPos = m_x_spin->value();
	m_return_annotation_property_struct->m_YPos = m_y_spin->value();
	return m_return_annotation_property_struct;
}

void annotation_property_widget::slot_note_to_axis()
{
	emit sig_note_to_axis();
}

void  annotation_property_widget::slot_note_to_border()
{
	emit sig_note_to_border();
}
