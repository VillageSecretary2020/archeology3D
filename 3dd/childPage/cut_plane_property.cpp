#include "cut_plane_property.h"
#include <QHBoxLayout>

cut_plane_property::cut_plane_property()
{
	this->setColumnCount(2);
	m_cut_plane_treeitem = new QTreeWidgetItem(this, QStringList(STRING_ITEM_CUTPLANE) );
	m_show_point_treeitem = new QTreeWidgetItem(m_cut_plane_treeitem, QStringList(STRING_ITEM_SEL_MODE));
	//m_show_point = new QCheckBox;
	//m_show_point->setChecked(false);
	m_select_mode = new QComboBox;
	m_select_mode->insertItem(0, STRING_ITEM_VSURF);
	m_select_mode->insertItem(1, STRING_ITEM_VPARA);
	m_select_mode->insertItem(2, STRING_ITEM_THREE_POINTS);
	this->setItemWidget(m_show_point_treeitem, 1, m_select_mode);//m_show_point
#if TRI_POINT
	m_three_points_treeitem = new QTreeWidgetItem(this, QStringList(STRING_ITEM_AXIS_POINTS) );
	m_point_treeitem[0] = new QTreeWidgetItem(m_three_points_treeitem, QStringList(STRING_ITEM_POINT_A));
	m_point_treeitem[1] = new QTreeWidgetItem(m_three_points_treeitem, QStringList(STRING_ITEM_POINT_B));
	m_point_treeitem[2] = new QTreeWidgetItem(m_three_points_treeitem, QStringList(STRING_ITEM_POINT_C));
	 QHBoxLayout *point_layout[3];
	QWidget *select_point_widget[3];
	int ic;
	for(ic=0;ic<3;ic++){
		m_select_point[ic]= new QCheckBox;
		m_select_point[ic]->setChecked(false);

		m_label_point[ic] = new QLabel;

		 point_layout[ic]= new QHBoxLayout;

		point_layout[ic]->addWidget(m_select_point[ic], 15);
		point_layout[ic]->addWidget(m_label_point[ic], 85);
		select_point_widget[ic]= new QWidget;
		select_point_widget[ic]->setLayout(point_layout[ic]);
		this->setItemWidget(m_point_treeitem[ic], 1, select_point_widget[ic]);
	}
#endif
	m_axis_treeitem = new QTreeWidgetItem(m_three_points_treeitem, QStringList(STRING_ITEM_AXISES));
	m_axis_combo = new QComboBox;
	m_axis_combo->insertItem(0, STRING_ITEM_XAXIS);
	m_axis_combo->insertItem(1, STRING_ITEM_YAXIS);
	m_axis_combo->insertItem(2, STRING_ITEM_ZAXIS);
	this->setItemWidget(m_axis_treeitem, 1, m_axis_combo);

	m_normal_treeitem = new QTreeWidgetItem(this, QStringList(STRING_ITEM_NORMAL) );
	m_place_direction_treeitem = new QTreeWidgetItem(m_normal_treeitem, QStringList(STRING_ITEM_PLACEDIR));
	m_normal_direction_treeitem = new QTreeWidgetItem(m_normal_treeitem, QStringList(STRING_ITEM_NORMALDIR));
	m_normal_direction_treeitem->setText(1, "(0,0,1)");
	m_normal_x_direction_treeitem = new QTreeWidgetItem(m_normal_direction_treeitem, QStringList(tr("x")));
	//m_normal_x_direction_treeitem->setText(1, "0");
	m_normal_y_direction_treeitem = new QTreeWidgetItem(m_normal_direction_treeitem, QStringList(tr("y")));
	//m_normal_y_direction_treeitem->setText(1, "0");
	m_normal_z_direction_treeitem = new QTreeWidgetItem(m_normal_direction_treeitem, QStringList(tr("z")));
	//m_normal_z_direction_treeitem->setText(1, "1");
	m_x_normal_edit = new QLineEdit;
	m_x_normal_edit->setText("0");
	m_y_normal_edit = new QLineEdit;
	m_y_normal_edit->setText("0");
	m_z_normal_edit = new QLineEdit;
	m_z_normal_edit->setText("1");
	this->setItemWidget(m_normal_x_direction_treeitem, 1, m_x_normal_edit);
	this->setItemWidget(m_normal_y_direction_treeitem, 1, m_y_normal_edit);
	this->setItemWidget(m_normal_z_direction_treeitem, 1, m_z_normal_edit);
	m_offset_from_center_treeitem = new QTreeWidgetItem(m_normal_treeitem, QStringList(STRING_ITEM_OFFSETCENT));
	m_place_direction = new QComboBox;
	m_place_direction->insertItem(0, "xy");
	m_place_direction->insertItem(1, "xz");
	m_place_direction->insertItem(2, "yz");
	this->setItemWidget(m_place_direction_treeitem, 1, m_place_direction);
	m_offset_from_center_edit = new QLineEdit;
	m_offset_from_center_slider = new QSlider(Qt::Horizontal);
	//m_offset_from_center_slider->setRange(0, 200);
	//m_offset_from_center_slider->setValue(100);
	QWidget *offset_from_center_widget = new QWidget;
	QHBoxLayout *offset_from_center_layout = new QHBoxLayout;
	offset_from_center_layout->addWidget(m_offset_from_center_edit, 15);
	offset_from_center_layout->addWidget(m_offset_from_center_slider, 85);
	m_point_button = new QPushButton;
	m_point_button->setText("...");
	//offset_from_center_layout->addWidget(m_point_button, 15);
	offset_from_center_widget->setLayout(offset_from_center_layout);
	this->setItemWidget(m_offset_from_center_treeitem, 1, offset_from_center_widget);
	this->insertTopLevelItem(0, m_cut_plane_treeitem);
	expandAll();
	int ind = m_select_mode->currentIndex();
	m_three_points_treeitem->setHidden(0==ind);
	m_normal_treeitem->setHidden(0!=ind);
	//m_normal_treeitem->setHidden(m_show_point->isChecked());
	m_point_treeitem[2]->setHidden(2 != ind);
	m_axis_treeitem->setHidden(1!=ind);
}

