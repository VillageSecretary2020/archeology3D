
// axis_property_widget.cpp

#include "axis_property_widget.h"
#include "border_property_widget.h"
#include "myqwidget.h"
#include "My_QTreeWidgetItem.h"
//#include "threeDdetectSystem.h"
//#include "mytable.h"
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#define AXIS_NOTE_EXT 0
#define AXIS_NOTE_SHOW_OPT 0
axis_property_widget::axis_property_widget(int num,G_Axis* axis)
{
	//init struct
	m_return_axis_property_struct = new G_Axis;
	baseStruct = m_return_axis_property_struct;

	QString label = (axis) ? QString::fromStdString(axis->m_name) : ( (num == 0)? STRING_LABEL_AXIS : STRING_LABEL_AXIS + QString::number(num + 1));
	 create_2buttons(this, label);
	//常规属性
	m_normal_property = new QTreeWidget(this);
	//m_normal_property->setHeaderLabel(tr("axis"));
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1,"empty");
	m_input_treeitem->setDisabled(true);

	//m_normal_property->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_normal_axis_treeitem = new QTreeWidgetItem(m_normal_property,QStringList(STRING_ITEM_AXIS));
#if 0
	m_order_by_data_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem,QStringList(tr("by data size")));
	m_order_by_data = new QCheckBox;
	m_order_by_data->setChecked(true);
	m_normal_property->setItemWidget(m_order_by_data_treeitem, 1,m_order_by_data);
	m_show_mesh_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(tr("display grid lines")));	// 网格线
	m_mesh_color_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(tr("gridline color")));
	m_meshline_width_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(tr("grid line width")));
	m_font_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(tr("font")));
	m_show_mesh = new QCheckBox;
	m_show_mesh->setChecked(false);
	m_normal_property->setItemWidget(m_show_mesh_treeitem, 1,m_show_mesh);
	m_mesh_color = new QComboBox;
	color_choose(m_mesh_color);
	m_normal_property->setItemWidget(m_mesh_color_treeitem, 1, m_mesh_color);
	m_mesh_color_treeitem->setDisabled(true);
	m_mesh_color->setEnabled(false);
	m_meshline_width_spin= new QSpinBox;
	m_meshline_width_spin->setRange(0, 4);
	m_meshline_width_spin->setValue(1);
	m_meshline_width_slider= new QSlider(Qt::Horizontal);
	m_meshline_width_slider->setRange(0, 4);
	m_meshline_width_slider->setValue(1);
	QHBoxLayout *meshline_width_layout = new QHBoxLayout;
	meshline_width_layout->addWidget(m_meshline_width_spin);
	meshline_width_layout->addWidget(m_meshline_width_slider);
	QWidget *meshline_width_widget = new QWidget;
	meshline_width_widget->setLayout(meshline_width_layout);
	m_normal_property->setItemWidget(m_meshline_width_treeitem, 1, meshline_width_widget);
	m_meshline_width_treeitem->setDisabled(true);
	m_meshline_width_spin->setEnabled(false);
	m_meshline_width_slider->setEnabled(false);
	//m_meshline_width_treeitem->set
	m_font = new QComboBox;
	m_font->insertItem(0, "verdana");
	m_font->insertItem(1, "vijaya");
	m_font->insertItem(2, "vrinda");
	m_font->insertItem(3, "Webdings");
	m_normal_property->setItemWidget(m_font_treeitem, 1, m_font);
