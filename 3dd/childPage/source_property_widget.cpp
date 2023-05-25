// source_property_widget.cpp

#include "source_property_widget.h"

source_property_widget::source_property_widget(QString a, SuperNodeStruct* param)
{
	create_rbuttons(this, QFileInfo(a).fileName());
	//常规属性
	m_normal_property = new QTreeWidget;
	m_normal_property->setColumnCount(2);
	m_normal_property->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_normal_property->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
#if 0
	QTreeWidgetItem *file_name = new QTreeWidgetItem(m_normal_property, QStringList(a));
	QTreeWidgetItem *file_path = new QTreeWidgetItem(file_name, QStringList("wowowo"));
	QTreeWidgetItem *work_table = new QTreeWidgetItem(file_name, QStringList(tr("worksheet")));
	QPushButton *edit_work_table = new QPushButton;
	edit_work_table->setText(tr("edit sheet"));
	m_normal_property->setItemWidget(work_table, 1,edit_work_table);
	m_normal_property->insertTopLevelItem(0,file_name);
	connect(edit_work_table, SIGNAL(clicked()), this, SLOT(slot_edit_table()));
	QTreeWidgetItem *output_property = new QTreeWidgetItem(m_normal_property, QStringList(tr("output properties")));
	QTreeWidgetItem *output_type = new QTreeWidgetItem(output_property, QStringList(tr("output type")));
	m_normal_property->insertTopLevelItem(1,output_property);
	m_output_type = new QComboBox;
	m_normal_property->setItemWidget(output_type, 1,m_output_type);
	QTreeWidgetItem *work_table_data_column = new QTreeWidgetItem(m_normal_property, QStringList(tr("data columns for worksheet")));
	QTreeWidgetItem *x_coordinate = new QTreeWidgetItem(work_table_data_column, QStringList(tr("X coordinate")));
	QTreeWidgetItem *y_coordinate = new QTreeWidgetItem(work_table_data_column, QStringList(tr("Y coordinate")));
	QTreeWidgetItem *z_coordinate = new QTreeWidgetItem(work_table_data_column, QStringList(tr("Z coordinate")));
	QTreeWidgetItem *component_attribution = new QTreeWidgetItem(work_table_data_column, QStringList(tr("component attribute")));
	QTreeWidgetItem *component_associate_column = new QTreeWidgetItem(component_attribution, QStringList(tr("number of component associated columns")));
	QTreeWidgetItem *component = new QTreeWidgetItem(component_attribution, QStringList(tr("compoent-1")));
	QTreeWidgetItem *mark = new QTreeWidgetItem(work_table_data_column, QStringList(tr("mark")));
	QTreeWidgetItem *mark_data_column = new QTreeWidgetItem(mark, QStringList(tr("number of data columns labeled")));
	QTreeWidgetItem *mark_1 = new QTreeWidgetItem(mark, QStringList(tr("mark-1")));
	m_x_coordinate = new QComboBox;
	m_normal_property->setItemWidget(x_coordinate, 1,m_x_coordinate);
	m_y_coordinate = new QComboBox;
	m_normal_property->setItemWidget(y_coordinate, 1,m_y_coordinate);
	m_z_coordinate = new QComboBox;
	m_normal_property->setItemWidget(z_coordinate, 1,m_z_coordinate);
	m_component = new QComboBox;
	m_normal_property->setItemWidget(component, 1,m_component);
	m_note = new QComboBox;
	m_normal_property->setItemWidget(mark_1, 1,m_note);
	m_normal_property->insertTopLevelItem(2,work_table_data_column);
	QTreeWidgetItem *work_table_data_row = new QTreeWidgetItem(m_normal_property, QStringList(tr("worksheet data row")));
	QTreeWidgetItem *title_line = new QTreeWidgetItem(work_table_data_row, QStringList(tr("title line")));
	QTreeWidgetItem *load_all_line = new QTreeWidgetItem(work_table_data_row, QStringList(tr("load all rows")));
	QTreeWidgetItem *first_line = new QTreeWidgetItem(work_table_data_row, QStringList(tr("first line")));
	QTreeWidgetItem *last_line = new QTreeWidgetItem(work_table_data_row, QStringList(tr("last line")));
	m_load_all_rows = new QCheckBox;
	m_normal_property->setItemWidget(load_all_line, 1,m_load_all_rows);
	m_form1_tab = new QTabWidget;
	m_form1_tab->addTab(m_normal_property, tr("general properties"));
#endif 
	//m_path_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(QFileInfo(a).fileName()));
	m_pre_treeitem = new QTreeWidgetItem(m_normal_property, QStringList(STRING_ITEM_INPUT));
	m_file_path_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_FILEPATH));
	m_file_path_treeitem->setText(1, a);
	//m_normal_property->insertTopLevelItem(0, m_path_treeitem);
	m_input_data_count_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_DATA_COUNT));
	m_input_data_count_treeitem->setText(1, "0");
	m_field_index_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_FIELDS));
	m_field_index_treeitem->setText(1, "0");
	m_normal_property->expandAll();

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_normal_property, QString(STRING_TAB_GENERAL));
	D_DataInfo *m_return_property_struct = new D_DataInfo;
	baseStruct = m_return_property_struct;
	if (param)
		*m_return_property_struct = *(D_DataInfo *)param;
	m_return_property_struct->m_filePath = a.toStdString();
	m_input_data_count_treeitem->setText(1, QString::number(m_return_property_struct->data_count));
	m_field_index_treeitem->setText(1, QString::number(m_return_property_struct->fields_num));

	this->NodeType = STRING_DTYPE_SOURCE;	// "source_widget"; // 
	m_can_append_widget.push_back(STRING_WIDGET_SCATTER);//("scatter_widget");
	//m_can_append_widget.push_back("vector_widget");
	//m_can_append_widget.push_back("axis_widget");
	m_can_append_widget.push_back(STRING_CTYPE_GRIDDING);	// ("mesh_widget");
	this->m_can_append_widget.push_back(STRING_CTYPE_TRANSFORM);
	this->m_can_append_widget.push_back(STRING_CTYPE_SUBSET);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_EXCLU);
	m_can_append_widget.push_back(STRING_CTYPE_FILTER_REPEAT);
	m_can_append_widget.push_back(STRING_WIDGET_BORDER);
	m_can_append_widget.push_back(STRING_GTYPE_HIGH_LOW);
	m_can_append_widget.push_back(STRING_GTYPE_PICTURE);
	connect(this, SIGNAL(sig_set_progress(int, int, const char*)), this, SLOT(slot_set_progress(int, int, const char*)));
}