void cut_plane_property::set_data(Child_CutingPlane* cut_plane, int multi, int offset)
{
	data = cut_plane;
	mp = multi; offs = offset;
	m_place_direction->setCurrentIndex(data->m_direction);
	m_x_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_x));
	m_y_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_y));
	m_z_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_z));
	range[0] = data->m_normalInfo.m_x;
	range[1] = data->m_normalInfo.m_y;
	range[2] = data->m_normalInfo.m_z;
	set_normal_text();
	m_select_mode->setCurrentIndex(data->m_mode);
	m_axis_combo->setCurrentIndex(data->m_axis);

	m_offset_from_center_edit->setText(QString("%1").arg(data->m_centerOffset));
	//m_offset_from_center_edit->setText("0");
	m_offset_from_center_slider->setRange(0, multi);//
	m_offset_from_center_slider->setValue(data->m_centerOffset  * mp+offset);
}

void cut_plane_property::set_range(float* r)
{
	memcpy_s(range, sizeof(range), r, sizeof(range));
	place_direction();
	set_normal_text();
}

void	cut_plane_property::connect_items()
{
	connect(m_place_direction, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_place_direction()));
	connect(m_x_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_y_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_z_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_offset_from_center_edit, SIGNAL(editingFinished()), this, SLOT(slot_offset_from_center_slider()));
	connect(m_offset_from_center_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_offset_from_center_spin(int)));
	//connect(m_point_button, SIGNAL(clicked(bool)), this, SLOT(slot_select_point()));
	//connect(m_show_point, SIGNAL(stateChanged(int)), this, SLOT(slot_show_point()));
#if TRI_POINT
	connect(m_select_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_select_mode(int)));
	connect(m_axis_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_select_axis(int)));
	connect(m_select_point[0], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point1()));
	connect(m_select_point[1], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point2()));
	connect(m_select_point[2], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point3()));
#endif
}

void cut_plane_property::slot_place_direction()
{
	place_direction();
	emit cut_plane_property_changed(data, 1);
}

void cut_plane_property::place_direction()
{
	CutPlaneDirection d= (CutPlaneDirection)m_place_direction->currentIndex();
	disconnect(m_x_normal_edit, 0, 0, 0);
	disconnect(m_y_normal_edit,  0, 0, 0);
	disconnect(m_z_normal_edit,  0, 0, 0);
	//disconnect(m_select_mode,  0, 0, 0);
	data->m_normalInfo = { 0, 0, 0 };
	if (0 == d) 
		data->m_normalInfo.m_z = range[2];
	else if (1 == d) 
		data->m_normalInfo.m_y = range[1];
	else
		data->m_normalInfo.m_x = range[0];
	m_x_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_x));
	m_y_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_y));
	m_z_normal_edit->setText(QString("%1").arg(data->m_normalInfo.m_z));
	//set_normal_text();

	connect(m_x_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_y_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_z_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	//connect(m_select_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_select_mode(int)));
	emit cut_plane_property_changed(data, 1);
}