#endif
	//m_show_arrow_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(tr("show arrows")));
	m_axis_thickness_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(STRING_ITEM_THICKOFAX));
	m_mark_size_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(STRING_ITEM_MARKSIZE));
	m_title_size_treeitem = new QTreeWidgetItem(m_normal_axis_treeitem, QStringList(STRING_ITEM_TITSIZE));
	m_show_arrow = new QCheckBox;
	m_show_arrow->setChecked(true);
	//m_normal_property->setItemWidget(m_show_arrow_treeitem, 1, m_show_arrow);
	m_axis_thickness_spin = new QDoubleSpinBox;
	m_axis_thickness_spin->setDecimals(2);
	m_axis_thickness_spin->setSingleStep(0.01);
	m_axis_thickness_spin->setValue(1);
	m_axis_thickness_spin->setRange(0, 100);
	m_axis_thickness_slider = new QSlider(Qt::Horizontal);
	m_axis_thickness_slider->setRange(0, 10000);
	m_axis_thickness_slider->setValue(100);
	QHBoxLayout *axis_thickness_layout = new QHBoxLayout;
	axis_thickness_layout->addWidget(m_axis_thickness_spin);
	axis_thickness_layout->addWidget(m_axis_thickness_slider);
	QWidget *axis_thickness_widget = new QWidget;
	axis_thickness_widget->setLayout(axis_thickness_layout);
	m_normal_property->setItemWidget(m_axis_thickness_treeitem, 1, axis_thickness_widget);
	m_mark_size_spin = new QSpinBox;
	m_mark_size_spin->setRange(0, 100);
	m_mark_size_spin->setValue(1);
	m_mark_size_slider = new QSlider(Qt::Horizontal);
	m_mark_size_slider->setRange(0, 100);
	m_mark_size_slider->setValue(1);
	QHBoxLayout *mark_size_layout = new QHBoxLayout;
	mark_size_layout->addWidget(m_mark_size_spin);
	mark_size_layout->addWidget(m_mark_size_slider);
	QWidget *mark_size_widget = new QWidget;
	mark_size_widget->setLayout(mark_size_layout);
	m_normal_property->setItemWidget(m_mark_size_treeitem, 1, mark_size_widget);
	m_title_size_spin = new QSpinBox;
	m_title_size_spin->setRange(0, 100);
	m_title_size_spin->setValue(1);
	m_title_size_slider = new QSlider(Qt::Horizontal);
	m_title_size_slider->setRange(0, 100);
	m_title_size_slider->setValue(1);
	QHBoxLayout *title_size_layout = new QHBoxLayout;
	title_size_layout->addWidget(m_title_size_spin);
	title_size_layout->addWidget(m_title_size_slider);
	QWidget *title_size_widget = new QWidget;
	title_size_widget->setLayout(title_size_layout);
	m_normal_property->setItemWidget(m_title_size_treeitem, 1, title_size_widget);
	m_normal_property->expandAll();

	//connect(m_axis_thickness_slider, SIGNAL(valueChanged(int)), this, SLOT(f11(int)));
	//connect(m_mark_size_slider, SIGNAL(valueChanged(int)), this, SLOT(f12(int)));
	 //x轴属性
	m_x_axis = new QTreeWidget(this);
	m_x_axis->setColumnCount(2);
	m_x_axis->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_x_axis->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_x_axis_treeitem = new QTreeWidgetItem(m_x_axis, QStringList(STRING_ITEM_XAXIS));
	m_x_axis->insertTopLevelItem(0,m_x_axis_treeitem);
    m_x_note_treeitem = new QTreeWidgetItem(m_x_axis, QStringList(STRING_ITEM_MARK));
	m_x_axis->insertTopLevelItem(1,m_x_note_treeitem);
#if AXIS_NOTE_SHOW_OPT
	m_x_show_axis_treeitem = new QTreeWidgetItem(m_x_axis_treeitem, QStringList(STRING_ITEM_SHOWAXIS));
	m_x_show_axis = new QCheckBox;
	m_x_show_axis->setChecked(true);
	m_x_axis->setItemWidget(m_x_show_axis_treeitem, 1, m_x_show_axis);
#endif
	m_x_color_treeitem = new QTreeWidgetItem(m_x_axis_treeitem, QStringList(STRING_ITEM_COLOR));
	m_xColor = new QComboBox;
	color_choose(m_xColor);
	m_xColor->setCurrentText("red");
	m_x_axis->setItemWidget(m_x_color_treeitem, 1, m_xColor);

#if AXIS_NOTE_EXT
	m_x_axis_min_treeitem = new QTreeWidgetItem(m_x_axis_treeitem, QStringList(STRING_ITEM_EXT_LEFT));
	m_x_axis_min_edit = new QLineEdit;
	m_x_axis_min_edit->setText( "-100");
	m_x_axis->setItemWidget(m_x_axis_min_treeitem,1, m_x_axis_min_edit);
	m_x_axis_max_treeitem = new QTreeWidgetItem(m_x_axis_treeitem, QStringList(STRING_ITEM_EXT_RIGHT));
	m_x_axis_max_edit = new QLineEdit;
	m_x_axis_max_edit->setText( "100");
	m_x_axis->setItemWidget(m_x_axis_max_treeitem,1, m_x_axis_max_edit);
