#include "repeat_filter_property_widget.h"
#include "myqwidget.h"

repeat_filter_property_widget::repeat_filter_property_widget(int num, C_RepeatFilter* repeat_filter)
{
	QString label = (repeat_filter) ? QString::fromStdString(repeat_filter->m_name) : ( (num == 0)? STRING_LABEL_REPEFILTER : STRING_LABEL_REPEFILTER + QString::number(num + 1));
	create_2buttons(this, label);

	m_normal_tree = new QTreeWidget;
	m_normal_tree->setColumnCount(2);
	m_normal_tree->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)

	m_pre_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	m_input_treeitem->setText(1, "empty");
	m_input_treeitem->setDisabled(true);

	m_repeat_filter_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_REPEFILTER));
	m_data_error_treeitem = new QTreeWidgetItem(m_normal_tree, QStringList(STRING_ITEM_DATAERROR));
	m_x_error_treeitem = new QTreeWidgetItem(m_data_error_treeitem, QStringList(STRING_ITEM_XERROR));
	m_x_error_edit = new QLineEdit(this);
	connect(m_x_error_edit, SIGNAL(editingFinished()), this, SLOT(slot_x_error())); //按下回车或者失去焦点时触发
	m_normal_tree->setItemWidget(m_x_error_treeitem, 1, m_x_error_edit);
	m_y_error_treeitem = new QTreeWidgetItem(m_data_error_treeitem, QStringList(STRING_ITEM_YERROR));
	m_y_error_edit = new QLineEdit(this);
	connect(m_y_error_edit, SIGNAL(editingFinished()), this, SLOT(slot_y_error()));
	m_normal_tree->setItemWidget(m_y_error_treeitem, 1, m_y_error_edit);
	m_z_error_treeitem = new QTreeWidgetItem(m_data_error_treeitem, QStringList(STRING_ITEM_ZERROR));
	m_z_error_edit = new QLineEdit(this);
	connect(m_z_error_edit, SIGNAL(editingFinished()), this, SLOT(slot_z_error()));
	m_normal_tree->setItemWidget(m_z_error_treeitem, 1, m_z_error_edit);
	m_normal_tree->insertTopLevelItem(0, m_repeat_filter_treeitem);
	m_normal_tree->insertTopLevelItem(1, m_data_error_treeitem);
	m_normal_tree->expandAll();

	m_return_repeat_filter_struct = new C_RepeatFilter;
	baseStruct = m_return_repeat_filter_struct;
	if (!repeat_filter) {
		CRepeatFilterNode::getDefault(m_return_repeat_filter_struct);
	}else
	{
		*m_return_repeat_filter_struct = *repeat_filter;
	}repeat_filter = m_return_repeat_filter_struct;
	{
		m_input_treeitem->setText(1, QString::fromStdString(repeat_filter->m_putIn));
		m_x_error_edit->setText(QString("%1").arg(repeat_filter->m_Xtolerance));
		m_y_error_edit->setText(QString("%1").arg(repeat_filter->m_Ytolerance));
		m_z_error_edit->setText(QString("%1").arg(repeat_filter->m_Ztolerance));
	}
	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_tree,QString(STRING_TAB_GENERAL));

	this->NodeType = STRING_CTYPE_FILTER_REPEAT;
	//CAN_APPEND_WIDGET_GTYPE_ALL(m_can_append_widget);
	this->m_can_append_widget.push_back(STRING_WIDGET_SCATTER);
	this->m_can_append_widget.push_back(STRING_WIDGET_AXIS);
	this->m_can_append_widget.push_back(STRING_WIDGET_BORDER);
	this->m_can_append_widget.push_back(STRING_GTYPE_HIGH_LOW);

	this->m_can_append_widget.push_back(STRING_CTYPE_TRANSFORM);
	this->m_can_append_widget.push_back(STRING_CTYPE_SUBSET);
	this->m_can_append_widget.push_back(STRING_CTYPE_FILTER_EXCLU);
	this->m_can_append_widget.push_back(STRING_CTYPE_FILTER_REPEAT);
	this->m_can_append_widget.push_back(STRING_CTYPE_GRIDDING);
}

repeat_filter_property_widget::~repeat_filter_property_widget()
{
	delete m_form1_tab;
}

