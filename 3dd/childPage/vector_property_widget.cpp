#include "vector_property_widget.h"
#include "axis_property_widget.h"
#include "myqwidget.h"
vector_property_widget::vector_property_widget(QString a)
{
	create_2buttons(this, a);

	m_NormalProperty = new QTreeWidget;
	m_NormalProperty->setColumnCount(2);
	m_NormalProperty->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList("input"));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(tr("source")));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_vector_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString("vector")));
	m_input_scalar_treeitem = new QTreeWidgetItem(m_vector_treeitem, QStringList(QString("input scalar")));
	m_input_scalar_treeitem->setText(1, "empty");
	m_place_direction_treeitem = new QTreeWidgetItem(m_vector_treeitem, QStringList(QString("place direction")));
    m_frequent_treeitem = new QTreeWidgetItem(m_vector_treeitem, QStringList(QString("frequency")));
	m_Frequency = new QSpinBox;
	m_Frequency->setValue(1);
	m_NormalProperty->setItemWidget(m_frequent_treeitem, 1, m_Frequency);
	m_render_treeitem = new QTreeWidgetItem(m_NormalProperty, QStringList(QString("render")));
	m_vector_style_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("vector style")));
	m_VectorStyle = new QComboBox;
	m_VectorStyle->insertItem(0, "linear");
	m_VectorStyle->insertItem(1, "Two wing arrow");
	m_VectorStyle->insertItem(2, "Four wing arrow");
	m_VectorStyle->insertItem(3, "cross arrow");
	m_VectorStyle->setCurrentIndex(1);
	m_NormalProperty->setItemWidget(m_vector_style_treeitem, 1, m_VectorStyle);
	m_scaling_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("scaling method")));
	m_ScalingMethod = new QComboBox;
	m_ScalingMethod->insertItem(0, "linear");
	m_ScalingMethod->insertItem(1, "square root");
	m_ScalingMethod->insertItem(2, "logarithm");
	m_NormalProperty->setItemWidget(m_scaling_method_treeitem, 1, m_ScalingMethod);
	m_min_rate_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("min scale")));
	m_MinRate_spin = new QDoubleSpinBox;
	m_MinRate_spin->setDecimals(4);
	m_MinRate_spin->setSingleStep(0.001);
	m_MinRate_spin->setValue(0);
	m_MinRate_spin->setRange(0, 5);
	m_MinRate_slider = new QSlider(Qt::Horizontal);
	m_MinRate_slider->setRange(0, 50000);
	m_MinRate_slider->setValue(0);
	connect(m_MinRate_spin, SIGNAL(valueChanged(double)), this, SLOT(m_MinRate_slider_setValue(double)));
	connect(m_MinRate_slider, SIGNAL(valueChanged(int)), this, SLOT(m_MinRate_spin_setValue(int)));
	QWidget* minscale_widget = new QWidget;
	QHBoxLayout* minscale_layout = new QHBoxLayout;
	minscale_layout->addWidget(m_MinRate_spin);
	minscale_layout->addWidget(m_MinRate_slider);
	minscale_widget->setLayout(minscale_layout);
	m_NormalProperty->setItemWidget(m_min_rate_treeitem, 1, minscale_widget);
	m_max_rate_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("max scale")));
	m_MaxRate_spin = new QDoubleSpinBox;
	m_MaxRate_spin->setDecimals(4);
	m_MaxRate_spin->setSingleStep(0.001);
	m_MaxRate_spin->setValue(1);
	m_MaxRate_spin->setRange(0, 5);
	m_MaxRate_slider = new QSlider(Qt::Horizontal);
	m_MaxRate_slider->setRange(0, 50000);
	m_MaxRate_slider->setValue(10000);
	connect(m_MaxRate_spin, SIGNAL(valueChanged(double)), this, SLOT(m_MaxRate_slider_setValue(double)));
	connect(m_MaxRate_slider, SIGNAL(valueChanged(int)), this, SLOT(m_MaxRate_spin_setValue(int)));
	QWidget* maxscale_widget = new QWidget;
	QHBoxLayout* maxscale_layout = new QHBoxLayout;
	maxscale_layout->addWidget(m_MaxRate_spin);
	maxscale_layout->addWidget(m_MaxRate_slider);
	maxscale_widget->setLayout(maxscale_layout);
	m_NormalProperty->setItemWidget(m_max_rate_treeitem, 1, maxscale_widget);
	m_arrow_rate_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("arrow rate")));
	m_ArrowRate_spin = new QDoubleSpinBox;
	m_ArrowRate_spin->setDecimals(4);
	m_ArrowRate_spin->setSingleStep(0.001);
	m_ArrowRate_spin->setValue(1);
	m_ArrowRate_spin->setRange(0, 5);
	m_ArrowRate_slider = new QSlider(Qt::Horizontal);
	m_ArrowRate_slider->setRange(0, 50000);
	m_ArrowRate_slider->setValue(10000);
	connect(m_ArrowRate_spin, SIGNAL(valueChanged(double)), this, SLOT(m_ArrowRate_slider_setValue(double)));
	connect(m_ArrowRate_slider, SIGNAL(valueChanged(int)), this, SLOT(m_ArrowRate_spin_setValue(int)));
	QWidget* arrowrate_widget = new QWidget;
	QHBoxLayout* arrowrate_layout = new QHBoxLayout;
	arrowrate_layout->addWidget(m_ArrowRate_spin);
	arrowrate_layout->addWidget(m_ArrowRate_slider);
	arrowrate_widget->setLayout(arrowrate_layout);
	m_NormalProperty->setItemWidget(m_arrow_rate_treeitem, 1, arrowrate_widget);
	m_symbol_size_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("symbol size")));
	m_SymbolSize_spin = new QDoubleSpinBox;
	m_SymbolSize_spin->setDecimals(4);
	m_SymbolSize_spin->setSingleStep(0.001);
	m_SymbolSize_spin->setValue(2);
	m_SymbolSize_spin->setRange(0, 48);
	m_SymbolSize_slider = new QSlider(Qt::Horizontal);
	m_SymbolSize_slider->setRange(0, 480000);
	m_SymbolSize_slider->setValue(20000);
	connect(m_SymbolSize_spin, SIGNAL(valueChanged(double)), this, SLOT(m_SymbolSize_slider_setValue(double)));
	connect(m_SymbolSize_slider, SIGNAL(valueChanged(int)), this, SLOT(m_SymbolSize_spin_setValue(int)));
	QWidget* symbolsize_widget = new QWidget;
	QHBoxLayout* symbolsize_layout = new QHBoxLayout;
	symbolsize_layout->addWidget(m_SymbolSize_spin);
	symbolsize_layout->addWidget(m_SymbolSize_slider);
	symbolsize_widget->setLayout(symbolsize_layout);
	m_NormalProperty->setItemWidget(m_symbol_size_treeitem, 1, symbolsize_widget);
	m_line_width_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString("line width")));
	m_LineWidth_spin = new QDoubleSpinBox;
	m_LineWidth_spin->setDecimals(4);
	m_LineWidth_spin->setSingleStep(0.001);
	m_LineWidth_spin->setValue(1);
	m_LineWidth_spin->setRange(0, 4);
	m_LineWidth_slider = new QSlider(Qt::Horizontal);
	m_LineWidth_slider->setRange(0, 40000);
	m_LineWidth_slider->setValue(10000);
	connect(m_LineWidth_spin, SIGNAL(valueChanged(double)),this, SLOT(m_LineWidth_slider_setValue(double)));
	connect(m_LineWidth_slider, SIGNAL(valueChanged(int)), this, SLOT(m_LineWidth_spin_setValue(int)));
	QWidget* linewidth_widget = new QWidget;
	QHBoxLayout* linewidth_layout = new QHBoxLayout;
	linewidth_layout->addWidget(m_LineWidth_spin);
	linewidth_layout->addWidget(m_LineWidth_slider);
	linewidth_widget->setLayout(linewidth_layout);
	m_NormalProperty->setItemWidget(m_line_width_treeitem, 1, linewidth_widget);
	m_reverse_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_REVERSE)));
	m_Reverse = new QCheckBox;
	m_NormalProperty->setItemWidget(m_reverse_treeitem, 1, m_Reverse);
	m_color_method_treeitem = new QTreeWidgetItem(m_render_treeitem, QStringList(QString(STRING_ITEM_COLORMETHOD)));
	m_ColorMethod = new QComboBox;
	m_ColorMethod->insertItem(0, STRING_COMBOX_GRADIENT);
	m_ColorMethod->insertItem(1, STRING_COMBOX_FIXED);
	m_ColorMethod->insertItem(2, STRING_ITEM_SCALLIST);
	m_NormalProperty->setItemWidget(m_color_method_treeitem, 1, m_ColorMethod);
	m_NormalProperty->insertTopLevelItem(0, m_vector_treeitem);
	m_NormalProperty->insertTopLevelItem(1, m_render_treeitem);
	m_NormalProperty->expandAll();
	m_form1_tab = new QTabWidget;
	m_form1_tab->addTab(m_NormalProperty, STRING_TAB_GENERAL);
}

void vector_property_widget::paintEvent(QPaintEvent *e)
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

	//QPainter p(this);
	//p.fillRect(0, 0, 10, 30, Qt::gray);
	//p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);

	QPolygon polygon = QPolygon();
	polygon << QPoint(0,15)<<QPoint(10,5)<<QPoint(10,0)<<QPoint(this->width()-10,0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);

}


void vector_property_widget::contextMenuEvent(QContextMenuEvent  *event)
{
	QMenu *menu = new QMenu(this);
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	out_put->addAction(tr("axis"), this, SLOT(slot_vector_to_axis()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_vector_to_border()));
	//QMenu *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect input module(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect input module"), this));
	menu->addAction(new QAction(tr("connecting pure scalar mesh"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output module(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//
	menu->addAction(new QAction(tr("connect output module"), this));
	menu->addAction(new QAction(tr("save data"), this));
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	menu->addAction(new QAction(STRING_ACTION_DELETE, this));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->move(cursor().pos());
	menu->show();
}

void vector_property_widget::delete_self()
{
	emit sig_delete_vector();
}

void vector_property_widget::slot_vector_to_axis()
{
	emit sig_vector_to_axis();
}

void vector_property_widget::slot_vector_to_border()
{
	emit sig_vector_to_border();
}