#endif
	m_x_note_show_treeitem = new QTreeWidgetItem(m_x_note_treeitem, QStringList(STRING_ITEM_SHOWMARK));
	m_x_show_note = new QCheckBox;
	m_x_show_note->setChecked(true);
	m_x_axis->setItemWidget(m_x_note_show_treeitem, 1, m_x_show_note);
	m_x_title_treeitem = new QTreeWidgetItem(m_x_note_treeitem, QStringList(STRING_ITEM_TITLE));
	m_x_title_edit = new QLineEdit;
	m_x_title_edit->setText(STRING_ITEM_XAXIS);
	m_x_axis->setItemWidget(m_x_title_treeitem,1, m_x_title_edit);
#if AXIS_NOTE_EXT
	m_x_note_min_treeitem1 = new QTreeWidgetItem(m_x_note_treeitem, QStringList(tr("minimum value of mark")));
	m_x_note_min_edit = new QLineEdit;
	m_x_note_min_edit->setText( "-100");
	m_x_axis->setItemWidget(m_x_note_min_treeitem1,1, m_x_note_min_edit);
	m_x_note_max_treeitem = new QTreeWidgetItem(m_x_note_treeitem, QStringList(tr("maximum value of mark")));
	m_x_note_max_edit = new QLineEdit;
	m_x_note_max_edit->setText( "100");
	m_x_axis->setItemWidget(m_x_note_max_treeitem,1, m_x_note_max_edit);
	m_x_note_interval_treeitem = new QTreeWidgetItem(m_x_note_treeitem, QStringList(tr("mark interval increment")));
	m_x_note_interval_edit = new QLineEdit;
	m_x_note_interval_edit->setText( "2");
	m_x_axis->setItemWidget(m_x_note_interval_treeitem,1, m_x_note_interval_edit);
	m_x_note_format_treeitem = new QTreeWidgetItem(m_x_note_treeitem, QStringList(tr("mark format")));
	m_x_note_type_treeitem = new QTreeWidgetItem(m_x_note_format_treeitem, QStringList(tr("type")));
	m_x_note_significant_treeitem = new QTreeWidgetItem(m_x_note_format_treeitem, QStringList(tr("significant number")));	
	m_x_note_type = new QComboBox;
	//m_x_axis->setItemWidget(m_x_note_type_treeitem, 1, m_x_note_type);
	m_x_note_type_treeitem->setText(1, "0");
	m_x_note_significant_treeitem->setText(1, "1");
	m_x_note_significant_spin = new QSpinBox;
	m_x_note_significant_spin->setValue(7);
	//m_x_axis->setItemWidget(m_x_note_significant_treeitem, 1, m_x_note_significant_spin);
#endif
	m_x_axis->expandAll();

	//y轴属性
	m_y_axis = new QTreeWidget(this);
	m_y_axis->setColumnCount(2);
	m_y_axis->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
//	m_y_axis->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_y_axis_treeitem = new QTreeWidgetItem(m_y_axis, QStringList(STRING_ITEM_YAXIS));
	m_y_axis->insertTopLevelItem(0,m_y_axis_treeitem);
    m_y_note_treeitem = new QTreeWidgetItem(m_y_axis, QStringList(STRING_ITEM_MARK));
	m_y_axis->insertTopLevelItem(1,m_y_note_treeitem);
#if AXIS_NOTE_SHOW_OPT
	m_y_show_axis_treeitem = new QTreeWidgetItem(m_y_axis_treeitem, QStringList(STRING_ITEM_SHOWAXIS));
	m_y_show_axis = new QCheckBox;
	m_y_show_axis->setChecked(true);
	m_y_axis->setItemWidget(m_y_show_axis_treeitem, 1, m_y_show_axis);
#endif
	m_y_color_treeitem = new QTreeWidgetItem(m_y_axis_treeitem, QStringList(STRING_ITEM_COLOR));
	m_yColor = new QComboBox;
	color_choose(m_yColor);
	m_yColor->setCurrentText("green");
	m_y_axis->setItemWidget(m_y_color_treeitem, 1, m_yColor);

#if AXIS_NOTE_EXT
	m_y_axis_min_treeitem = new QTreeWidgetItem(m_y_axis_treeitem, QStringList(STRING_ITEM_EXT_TOP));
	m_y_axis_min_edit = new QLineEdit;
	m_y_axis_min_edit->setText( "-100");
	m_y_axis->setItemWidget(m_y_axis_min_treeitem,1, m_y_axis_min_edit);
	m_y_axis_max_treeitem = new QTreeWidgetItem(m_y_axis_treeitem, QStringList(STRING_ITEM_EXT_BOTTOM));
	m_y_axis_max_edit = new QLineEdit;
	m_y_axis_max_edit->setText( "100");
	m_y_axis->setItemWidget(m_y_axis_max_treeitem,1, m_y_axis_max_edit);