C_RepeatFilter* repeat_filter_property_widget::return_repeat_filter_property_struct()
{
	getBaseStruct();
	m_return_repeat_filter_struct->m_putIn = m_input_treeitem->text(1).toStdString();
	m_return_repeat_filter_struct->m_Xtolerance = m_x_error_edit->text().toFloat();
	m_return_repeat_filter_struct->m_Ytolerance = m_y_error_edit->text().toFloat();
	m_return_repeat_filter_struct->m_Ztolerance = m_z_error_edit->text().toFloat();
	return m_return_repeat_filter_struct;
}

void repeat_filter_property_widget::m_update_widget_property()
{
	C_RepeatFilter* pTemp = this->return_repeat_filter_property_struct();
	std::shared_ptr<C_RepeatFilter>ptr(std::make_shared<C_RepeatFilter>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void repeat_filter_property_widget::paintEvent(QPaintEvent *e)
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
	p.fillRect(0, 0, 10, 30, Qt::gray);
	p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);
#endif
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

void repeat_filter_property_widget::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *menu = new QMenu(this);
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAPHIC);
#if 1
	out_put->addAction(STRING_ACTION_SACTDIAGRAM, receiver1, SLOT(create_scatter()));
	out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border()));
#else
	out_put->addAction(STRING_ACTION_HIANLOREA, receiver1, SLOT(create_highlowmap()));
	MENU_RENDER_CALC_NODE(out_put, receiver1);
	out_put->addAction(STRING_ACTION_SCATTER, this, SLOT(slot_repeat_filter_to_scatter()));
	out_put->addAction(tr("axis"), this, SLOT(slot_repeat_filter_to_border()));
	out_put->addAction(tr("bounding box"), this, SLOT(slot_create_border()));
#endif
	//QMenu *normal = menu->addMenu(tr("normal module"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	QMenu *calc = menu->addMenu(STRING_MENU_CALC);
	calc->addAction(STRING_ACTION_TRANSFORM, receiver1, SLOT(create_transform()));// this, SLOT(slot_gridding_to_transform()));
	//calc->addAction(STRING_ACTION_SUBSET, receiver1, SLOT(create_subset()));// this, SLOT(slot_gridding_to_subset()));
	calc->addAction(STRING_ACTION_EXCLUFILTER, receiver1, SLOT(create_exclusion_filter()));
	calc->addAction(STRING_ACTION_REPEATFILT, receiver1, SLOT(create_repeat_filter()));
	calc->addAction(STRING_ACTION_GRIDDING, receiver1, SLOT(create_gridding()));
	//if (this->node->ptr_node)
	//	menu->addAction(new QAction(tr("connect the number of point input(") + QString(this->node->ptr_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect the number of point input(not connect)"), this));
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect the number of point output(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect the number of point output"), this));
	menu->addAction(STRING_ACTION_CONN_IN, this, SLOT(slot_connect_input()));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
	menu->addAction(STRING_ACTION_EXPORT, this, SLOT(slot_save_widget()));
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_repeat_filter()) );
	menu->move(cursor().pos());
	menu->show();
}

void repeat_filter_property_widget::delete_self()
{
	emit sig_delete_repeat();
}

void repeat_filter_property_widget::slot_repeat_filter_to_scatter()
{
	emit sig_repeat_filter_to_scatter();
}

void repeat_filter_property_widget::slot_repeat_filter_to_axis()
{
	emit sig_repeat_filter_to_axis();
}

void repeat_filter_property_widget::slot_repeat_filter_to_border()
{
	emit sig_repeat_filter_to_border();
}

void repeat_filter_property_widget::slot_repeat_filter_to_transform()
{
	emit sig_repeat_filter_to_transform();
}

void repeat_filter_property_widget::slot_repeat_filter_to_exclusion_filter()
{
	emit sig_repeat_filter_to_exclusion_filter();
}

void repeat_filter_property_widget::slot_repeat_filter_to_gridding()
{
	emit sig_repeat_filter_to_gridding();
}

void repeat_filter_property_widget::slot_repeat_filter_to_repeat_filter()
{
	emit sig_repeat_filter_to_repeat_filter();
}

void repeat_filter_property_widget::slot_x_error()
{
	m_update_widget_property();
}

void repeat_filter_property_widget::slot_y_error()
{
	m_update_widget_property();
}

void repeat_filter_property_widget::slot_z_error()
{
	m_update_widget_property();
}
