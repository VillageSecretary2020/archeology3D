// color_scale_property.cpp

#include "color_scale_property.h"
#include <QHBoxLayout>

#define	SLIDER_MULTIPLIER	1000

color_scale_property::color_scale_property()
{
	this->setColumnCount(2);
	//this->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_color_scale_treeitem = new QTreeWidgetItem(this, QStringList(QString(STRING_ITEM_COLOR_SCALE)));
	m_show_color_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_SHOWCOLORSCA)));
	m_color_place_direction_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_PLACEDIRTION)));
	m_x_position_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_XPOSITION)));
	m_y_position_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_YPOSITION)));
	m_width_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_WIDTH)));
	m_length_treeitem = new QTreeWidgetItem(m_color_scale_treeitem, QStringList(QString(STRING_ITEM_LENGTH)));
	m_title = new QTreeWidgetItem(this, QStringList(QString(STRING_ITEM_TITLE)));
	m_title_treeitem = new QTreeWidgetItem(m_title, QStringList(QString(STRING_ITEM_TITLE)));
	//m_title_treeitem->setText(1, "hello");
	m_title_edit = new QLineEdit();
	m_title_edit->setText(u8"色阶");
	setItemWidget(m_title_treeitem, 1, m_title_edit);
	m_font_size_treeitem = new QTreeWidgetItem(m_title, QStringList(QString(STRING_ITEM_FONTSIZE)));
	m_note_treeitem = new QTreeWidgetItem(this, QStringList(QString(STRING_ITEM_NOTE)));
	m_note_number_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(QString(STRING_ITEM_NOTENUMBER)));
	m_note_font_size_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(QString(STRING_ITEM_FONTSIZE)));