#endif
	m_y_note_show_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList(STRING_ITEM_SHOWMARK));
	m_y_note_show_check = new QCheckBox;
	m_y_note_show_check->setChecked(true);
	m_y_axis->setItemWidget(m_y_note_show_treeitem, 1, m_y_note_show_check);
	m_y_title_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList(STRING_ITEM_TITLE));
	m_y_title_edit = new QLineEdit;
	m_y_title_edit->setText(STRING_ITEM_YAXIS);
	m_y_axis->setItemWidget(m_y_title_treeitem,1, m_y_title_edit);
#if AXIS_NOTE_EXT
	m_y_note_min_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList("note min"));
	m_y_note_min_edit = new QLineEdit;
	m_y_note_min_edit->setText( "-100");
	m_y_axis->setItemWidget(m_y_note_min_treeitem,1, m_y_note_min_edit);
	m_y_note_max_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList("note max"));
	m_y_note_max_edit = new QLineEdit;
	m_y_note_max_edit->setText( "100");
	m_y_axis->setItemWidget(m_y_note_max_treeitem,1, m_y_note_max_edit);
	m_y_note_interval_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList("note interval"));
	m_y_note_interval_edit = new QLineEdit;
	m_y_note_interval_edit->setText( "2");
	m_y_axis->setItemWidget(m_y_note_interval_treeitem,1, m_y_note_interval_edit);
	m_y_note_format_treeitem = new QTreeWidgetItem(m_y_note_treeitem, QStringList("note format"));
	m_y_note_type_treeitem = new QTreeWidgetItem(m_y_note_format_treeitem, QStringList("type"));
	m_y_note_significant_treeitem = new QTreeWidgetItem(m_y_note_format_treeitem, QStringList("significant"));

	m_y_note_type_combox = new QComboBox;
	//m_y_note_type_combox->insertItem(0, "fixed");
	//m_y_note_type_combox->insertItem(1, "Exponential");
	//m_y_note_type_combox->insertItem(2, "General");
	//m_y_note_type_combox->setCurrentIndex(2);
	//m_y_axis->setItemWidget(m_y_note_type_treeitem, 1, m_y_note_type_combox);
	m_y_note_type_treeitem->setText(1, "0");
	m_y_note_significant_treeitem->setText(1, "1");
	m_y_note_significant_spin = new QSpinBox;
	//m_y_note_significant_spin->setValue(7);
	//m_y_axis->setItemWidget(m_y_note_significant_treeitem, 1, m_y_note_significant_spin);
#endif
	m_y_axis->expandAll();

	//z轴属性
	m_z_axis = new QTreeWidget(this);
	m_z_axis->setColumnCount(2);
	m_z_axis->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_z_axis->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_z_axis_treeitem = new QTreeWidgetItem(m_z_axis, QStringList(STRING_ITEM_ZAXIS));
	m_z_axis->insertTopLevelItem(0,m_z_axis_treeitem);
    m_z_note_treeitem = new QTreeWidgetItem(m_z_axis, QStringList(STRING_ITEM_MARK));
	m_z_axis->insertTopLevelItem(1,m_z_note_treeitem);
#if AXIS_NOTE_SHOW_OPT
	m_z_show_axis_treeitem = new QTreeWidgetItem(m_z_axis_treeitem, QStringList(STRING_ITEM_SHOWAXIS));
	m_z_show_axis = new QCheckBox;
	m_z_show_axis->setChecked(true);
	m_z_axis->setItemWidget(m_z_show_axis_treeitem, 1, m_z_show_axis);
#endif
	m_z_color_treeitem = new QTreeWidgetItem(m_z_axis_treeitem, QStringList(STRING_ITEM_COLOR));
	m_zColor = new QComboBox;
	color_choose(m_zColor);
	m_zColor->setCurrentText("blue");
	m_z_axis->setItemWidget(m_z_color_treeitem, 1, m_zColor);

#if AXIS_NOTE_EXT
	m_z_axis_min_treeitem = new QTreeWidgetItem(m_z_axis_treeitem, QStringList(STRING_ITEM_EXT_FORE));
	m_z_axis_min_edit = new QLineEdit;
	m_z_axis_min_edit->setText( "-100");
	m_z_axis->setItemWidget(m_z_axis_min_treeitem,1, m_z_axis_min_edit);
	m_z_axis_max_treeitem = new QTreeWidgetItem(m_z_axis_treeitem, QStringList(STRING_ITEM_EXT_BACK));
	m_z_axis_max_edit = new QLineEdit;
	m_z_axis_max_edit->setText( "100");
	m_z_axis->setItemWidget(m_z_axis_max_treeitem,1, m_z_axis_max_edit);