void cut_plane_property::set_normal(double* nor)
{
	disconnect(m_x_normal_edit,  0, 0, 0);
	disconnect(m_y_normal_edit, 0, 0, 0);
	disconnect(m_z_normal_edit, 0, 0, 0);
	if(0<m_point_n)
		m_label_point[m_point_n-1]->setText(QString("%1 %2 %3").arg(nor[0]).arg(nor[1]).arg(nor[2]));
	else{
		m_x_normal_edit->setText(QString("%1").arg(nor[0]));
		m_y_normal_edit->setText(QString("%1").arg(nor[1]));
		m_z_normal_edit->setText(QString("%1").arg(nor[2]));
	}
	connect(m_x_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_y_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
	connect(m_z_normal_edit, SIGNAL(editingFinished()), this, SLOT(slot_normal()));
}

void cut_plane_property::slot_normal()
{
	data->m_normalInfo.m_x = m_x_normal_edit->text().toFloat();
	data->m_normalInfo.m_y = m_y_normal_edit->text().toFloat();
	data->m_normalInfo.m_z = m_z_normal_edit->text().toFloat();
	//set_normal_text();
	emit cut_plane_property_changed(data, 2);
}

void cut_plane_property::slot_select_axis(int ind)
{
	data->m_axis = ind;
	emit cut_plane_property_changed(data, 4);
}

void cut_plane_property::slot_select_mode(int ind)
{
	m_three_points_treeitem->setHidden(0 == ind);
	m_normal_treeitem->setHidden(0 != ind);
	m_point_treeitem[2]->setHidden(2 != ind);
	m_axis_treeitem->setHidden(1 != ind);
	data->m_mode = ind;
	if (1 == ind) {
		m_three_points_treeitem->setText(1, STRING_ITEM_VPARA_DESC);
	}else if(2==ind)
		m_three_points_treeitem->setText(1, STRING_ITEM_THREE_POINTS_DESC);

	emit cut_plane_property_changed(data, 20);
}

void cut_plane_property::select_point(int ch)
{
	int ic;
	int type=22;
	disconnect(m_select_point[0], 0, 0, 0);
	disconnect(m_select_point[1], 0, 0, 0);
	disconnect(m_select_point[2], 0, 0, 0);
	for (ic = 0; ic < 3; ic++)
		m_select_point[ic]->setChecked(false);
	if (0 < ch) {
		m_select_point[ch-1]->setChecked(true);
		type = 22+ch;
		m_point_n = ch;
	}
	else
		m_point_n = 0;
	connect(m_select_point[0], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point1()));
	connect(m_select_point[1], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point2()));
	connect(m_select_point[2], SIGNAL(stateChanged(int)), this, SLOT(slot_select_point3()));
	emit cut_plane_property_changed(data, type);
}

void cut_plane_property::slot_select_point1()
{
	select_point(m_select_point[0]->isChecked() ?1:-1);
}

void cut_plane_property::slot_select_point2()
{
	select_point(m_select_point[1]->isChecked() ?2:-2);
}

void cut_plane_property::slot_select_point3()
{
	select_point(m_select_point[2]->isChecked() ?3:-3);
}

void cut_plane_property::slot_show_point()
{
	//m_three_points_treeitem->setHidden(!m_show_point->isChecked());
	//m_normal_treeitem->setHidden(m_show_point->isChecked());
	//emit cut_plane_property_changed(data, 20+ (m_show_point->isChecked()?1:0));
}

// 0826改用于提示range值
void cut_plane_property::set_normal_text()
{
	QString normal_direction = QString(STRING_ITEM_RANGE);
	normal_direction += QString("(");
	normal_direction += QString("%1").arg(range[0]);// (data->m_normalInfo.m_x);
	normal_direction += QString(", ");
	normal_direction += QString("%1").arg(range[1]);// (data->m_normalInfo.m_y);
	normal_direction += QString(", ");
	normal_direction += QString("%1").arg(range[2]);// (data->m_normalInfo.m_z);
	normal_direction += QString(")");
	m_normal_direction_treeitem->setText(1, normal_direction);
}

void cut_plane_property::slot_offset_from_center_slider()
{
	data->m_centerOffset = m_offset_from_center_edit->text().toFloat();
	disconnect(m_offset_from_center_slider, 0, 0, 0);
	int value = data->m_centerOffset * mp + offs;
	m_offset_from_center_slider->setValue(value);
	connect(m_offset_from_center_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_offset_from_center_spin(int)));
	data->m_normalInfo.m_x = m_x_normal_edit->text().toFloat();
	data->m_normalInfo.m_y = m_y_normal_edit->text().toFloat();
	data->m_normalInfo.m_z = m_z_normal_edit->text().toFloat();
	emit cut_plane_property_changed(data, 3);
}

void cut_plane_property::slot_offset_from_center_spin(int a)
{
	disconnect(m_offset_from_center_edit, 0, 0, 0);
	float value = (a - offs);  value /= mp;
	m_offset_from_center_edit->setText(QString("%1").arg(value));
	connect(m_offset_from_center_edit, SIGNAL(textChanged(QString)), this, SLOT(slot_offset_from_center_slider()));
	data->m_centerOffset = m_offset_from_center_edit->text().toFloat();
	data->m_normalInfo.m_x = m_x_normal_edit->text().toFloat();
	data->m_normalInfo.m_y = m_y_normal_edit->text().toFloat();
	data->m_normalInfo.m_z = m_z_normal_edit->text().toFloat();
	emit cut_plane_property_changed(data, 3);
}

void cut_plane_property::slot_property_changed()
{
	data->m_direction = (CutPlaneDirection)m_place_direction->currentIndex();
	data->m_normalInfo.m_x = m_x_normal_edit->text().toFloat();
	data->m_normalInfo.m_y = m_y_normal_edit->text().toFloat();
	data->m_normalInfo.m_z = m_z_normal_edit->text().toFloat();
	data->m_centerOffset = m_offset_from_center_edit->text().toFloat();
	emit cut_plane_property_changed(data, 0);
}
