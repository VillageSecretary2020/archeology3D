#include "text_property_widget.h"
#include "myqwidget.h"
#include <QDateTime>
text_property_widget::text_property_widget(int num,G_Text* text)
{
	m_return_text_property = new G_Text;
	baseStruct = m_return_text_property;
	QString label = (text) ? QString::fromStdString(text->m_name) : ( (num == 0)? STRING_LABEL_TEXT : STRING_LABEL_TEXT + QString::number(num + 1));
	 create_rbuttons(this, label);
	//this->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black;}");
	//常规属性
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_normal_property->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black;}");
	m_text = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_TEXT));
	m_text_treeitem = new QTreeWidgetItem(m_text, QStringList(STRING_ITEM_TEXT));
	m_font_treeitem = new QTreeWidgetItem(m_text, QStringList(STRING_ITEM_FONT));
	m_text_edit = new QLineEdit;
	QDateTime datetime(QDateTime::currentDateTime());
	QString qstr = datetime.toString("yyyy-MM-dd");
	m_text_edit->setText(qstr);
	m_normal_property->setItemWidget(m_text_treeitem, 1, m_text_edit);
	m_font = new QComboBox;
	m_font->insertItem(0, STRING_ITEM_FONT_DEFAULT);
	//m_font->insertItem(0, "verdana");
	//m_font->insertItem(1, "vijaya");
	//m_font->insertItem(2, "vrinda");
	//m_font->insertItem(3, "webdings");
	m_normal_property->setItemWidget(m_font_treeitem, 1, m_font);
	m_font_size_treeitem = new QTreeWidgetItem(m_text, QStringList(STRING_ITEM_FONTSIZE));
	m_font_size_spinbox = new QSpinBox;
	m_font_size_spinbox->setRange(4, 72);
	m_font_size_spinbox->setValue(14);
	m_font_size_slider = new QSlider(Qt::Horizontal);
	m_font_size_slider->setRange(4, 72);
	m_font_size_slider->setValue(14);
	QWidget *font_size_widget = new QWidget;
	QHBoxLayout *font_size_layout = new QHBoxLayout;
	font_size_layout->addWidget(m_font_size_spinbox);
	font_size_layout->addWidget(m_font_size_slider);
	font_size_widget->setLayout(font_size_layout);
	m_normal_property->setItemWidget(m_font_size_treeitem,1,font_size_widget);
	m_color_treeitem = new QTreeWidgetItem(m_text, QStringList(STRING_ITEM_COLOR));
	m_color = new QComboBox;
	color_choose(m_color);
	m_normal_property->setItemWidget(m_color_treeitem, 1, m_color);
	m_normal_property->insertTopLevelItem(0,m_text);
	m_position_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_POSITION));
	m_x_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(tr("X")));
	m_x_position_spin = new QDoubleSpinBox;
	m_normal_property->setItemWidget(m_x_treeitem, 1, m_x_position_spin);
	m_x_position_spin->setValue(0);
	m_x_position_spin->setDecimals(2);
	m_x_position_spin->setSingleStep(0.1);
	m_y_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(tr("Y")));
	m_y_position_spin = new QDoubleSpinBox;
	m_normal_property->setItemWidget(m_y_treeitem, 1, m_y_position_spin);
	m_y_position_spin->setValue(0);
	m_y_position_spin->setDecimals(2);
	m_y_position_spin->setSingleStep(0.1);
	m_z_treeitem = new QTreeWidgetItem(m_position_treeitem, QStringList(tr("Z")));
	m_z_position_spin = new QDoubleSpinBox;
	m_normal_property->setItemWidget(m_z_treeitem, 1, m_z_position_spin);
	m_z_position_spin->setValue(0);
	m_z_position_spin->setDecimals(2);
	m_z_position_spin->setSingleStep(0.1);
	m_normal_property->insertTopLevelItem(1,m_position_treeitem);
	m_normal_property->expandAll();

	if (text)
	{
		m_text_edit->setText(QString::fromStdString(text->m_text));
		m_font->setCurrentIndex(text->m_fontType);
		m_font_size_spinbox->setValue(text->m_fontSize);
		m_font_size_slider->setValue(text->m_fontSize);
		m_color->setCurrentText(crgb2name(text->m_rgb));
		m_x_position_spin->setValue(text->m_pos.m_x);
		m_y_position_spin->setValue(text->m_pos.m_y);
		m_z_position_spin->setValue(text->m_pos.m_z);
	}

	//m_return_text_property = new G_Text;
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	this->NodeType = STRING_WIDGET_TEXT;
	connect(m_text_edit, SIGNAL(editingFinished()), this, SLOT(slot_text_edit()));
	connect(m_font, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_font()));
	connect(m_font_size_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_font_size_slider(int)));
	connect(m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_font_size_spin(int)));
	connect(m_color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_text_color()));
	connect(m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_x_position_spin()));
	connect(m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_x_position_spin()));
	connect(m_z_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_x_position_spin()));
}