#endif
	m_z_note_show_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList(STRING_ITEM_SHOWMARK));
	m_z_note_show_check = new QCheckBox;
	m_z_note_show_check->setChecked(true);
	m_z_axis->setItemWidget(m_z_note_show_treeitem, 1, m_z_note_show_check);

	m_z_title_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList(STRING_ITEM_TITLE));
	m_z_title_edit = new QLineEdit;
	m_z_title_edit->setText(STRING_ITEM_ZAXIS);
	m_z_axis->setItemWidget(m_z_title_treeitem,1, m_z_title_edit);
#if AXIS_NOTE_EXT
	m_z_note_min_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList("note min"));
	m_z_note_min_edit = new QLineEdit;
	m_z_note_min_edit->setText( "-100");
	m_z_axis->setItemWidget(m_z_note_min_treeitem,1, m_z_note_min_edit);
	m_z_note_max_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList("note max"));
	m_z_note_max_edit = new QLineEdit;
	m_z_note_max_edit->setText( "100");
	m_z_axis->setItemWidget(m_z_note_max_treeitem,1, m_z_note_max_edit);
	m_z_note_interval_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList("note interval"));
	m_z_note_interval_edit = new QLineEdit;
	m_z_note_interval_edit->setText( "2");
	m_z_axis->setItemWidget(m_z_note_interval_treeitem,1, m_z_note_interval_edit);
	m_z_note_format_treeitem = new QTreeWidgetItem(m_z_note_treeitem, QStringList("note format"));
	m_z_note_type_treeitem = new QTreeWidgetItem(m_z_note_format_treeitem, QStringList("type"));
	m_z_note_significant_treeitem = new QTreeWidgetItem(m_z_note_format_treeitem, QStringList("significant"));

	m_z_note_type_combox = new QComboBox;
	m_z_note_type_treeitem->setText(1, "0");
	m_z_note_significant_treeitem->setText(1, "1");
	m_z_note_significant_spin = new QSpinBox;