source_property_widget::~source_property_widget()
{
	delete m_form1_tab;
}

void source_property_widget::paintEvent(QPaintEvent *e)
{
#if 0
	QPainter p(this);
	//p.setCompositionMode(QPainter::CompositionMode_Clear);
	//p.fillRect(0, 0, 10, 40, Qt::SolidPattern);
	//p.fillRect(0, 0, 10, 30, Qt::gray);
	p.fillRect(this->width() - 10, 0, 10, 30, Qt::gray);
#endif

	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 0) << QPoint(this->width() - 10, 0) << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15);
	polygon << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30) << QPoint(0, 30);
	setMask(polygon);
}

void source_property_widget::contextMenuEvent(QContextMenuEvent  *event) {
	bool dem= m_file_path_treeitem->text(1).right(3).contains(QString("dem"), Qt::CaseInsensitive);
	QMainWindow* receiver1 = (QMainWindow*)OperationalLayer::getOperationInstance().getMainWin();
	QMenu *menu = new QMenu(this);
	QMenu *out_put = menu->addMenu(STRING_MENU_GRAOUTPUT);
	if(!dem){
	out_put->addAction(STRING_ACTION_SACTDIAGRAM,	receiver1, SLOT(create_scatter())); 
	out_put->addAction(STRING_ACTION_BOUNDBOX,	receiver1, SLOT(create_border()));
	//out_put->addAction(STRING_ACTION_PICTURE,	receiver1, SLOT(create_picture()));
	//connect(new1, SIGNAL(sig_data_to_highlowmap(QString)), this, SLOT(create_highlowmap(QString)));
	}else
		out_put->addAction(STRING_ACTION_HIANLOREA,	receiver1, SLOT(create_highlowmap()));//
	//QMenu *normal = menu->addMenu(tr("normal"));
	//normal->addAction(tr("message"), this, SLOT(slot_create_mess()));
	if (!dem) {
		QMenu *calc = menu->addMenu(STRING_MENU_CALC);
		//calc->addAction(tr("change tye"), this, SLOT(f1()));
		calc->addAction(STRING_ACTION_TRANSFORM, receiver1, SLOT(create_transform()));
		//calc->addAction(STRING_ACTION_SUBSET, receiver1, SLOT(create_subset()));
		calc->addAction(STRING_ACTION_EXCLUFILTER, receiver1, SLOT(create_exclusion_filter()));
		calc->addAction(STRING_ACTION_REPEATFILT, receiver1, SLOT(create_repeat_filter()));
		calc->addAction(STRING_ACTION_GRIDDING, receiver1, SLOT(create_gridding()));
	}
	//if (this->node->end_node)
	//	menu->addAction(new QAction(tr("connect output data(") + QString(this->node->end_node->message) + QString(")"), this));
	//else
	//	menu->addAction(new QAction(tr("connect output data"), this));
	menu->addAction(STRING_ACTION_CONN_OUT, this, SLOT(slot_connect_output()));
	// 0804 menu->addAction(new QAction(tr("save data"), this));
	menu->addAction(STRING_ACTION_COPY, this, SLOT(slot_copy_widget()));
	//menu->addAction(new QAction(STRING_ACTION_DELETE, this));
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_source())); //this, SLOT(slot_delete_self()));	// yzw 0705
	menu->addAction(STRING_ACTION_RENAME, this, SLOT(slot_rename_control()));
	//menu->move(cursor().pos());
	//menu->show();
	menu->exec(cursor().pos());
	//释放
	QList<QAction*> list = menu->actions();
	foreach(QAction* pAction, list) delete pAction;
	delete menu;
}