//	m_adopt_self_note_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(QString("adopt self note")));
	//m_self_note_treeitem = new QTreeWidgetItem(m_note_treeitem, QStringList(QString("self note")));
	m_note_format_treeitem = m_note_treeitem;// new QTreeWidgetItem(m_note_treeitem, QStringList(QString(STRING_ITEM_NOTEFORMAT)));
	//m_note_format_treeitem->setText(1, "d.dddddd");
	m_type_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(QString(STRING_ITEM_TYPE)));
	m_significant_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(QString(STRING_ITEM_SIGNNUM)));
	m_prefix_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(QString(STRING_ITEM_PREFIX)));
	m_prefix_edit = new QLineEdit();
	m_prefix_edit->setText("");
	setItemWidget(m_prefix_treeitem, 1, m_prefix_edit);
	m_suffix_treeitem = new QTreeWidgetItem(m_note_format_treeitem, QStringList(QString(STRING_ITEM_SUFFIX)));
	m_suffix_edit = new QLineEdit();
	m_suffix_edit->setText("");
	setItemWidget(m_suffix_treeitem, 1, m_suffix_edit);
	m_show_color_scale = new QCheckBox;
	m_show_color_scale->setChecked(true);
	this->setItemWidget(m_show_color_treeitem, 1, m_show_color_scale);
	m_color_place_direction = new QComboBox;
	m_color_place_direction->insertItem(0, STRING_COMBOX_VERTICAL);
	m_color_place_direction->insertItem(1, STRING_COMBOX_HORIZON);
	m_color_place_direction->setCurrentIndex(1);
	this->setItemWidget(m_color_place_direction_treeitem, 1, m_color_place_direction);
	m_x_position_spin = new QDoubleSpinBox;
	m_x_position_spin->setDecimals(3);
	m_x_position_spin->setValue(0.02);
	m_x_position_spin->setSingleStep(0.01);
	m_x_position_spin->setRange(0.0, 1.0);
	m_x_position_slider = new QSlider(Qt::Horizontal);
	m_x_position_slider->setRange(0, SLIDER_MULTIPLIER);
	m_x_position_slider->setValue(200);
	QWidget *x_position_widget = new QWidget;
	QHBoxLayout *x_position_layout = new QHBoxLayout;
	x_position_layout->addWidget(m_x_position_spin);
	x_position_layout->addWidget(m_x_position_slider);
	x_position_widget->setLayout(x_position_layout);
	this->setItemWidget(m_x_position_treeitem, 1, x_position_widget);
	m_y_position_spin = new QDoubleSpinBox;
	m_y_position_spin->setDecimals(3);
	m_y_position_spin->setValue(0.02);
	m_y_position_spin->setSingleStep(0.01);
	m_y_position_spin->setRange(0, 1);
	m_y_position_slider = new QSlider(Qt::Horizontal);
	m_y_position_slider->setRange(0, SLIDER_MULTIPLIER);
	m_y_position_slider->setValue(399);
	QWidget *y_position_widget = new QWidget;
	QHBoxLayout *y_position_layout = new QHBoxLayout;
	y_position_layout->addWidget(m_y_position_spin);
	y_position_layout->addWidget(m_y_position_slider);
	y_position_widget->setLayout(y_position_layout);
	this->setItemWidget(m_y_position_treeitem, 1, y_position_widget);
	m_width_spin = new QDoubleSpinBox;
	m_width_spin->setDecimals(3);
	m_width_spin->setValue(0.1);
	m_width_spin->setRange(0, 1.0);
	m_width_spin->setSingleStep(0.01);
	m_width_slider = new QSlider(Qt::Horizontal);
	m_width_slider->setRange(0, SLIDER_MULTIPLIER);
	m_width_slider->setValue(100);
	QWidget *width_widget = new QWidget;
	QHBoxLayout *width_layout = new QHBoxLayout;
	width_layout->addWidget(m_width_spin);
	width_layout->addWidget(m_width_slider);
	width_widget->setLayout(width_layout);
	this->setItemWidget(m_width_treeitem, 1, width_widget);
	m_length_spin = new QDoubleSpinBox;
	m_length_spin->setDecimals(3);
	m_length_spin->setRange(0, 1.0);
	m_length_spin->setSingleStep(0.01);
	m_length_spin->setValue(0.6);
	m_length_slider = new QSlider(Qt::Horizontal);
	m_length_slider->setRange(0, SLIDER_MULTIPLIER);
	m_length_slider->setValue(600);
	QWidget *length_widget = new QWidget;
	QHBoxLayout *length_layout = new QHBoxLayout;
	length_layout->addWidget(m_length_spin);
	length_layout->addWidget(m_length_slider);
	length_widget->setLayout(length_layout);
	this->setItemWidget(m_length_treeitem, 1, length_widget);
	m_font_size_spin = new QSpinBox;
	m_font_size_spin->setRange(4, 72);
	m_font_size_spin->setValue(12);
	m_font_size_slider = new QSlider(Qt::Horizontal);
	m_font_size_slider->setRange(4, 72);
	m_font_size_slider->setValue(12);
	QWidget *font_size_widget = new QWidget;
	QHBoxLayout *font_size_layout = new QHBoxLayout;
	font_size_layout->addWidget(m_font_size_spin);
	font_size_layout->addWidget(m_font_size_slider);
	font_size_widget->setLayout(font_size_layout);
	this->setItemWidget(m_font_size_treeitem, 1, font_size_widget);
	m_note_number_spin = new QSpinBox;
	m_note_number_spin->setValue(5);
	this->setItemWidget(m_note_number_treeitem, 1, m_note_number_spin);
	m_note_font_size_spin = new QSpinBox;
	m_note_font_size_spin->setRange(4, 72);
	m_note_font_size_spin->setValue(8);
	m_note_font_size_slider = new QSlider(Qt::Horizontal);
	m_note_font_size_slider->setRange(4, 72);
	m_note_font_size_slider->setValue(8);
	QWidget *note_font_size_widget = new QWidget;
	QHBoxLayout *note_font_size_layout = new QHBoxLayout;
	note_font_size_layout->addWidget(m_note_font_size_spin);
	note_font_size_layout->addWidget(m_note_font_size_slider);
	note_font_size_widget->setLayout(note_font_size_layout);
	this->setItemWidget(m_note_font_size_treeitem, 1, note_font_size_widget);
	m_adopt_self_note = new QCheckBox;
//	this->setItemWidget(m_adopt_self_note_treeitem, 1, m_adopt_self_note);
	m_type = new QComboBox;
	m_type->insertItem(0, STRING_COMBOX_FIXVALUE);
	m_type->insertItem(1, STRING_COMBOX_NORMALPRO);
	m_type->insertItem(2, STRING_COMBOX_EXPONFOR);
	m_type->setCurrentIndex(0);
	this->setItemWidget(m_type_treeitem, 1, m_type);
	m_significant_number_spin = new QSpinBox;
	m_significant_number_spin->setValue(7);
	this->setItemWidget(m_significant_treeitem, 1, m_significant_number_spin);
	this->expandItem(m_color_scale_treeitem);
	expandAll();
}