#endif
	m_z_axis->expandAll();

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, STRING_TAB_GENERAL);
	m_form1_tab->addTab(m_x_axis, STRING_TAB_XAXIS);
	m_form1_tab->addTab(m_y_axis, STRING_TAB_YAXIS);
	m_form1_tab->addTab(m_z_axis, STRING_TAB_ZAXIS);

	if (!axis) 
	{
		GAxis::getDefault(m_return_axis_property_struct);
		axis = m_return_axis_property_struct;
	}
	{
		m_input_treeitem->setText(1, QString::fromStdString(axis->m_putIn));
		//m_show_mesh->setChecked(axis->m_display_gridLine);
		//m_mesh_color->setCurrentText(crgb2name(axis->m_LineRgb));
		//m_mesh_color->setEnabled(axis->m_display_gridLine);
		//m_meshline_width_spin->setValue(axis->m_lineWidth);
		//m_meshline_width_slider->setValue(axis->m_lineWidth);
		//m_font->setCurrentIndex(axis->m_font);
		//m_show_arrow->setChecked(axis->m_showArrows);
		m_axis_thickness_spin->setValue(axis->m_axisDiameter);
		m_axis_thickness_slider->setValue(axis->m_axisDiameter * 100);
		m_mark_size_spin->setValue(axis->m_labelMarkSize);
		m_mark_size_slider->setValue(axis->m_labelMarkSize);
		m_title_size_spin->setValue(axis->m_textSize);
		m_title_size_slider->setValue(axis->m_textSize);

		m_xColor->setCurrentText(crgb2name(axis->m_xAxis.m_rgb));
#if AXIS_NOTE_SHOW_OPT
		m_x_show_axis->setChecked(axis->m_xAxis.m_showAxis);
#endif
		m_x_title_edit->setText( QString::fromStdString(axis->m_xAxis.m_textTitle));
		m_x_show_note->setChecked(axis->m_xAxis.m_labelmark.m_showLabelMark);
#if AXIS_NOTE_EXT
		m_x_axis_min_edit->setText( QString::number(axis->m_xAxis.m_minValue));
		m_x_axis_max_edit->setText( QString::number(axis->m_xAxis.m_maxValue));
		m_x_note_min_edit->setText(QString::number(axis->m_xAxis.m_labelmark.m_minValue));//(1,setTextQString::number(
		m_x_note_max_edit->setText(QString::number(axis->m_xAxis.m_labelmark.m_maxValue));
		m_x_note_interval_edit->setText(QString::number(axis->m_xAxis.m_labelmark.m_intervalValue));
		m_x_note_format_treeitem->setText(1, QString::fromStdString(axis->m_xAxis.m_labelmark.m_format));
		//m_x_note_type->setCurrentIndex(axis->m_xAxis.m_labelmark.)
		//m_x_note_significant_spin->setValue(axis->m_xAxis.m_labelmark.m_significant_digits);
#endif

		m_yColor->setCurrentText(crgb2name(axis->m_yAxis.m_rgb));
#if AXIS_NOTE_SHOW_OPT
		m_y_show_axis->setChecked(axis->m_yAxis.m_showAxis);
#endif
		m_y_title_edit->setText(QString::fromStdString(axis->m_yAxis.m_textTitle));
		m_y_note_show_check->setChecked(axis->m_yAxis.m_labelmark.m_showLabelMark);
#if AXIS_NOTE_EXT
		m_y_axis_min_edit->setText(QString::number(axis->m_yAxis.m_minValue));
		m_y_axis_max_edit->setText(QString::number(axis->m_yAxis.m_maxValue));
		m_y_note_min_edit->setText(QString::number(axis->m_yAxis.m_labelmark.m_minValue));
		m_y_note_max_edit->setText(QString::number(axis->m_yAxis.m_labelmark.m_maxValue));
		m_y_note_interval_edit->setText(QString::number(axis->m_yAxis.m_labelmark.m_intervalValue));
		m_y_note_format_treeitem->setText(1, QString::fromStdString(axis->m_yAxis.m_labelmark.m_format));
		//m_y_note_type->setCurrentIndex(axis->m_yAxis.m_labelmark.)
		m_y_note_significant_spin->setValue(axis->m_yAxis.m_labelmark.m_significant_digits);
#endif
		m_zColor->setCurrentText(crgb2name(axis->m_zAxis.m_rgb));
#if AXIS_NOTE_SHOW_OPT
		m_z_show_axis->setChecked(axis->m_zAxis.m_showAxis);
#endif
		m_z_title_edit->setText(QString::fromStdString(axis->m_zAxis.m_textTitle));
		m_z_note_show_check->setChecked(axis->m_zAxis.m_labelmark.m_showLabelMark);
#if AXIS_NOTE_EXT
		m_z_axis_min_edit->setText(QString::number(axis->m_zAxis.m_minValue));
		m_z_axis_max_edit->setText(QString::number(axis->m_zAxis.m_maxValue));
		m_z_note_min_edit->setText(QString::number(axis->m_zAxis.m_labelmark.m_minValue));
		m_z_note_max_edit->setText(QString::number(axis->m_zAxis.m_labelmark.m_maxValue));
		m_z_note_interval_edit->setText(QString::number(axis->m_zAxis.m_labelmark.m_intervalValue));
		m_z_note_format_treeitem->setText(1, QString::fromStdString(axis->m_zAxis.m_labelmark.m_format));
		m_z_note_significant_spin->setValue(axis->m_zAxis.m_labelmark.m_significant_digits);
#endif
	}
	
	//connect(m_order_by_data, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
	//connect(m_show_mesh, SIGNAL(stateChanged(int)), this, SLOT(slot_show_mesh()));
	//connect(m_mesh_color, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	//connect(m_meshline_width_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_meshline_width_slider_setValue(int)));
	//connect(m_meshline_width_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_meshline_width_spin_setValue(int)));
	//connect(m_font, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_font_slot(int)));
	//connect(m_show_arrow, SIGNAL(stateChanged(int)), this, SIGNAL(slot_show_arrow_slot(int)));
	connect(m_axis_thickness_spin, SIGNAL(valueChanged(double)), this, SLOT(slot_axis_thickness_slider_setValue(double)));
	connect(m_axis_thickness_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_axis_thickness_spin_setValue(int)));
	connect(m_mark_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_mark_size_slider_setValue(int)));
	connect(m_mark_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_mark_size_spin_setValue(int)));
	connect(m_title_size_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_title_size_slider_setValue(int)));
	connect(m_title_size_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_title_szie_spin_setValue(int)));
	connect(m_xColor, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_yColor, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_zColor, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));