text_property_widget::~text_property_widget()
{
	delete m_form1_tab;
}

void text_property_widget::paintEvent(QPaintEvent *e)
{
#if 0
	QPainter p(this);
	//p.setCompositionMode(QPainter::CompositionMode_Clear);
	//p.fillRect(0, 0, 10, 40, Qt::SolidPattern);
	//p.fillRect(0, 0, 10, 30, Qt::gray);
	p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);
#endif 
	
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 0) << QPoint(this->width()-10, 0) << QPoint(this->width()-10, 5) << QPoint(this->width(), 15);
	polygon << QPoint(this->width() - 10, 25) << QPoint(this->width()-10, 30) << QPoint(0, 30);
	setMask(polygon);
}

void text_property_widget::contextMenuEvent(QContextMenuEvent  *event)
{
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *menu = new QMenu(this);
#if 0
	QMenu *out_put = menu->addMenu(tr("graphic output"));
	out_put->addAction(tr("axis"), this, SLOT(slot_text_to_axis()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_text_to_border()));
	//QMenu *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output geometry(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output geometry"), this));
	menu->addAction(tr("connect output module"), this, SLOT(slot_create_output()));
	menu->addAction(new QAction(tr("save data"), this));
#endif
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_DELETE, this,SLOT(slot_delete_self_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_text()));
	menu->move(cursor().pos());
	menu->show();
}

void text_property_widget::delete_self()
{
	emit sig_delete_text();
}

G_Text* text_property_widget::return_text_property_struct()
{
	getBaseStruct();
	m_return_text_property->m_text = m_text_edit->text().toStdString();
	//m_return_text_property->m_text = "hello-HHH:: HHH";
	m_return_text_property->m_fontType = m_font->currentIndex();
	m_return_text_property->m_fontSize = m_font_size_spinbox->value();

	m_return_text_property->m_rgb = 	name2crgb(m_color->currentText());
	m_return_text_property->m_pos.m_x = m_x_position_spin->value();
	m_return_text_property->m_pos.m_y = m_y_position_spin->value();
	m_return_text_property->m_pos.m_z = m_z_position_spin->value();
	return m_return_text_property;
}

void text_property_widget::m_update_widget_property()
{
	G_Text* pTemp = this->return_text_property_struct();
	std::shared_ptr<G_Text>ptr(std::make_shared<G_Text>());
	*(ptr.get()) = *pTemp;

	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_new_project_update();
}

void text_property_widget::slot_text_to_axis()
{
	emit sig_text_to_axis();
}
void text_property_widget::slot_text_to_border()
{
	emit sig_text_to_border();
}

void text_property_widget::slot_text_edit()
{
	m_update_widget_property();
}

void text_property_widget::slot_font()
{
	m_update_widget_property();
}

void text_property_widget::slot_font_size_slider(int a)
{
	disconnect(m_font_size_spinbox, 0, 0, 0);
	m_font_size_spinbox->setValue(a);
	m_update_widget_property();
	connect(m_font_size_spinbox, SIGNAL(valueChanged(int)), this, SLOT(slot_font_size_spin(int)));
}

void text_property_widget::slot_font_size_spin(int a)
{
	disconnect(m_font_size_slider, 0, 0, 0);
	m_font_size_slider->setValue(a);
	m_update_widget_property();
	connect(m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_font_size_slider(int)));
}

void text_property_widget::slot_text_color()
{
	m_update_widget_property();
}

void text_property_widget::slot_x_position_spin()
{
	QString text = "position";
	QString temp = "(";
	temp += QString("%1").arg(m_x_position_spin->value());
	temp += ",";
	temp += QString("%1").arg(m_y_position_spin->value());
	temp += ",";
	temp += QString("%1").arg(m_z_position_spin->value());
	temp += ")";
	text += temp;
	m_position_treeitem->setText(0, text);
	m_update_widget_property();
}