void color_scale_property::set_data(Child_ColorBar* ccb)
{
	data = ccb;

	m_show_color_scale->setChecked(data->m_displayOn);
	m_color_place_direction->setCurrentIndex(data->m_direction);
	//if (data->m_direction == D_VERTICAL)
	{
	m_width_spin->setValue(data->m_width);
	m_width_slider->setValue((int)data->m_width * SLIDER_MULTIPLIER);
	m_length_spin->setValue(data->m_height);
	m_length_slider->setValue(data->m_height * SLIDER_MULTIPLIER);
	}
	m_x_position_spin->setValue(data->m_xPos);
	m_x_position_slider->setValue((int)data->m_xPos * SLIDER_MULTIPLIER);
	m_y_position_spin->setValue(data->m_yPos);
	m_y_position_slider->setValue((int)data->m_yPos * SLIDER_MULTIPLIER);

	m_title_edit->setText(QString::fromStdString(data->m_title.m_title));
	m_font_size_spin->setValue(data->m_title.m_fontSize);
	m_font_size_slider->setValue(data->m_title.m_fontSize);
	m_note_number_spin->setValue(data->m_label.m_num);
	m_note_font_size_spin->setValue(data->m_label.m_fontSize);
	m_note_font_size_slider->setValue(data->m_label.m_fontSize);
	m_adopt_self_note->setChecked(data->m_label.m_customOn);
	m_type->setCurrentIndex(data->m_label.m_type );
	m_significant_number_spin->setValue(data->m_label.m_decimalDigits);
	m_prefix_edit->setText(QString::fromStdString(data->m_label.m_preText));
	m_suffix_edit->setText(QString::fromStdString(data->m_label.m_lastText));
}

void	color_scale_property::connect_items()
{
	connect(m_show_color_scale, SIGNAL(stateChanged(int)), this, SLOT(slot_show_color_scale(int)));
	connect(m_color_place_direction, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_place_direction(int)));
	connect(m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
	connect(m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
	connect(m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
	connect(m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
	connect(m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
	connect(m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
	connect(m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
	connect(m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));

	connect(m_title_edit, SIGNAL(editingFinished()), this, SLOT(slot_property_changed()));
	connect(m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
	connect(m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
	connect(m_note_number_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_number(int)));
	connect(m_note_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_slider(int)));
	connect(m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
//	connect(m_adopt_self_note, SIGNAL(stateChanged(int)), this, SLOT(slot_m_adopt_self_note(int)));
	connect(m_type, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_color_type(int)));
	connect(m_significant_number_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_significant(int)));
	connect(m_prefix_edit, SIGNAL(editingFinished()), this, SLOT(slot_annotation_changed()));
	connect(m_suffix_edit, SIGNAL(editingFinished()), this, SLOT(slot_annotation_changed()));
}

void color_scale_property::slot_show_color_scale(int a)
{
	data->m_displayOn= m_show_color_scale->isChecked();
	emit color_scale_property_changed(data, 1);
}

void color_scale_property::slot_place_direction(int a)
{
	disconnect(m_width_spin, 0, 0, 0);
	disconnect(m_width_slider, 0, 0, 0);
	disconnect(m_length_spin, 0, 0, 0);
	disconnect(m_length_slider, 0, 0, 0);
	data->m_direction = (ColorBarDirection)m_color_place_direction->currentIndex();
	data->m_width = (float)m_width_spin->value();
	data->m_height = (float)m_length_spin->value();
	if (data->m_direction == D_VERTICAL)
	{
		if (data->m_height < data->m_width) {
			float tmp = data->m_height;
			data->m_height = data->m_width;
			data->m_width = tmp;
		}
	}
	else
	{
		if (data->m_height > data->m_width) {
			float tmp = data->m_height;
			data->m_height = data->m_width;
			data->m_width = tmp;
		}
	}
		m_width_spin->setValue(data->m_width);
		m_width_slider->setValue((int)data->m_width * SLIDER_MULTIPLIER);
		m_length_spin->setValue(data->m_height);
		m_length_slider->setValue(data->m_height * SLIDER_MULTIPLIER);

		connect(m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
		connect(m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
		connect(m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
		connect(m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
	slot_property_changed();
}

void color_scale_property::slot_m_x_position_slider(double a)
{
	disconnect(m_x_position_slider, 0, 0, 0);
	int value = (int)(a * SLIDER_MULTIPLIER);
	m_x_position_slider->setValue(value);
	slot_property_changed();
	connect(m_x_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_x_position_spin(int)));
}

void color_scale_property::slot_m_x_position_spin(int a)
{
	disconnect(m_x_position_spin, 0, 0, 0);
	double value = a; value /= SLIDER_MULTIPLIER;
	m_x_position_spin->setValue(value);
	slot_property_changed();
	connect(m_x_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_x_position_slider(double)));
}

void color_scale_property::slot_m_y_position_slider(double a)
{
	disconnect(m_y_position_slider, 0, 0, 0);
	int value = (int)(a * SLIDER_MULTIPLIER);
	m_y_position_slider->setValue(value);
	slot_property_changed();
	connect(m_y_position_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_y_position_spin(int)));
}

void color_scale_property::slot_m_y_position_spin(int a)
{
	disconnect(m_y_position_spin, 0, 0, 0);
	double value = a; value /=  SLIDER_MULTIPLIER;
	m_y_position_spin->setValue(value);
	slot_property_changed();
	connect(m_y_position_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_y_position_slider(double)));
}

void color_scale_property::slot_m_width_slider(double a)
{
	disconnect(m_width_slider, 0, 0, 0);
	int value = (int)(a * 2);
	m_width_slider->setValue(value);
	slot_property_changed();
	connect(m_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_width_spin(int)));
}

void color_scale_property::slot_m_width_spin(int a)
{
	disconnect(m_width_spin, 0, 0, 0);
	double value = a; value /= SLIDER_MULTIPLIER;
	m_width_spin->setValue(value);
	slot_property_changed();
	connect(m_width_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_width_slider(double)));
}

void color_scale_property::slot_m_length_slider(double a)
{
	disconnect(m_length_slider, 0, 0, 0);
	a *= SLIDER_MULTIPLIER;
	m_length_slider->setValue(a);
	slot_property_changed();
	connect(m_length_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_length_spin(int)));
}

void color_scale_property::slot_m_length_spin(int a)
{
	disconnect(m_length_spin, 0, 0, 0);
	double value = a; value /= SLIDER_MULTIPLIER;
	m_length_spin->setValue(value);
	slot_property_changed();
	connect(m_length_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_m_length_slider(double)));
}