#if AXIS_NOTE_SHOW_OPT
	connect(m_x_show_axis, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#endif
	connect(m_x_title_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_show_note, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#if AXIS_NOTE_EXT
	connect(m_x_axis_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_axis_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_note_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_note_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_note_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_x_note_type, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_x_note_significant_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
#endif

#if AXIS_NOTE_SHOW_OPT
	connect(m_y_show_axis, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#endif
	connect(m_y_title_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_show_check, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#if AXIS_NOTE_EXT
	connect(m_y_axis_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_axis_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_type_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_y_note_significant_spin,SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
#endif
#if AXIS_NOTE_SHOW_OPT
	connect(m_z_show_axis, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#endif
	connect(m_z_title_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_show_check, SIGNAL(stateChanged(int)), this, SLOT(slot_update_widget_property()));
#if AXIS_NOTE_EXT
	connect(m_z_axis_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_axis_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_min_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_max_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_interval_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_type_combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_update_widget_property()));
	connect(m_z_note_significant_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_update_widget_property()));
#endif
	//m_return_axis_property_struct = new G_Axis;
	this->NodeType = STRING_WIDGET_AXIS;
}

axis_property_widget::~axis_property_widget()
{
	delete m_form1_tab;
}


void axis_property_widget::paintEvent(QPaintEvent *e)
{
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}


void axis_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu   *menu = new QMenu(this);
#if 0
	QMenu  *out_put = menu->addMenu(tr("graphic output"));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_axis_to_border()));
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
	menu->addAction(new QAction(tr("sava data"), this));
#endif
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_COPY, this,SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE,receiver1, SLOT(delete_axis()));
	menu->move(cursor().pos());
	menu->show();
}

void axis_property_widget::delete_self()
{
	emit sig_delete_axis();
}

G_Axis* axis_property_widget::return_axis_property_struct()
{
	getBaseStruct();
	m_return_axis_property_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_axis_property_struct->m_display_gridLine = false;// m_show_mesh->isChecked();
	//m_return_axis_property_struct->m_LineRgb = name2crgb(m_mesh_color->currentText());
	//m_return_axis_property_struct->m_lineWidth = m_meshline_width_spin->value();
	//m_return_axis_property_struct->m_font = m_font->currentIndex();
	m_return_axis_property_struct->m_showArrows = false;// m_show_arrow->isChecked();
	m_return_axis_property_struct->m_axisDiameter = m_axis_thickness_spin->value();
	m_return_axis_property_struct->m_labelMarkSize = m_mark_size_spin->value();
	m_return_axis_property_struct->m_textSize = m_title_size_spin->value();
#if AXIS_NOTE_SHOW_OPT
	m_return_axis_property_struct->m_xAxis.m_showAxis = m_x_show_axis->isChecked();
#endif
	m_return_axis_property_struct->m_xAxis.m_textTitle = m_x_title_edit->text().toStdString();
	//m_return_axis_property_struct.m_xAxis.m_rgb
	m_return_axis_property_struct->m_xAxis.m_rgb = 	name2crgb(m_xColor->currentText());

	m_return_axis_property_struct->m_xAxis.m_labelmark.m_showLabelMark = m_x_show_note->isChecked();
#if AXIS_NOTE_EXT
	m_return_axis_property_struct->m_xAxis.m_minValue = m_x_axis_min_edit->text().toInt();
	m_return_axis_property_struct->m_xAxis.m_maxValue = m_x_axis_max_edit->text().toInt();
	m_return_axis_property_struct->m_xAxis.m_labelmark.m_minValue = m_x_note_min_edit-> text().toInt();
	m_return_axis_property_struct->m_xAxis.m_labelmark.m_maxValue = m_x_note_max_edit->text().toInt();
	m_return_axis_property_struct->m_xAxis.m_labelmark.m_intervalValue = m_x_note_interval_edit->text().toInt();
	m_return_axis_property_struct->m_xAxis.m_labelmark.m_format = m_x_note_format_treeitem->text(1).toStdString();
	m_return_axis_property_struct->m_xAxis.m_labelmark.m_significant_digits = 0;// m_x_note_significant_spin->value();
#endif

#if AXIS_NOTE_SHOW_OPT
	m_return_axis_property_struct->m_yAxis.m_showAxis = m_y_show_axis->isChecked();
#endif
	m_return_axis_property_struct->m_yAxis.m_textTitle = m_y_title_edit->text().toStdString();
	//m_return_axis_property_struct.m_yAxis.m_rgb
	m_return_axis_property_struct->m_yAxis.m_rgb = 	name2crgb(m_yColor->currentText());
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_showLabelMark = m_y_note_show_check->isChecked();
#if AXIS_NOTE_EXT
	m_return_axis_property_struct->m_yAxis.m_minValue = m_y_axis_min_edit->text().toInt();
	m_return_axis_property_struct->m_yAxis.m_maxValue = m_y_axis_max_edit->text().toInt();
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_minValue = m_y_note_min_edit->text().toInt();
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_maxValue = m_y_note_max_edit->text().toInt();
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_intervalValue = m_y_note_interval_edit->text().toInt();
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_format = m_y_note_format_treeitem->text(1).toStdString();
	m_return_axis_property_struct->m_yAxis.m_labelmark.m_significant_digits = 0;//m_y_note_significant_spin->value();
#endif
#if AXIS_NOTE_SHOW_OPT
	m_return_axis_property_struct->m_zAxis.m_showAxis = m_z_show_axis->isChecked();
#endif
	m_return_axis_property_struct->m_zAxis.m_textTitle = m_z_title_edit->text().toStdString();
	//m_return_axis_property_struct.m_yAxis.m_rgb

	m_return_axis_property_struct->m_zAxis.m_rgb =	name2crgb(m_zColor->currentText());
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_showLabelMark = m_z_note_show_check->isChecked();
#if AXIS_NOTE_EXT
	m_return_axis_property_struct->m_zAxis.m_minValue = m_z_axis_min_edit->text().toInt();
	m_return_axis_property_struct->m_zAxis.m_maxValue = m_z_axis_max_edit->text().toInt();
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_minValue = m_z_note_min_edit->text().toInt();
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_maxValue = m_z_note_max_edit->text().toInt();
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_intervalValue = m_z_note_interval_edit->text().toInt();
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_format = m_z_note_format_treeitem->text(1).toStdString();
	m_return_axis_property_struct->m_zAxis.m_labelmark.m_significant_digits = m_z_note_significant_spin->value();
#endif
	return m_return_axis_property_struct;
}