void source_property_widget::delete_self()
{
	emit sig_delete_source();
}

void source_property_widget::end_progress()
{
	OperationalLayer::getOperationInstance().nextNodeRun(this->id_key);	// 在主线程内运行vtk相关操作，避免某些异常
}

#if 0
void source_property_widget::slot_edit_table()
{
	threeDdetectSystem* three = (threeDdetectSystem*)(parentWidget()->parentWidget()->parentWidget());
	QString str1 = QString(QLatin1String(desc));
	QFile file(str1);
	if (file.open(QIODevice::ReadOnly) == true)
	{
		QTextStream  text(&file);
		text.setCodec("UTF-8");   //设置编码格式
		QString  str = text.readAll();
		QStringList list0 = str.split("\r\n");
		Mytable  *table0 = new Mytable(list0.length(), three->return_m_tab());
		table0->setRowCount(list0.length());
		for (int i = 0; i < list0.length(); i++)
		{
			QStringList  temp = list0[i].split("\t");
			for (int j = 0; j < temp.length(); j++)
			{
				table0->setItem(i, j, new QTableWidgetItem(temp[j]));
			}
		}
		//three->m_tab->addTab(table0, QFileInfo(str1).fileName());
		three->return_m_tab()->addTab(table0, QFileInfo(str1).fileName());
		//int index = three->m_tab->count();
		int index = three->return_m_tab()->count();
		//three->m_tab->setCurrentIndex(index - 1);
		three->return_m_tab()->setCurrentIndex(index - 1);
	}
}
#endif

D_DataInfo* source_property_widget::return_data_source_property_struct()
{
	getBaseStruct();
	return (D_DataInfo*)baseStruct;
}

bool source_property_widget::set_progress(int r, int p, const char* m)
{
	emit  sig_set_progress(r, p, m);
	return true;
}

void source_property_widget::slot_set_progress(int r, int p, const char* m)
{
	data_count_to_widget(r, p);
}