void color_scale_property::slot_m_font_size_slider(int a)
{
	disconnect(m_font_size_slider, 0, 0, 0);
	m_font_size_slider->setValue(a);
	slot_property_changed();
	connect(m_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_spin(int)));
}

void color_scale_property::slot_m_font_size_spin(int a)
{
	disconnect(m_font_size_spin, 0, 0, 0);
	m_font_size_spin->setValue(a);
	slot_property_changed();
	connect(m_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_font_size_slider(int)));
}

void color_scale_property::slot_m_note_number(int a)
{
	slot_annotation_changed();
}

void color_scale_property::slot_m_note_font_size_slider(int a)
{
	disconnect(m_note_font_size_slider, 0, 0, 0);
	int value = a;
	m_note_font_size_slider->setValue(value);
	slot_property_changed();
	connect(m_note_font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_spin(int)));
}

void color_scale_property::slot_m_note_font_size_spin(int a)
{
	disconnect(m_note_font_size_spin, 0, 0, 0);
	int value = a;
	m_note_font_size_spin->setValue(value);
	slot_property_changed();
	connect(m_note_font_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_note_font_size_slider(int)));
}

void color_scale_property::slot_m_adopt_self_note(int a)
{
	slot_property_changed();
}

void color_scale_property::slot_color_type(int a)
{
	slot_annotation_changed();
}

void color_scale_property::slot_annotation_changed()
{
	property_changed(0);
}

void color_scale_property::slot_significant(int a)
{
	slot_annotation_changed();
}

void color_scale_property::slot_property_changed()
{
	property_changed(1);
}

void color_scale_property::property_changed(int type)
{
	data->m_direction = (ColorBarDirection)m_color_place_direction->currentIndex();// D_HORIZONTAL;
	data->m_xPos = (float)m_x_position_spin->value();
	data->m_yPos = (float)m_y_position_spin->value();
	data->m_width = (float)m_width_spin->value();
	data->m_height = (float)m_length_spin->value();
	data->m_title.m_title = m_title_edit->text().toStdString();
	data->m_title.m_fontSize = m_font_size_spin->value();
	data->m_label.m_num = m_note_number_spin->value();
	data->m_label.m_fontSize = m_note_font_size_spin->value();
	data->m_label.m_customOn = m_adopt_self_note->isChecked();
	data->m_label.m_type = m_type->currentIndex() %3;
	data->m_label.m_decimalDigits = m_significant_number_spin->value();
	data->m_label.m_preText = m_prefix_edit->text().toStdString();
	data->m_label.m_lastText = m_suffix_edit->text().toStdString();
	emit color_scale_property_changed(data, type);
}