void axis_property_widget::m_update_widget_property()
{
	G_Axis* pTemp = this->return_axis_property_struct();
	std::shared_ptr<G_Axis>ptr(std::make_shared<G_Axis>());
	*(ptr.get()) = *pTemp;	// yzw 0630
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void axis_property_widget::slot_show_mesh()
{
	if (m_show_mesh->isChecked())
	{
		m_mesh_color_treeitem->setDisabled(false);
		m_mesh_color->setEnabled(true);
		m_meshline_width_treeitem->setDisabled(false);
		m_meshline_width_spin->setEnabled(true);
		m_meshline_width_slider->setEnabled(true);
	}
	else
	{
		m_mesh_color_treeitem->setDisabled(true);
		m_mesh_color->setEnabled(false);
		m_meshline_width_treeitem->setDisabled(true);
		m_meshline_width_spin->setEnabled(false);
		m_meshline_width_slider->setEnabled(false);
	}
	m_update_widget_property();
}

void axis_property_widget::slot_meshline_width_slider_setValue(int a)
{
	m_meshline_width_slider->setValue(a);
	m_update_widget_property();

}

void axis_property_widget::slot_meshline_width_spin_setValue(int a)
{
	m_meshline_width_spin->setValue(a);
	m_update_widget_property();
}
void axis_property_widget::slot_axis_thickness_slider_setValue(double a)
{
	int value = (int)(a * 100);
	m_axis_thickness_slider->setValue(value);
	m_update_widget_property();
}

void axis_property_widget::slot_axis_thickness_spin_setValue(int a)
{
	double value = a / 100.0;
	m_axis_thickness_spin->setValue(value);
	m_update_widget_property();
}

void axis_property_widget::slot_mark_size_slider_setValue(int a)
{
	m_mark_size_slider->setValue(a);
	m_update_widget_property();
}

void axis_property_widget::slot_mark_size_spin_setValue(int a)
{
	m_mark_size_spin->setValue(a);
	m_update_widget_property();
}

void axis_property_widget::slot_title_size_slider_setValue(int a)
{
	m_title_size_slider->setValue(a);
	m_update_widget_property();
}

void axis_property_widget::slot_title_szie_spin_setValue(int a)
{
	m_title_size_spin->setValue(a);
	m_update_widget_property();
}

void axis_property_widget::slot_axis_to_border()
{
	emit sig_axis_to_border();
}

void axis_property_widget::slot_update_widget_property()
{
	m_update_widget_property();
}
