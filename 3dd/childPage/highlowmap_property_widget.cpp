#include "highlowmap_property_widget.h"
#include "axis_property_widget.h"
#include "myqwidget.h"

highlowmap_property_widget::highlowmap_property_widget(int num, QString filename, G_HighLowZoneMap *data)
{
	m_return_border_property_struct = new G_HighLowZoneMap();
	baseStruct = m_return_border_property_struct;
	m_return_border_property_struct->demFile = filename.toStdString();
	QString label = (data) ? QString::fromStdString(data->m_name) : ( (num == 0)? STRING_LABEL_HIGHLOWMAP : STRING_LABEL_HIGHLOWMAP + QString::number(num + 1));
	 create_2buttons(this, label);
	//常规属性
	m_Normal_TreeWidget = new QTreeWidget;
	m_Normal_TreeWidget->setColumnCount(2);
	m_Normal_TreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents); //自适应内容列宽(列固定不能移动)
	//m_Normal_TreeWidget->setStyleSheet("QTreeWidget::item:!has-children{border-right:1px solid rgb(179,216,247);border-left:1px solid rgb(179,216,247);border-bottom: 1px solid rgb(179,216,247);border-top:1px solid rgb(179,216,247);padding: 2px; margin:0px;}");
	m_pre_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_INPUT));
	m_input_treeitem = new QTreeWidgetItem(m_pre_treeitem, QStringList(STRING_ITEM_SOURCE));
	
	m_normal_property_treeitem = new QTreeWidgetItem(m_Normal_TreeWidget, QStringList(STRING_ITEM_HIGHLOWMAP));
#if 0
	m_directtion_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_DIRECTION));
	m_directtion_combo = new QComboBox;
	m_directtion_combo->insertItem(0, "D_XY");
	m_directtion_combo->insertItem(1, "D_XZ");
	m_directtion_combo->insertItem(2, "D_YZ");
	m_Normal_TreeWidget->setItemWidget(m_directtion_treeitem, 1, m_directtion_combo);
	m_sectionId_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_SECTIONID));
	m_sectionId_edit = new QLineEdit;
	m_sectionId_edit->setText( "5");
	m_Normal_TreeWidget->setItemWidget(m_sectionId_treeitem, 1, m_sectionId_edit);
#endif
	m_scaleFactor_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_SCALEFACTOR));
	m_scaleFactor_edit = new QLineEdit;
	m_scaleFactor_edit->setText( "1.0");
	m_Normal_TreeWidget->setItemWidget(m_scaleFactor_treeitem, 1, m_scaleFactor_edit);

	m_renderInfo_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_RENDER));	// STRING_ITEM_RENDERINFO
	m_drawStyle_treeitem = new QTreeWidgetItem(m_renderInfo_treeitem, QStringList(STRING_ITEM_DRAWSTYLE));
	m_drawStyle_combo = new QComboBox;
	m_drawStyle_combo->insertItem(0, STRING_COMBOX_SHADOWSHAPE);
	m_drawStyle_combo->insertItem(1, STRING_COMBOX_LINESHAPE);
	m_drawStyle_combo->insertItem(2, STRING_COMBOX_SCATERSHAPE);
	m_Normal_TreeWidget->setItemWidget(m_drawStyle_treeitem, 1, m_drawStyle_combo);

	m_opacity_treeitem = new QTreeWidgetItem(m_renderInfo_treeitem, QStringList(STRING_ITEM_OPACITY));
	m_opacity_t_edit = new QLineEdit;
	m_opacity_t_edit->setText( "1.0");
	m_Normal_TreeWidget->setItemWidget(m_opacity_treeitem, 1, m_opacity_t_edit);

	m_overlap_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_OVERLAP));
	m_showCover_treeitem = new QTreeWidgetItem(m_overlap_treeitem, QStringList(STRING_ITEM_OVERLAP));
	//m_showCover_treeitem->setText(1, "true");
	m_showCover_combo = new QComboBox;
	m_showCover_combo->insertItem(0, STRING_COMBOX_FALSE);
	m_showCover_combo->insertItem(1, STRING_COMBOX_TRUE);
	m_Normal_TreeWidget->setItemWidget(m_showCover_treeitem, 1, m_showCover_combo);

	m_modulation_tree_item = new QTreeWidgetItem(m_overlap_treeitem, QStringList(STRING_ITEM_COLORMODUL));
	m_modulation_combo = new QComboBox;
	m_modulation_combo->insertItem(0, STRING_COMBOX_OVERSURFACE);
	m_modulation_combo->insertItem(1, STRING_COMBOX_OVERONLY);
	m_modulation_combo->insertItem(2, STRING_COMBOX_BLENDCLOLR);
	m_modulation_combo->insertItem(3, STRING_COMBOX_COLORMAP);
	m_Normal_TreeWidget->setItemWidget(m_modulation_tree_item, 1, m_modulation_combo);

	m_suitedType_tree_item = new QTreeWidgetItem(m_overlap_treeitem, QStringList(STRING_ITEM_SUITEDTYPE));
//	m_suitedType_tree_item->setText(1, "Hello");
	m_suitedType_combo = new QComboBox;
	m_suitedType_combo->insertItem(0, STRING_COMBOX_STRETCHSUR);
	m_suitedType_combo->insertItem(1, STRING_COMBOX_OVERONLY);
	m_Normal_TreeWidget->setItemWidget(m_suitedType_tree_item, 1, m_suitedType_combo);

	m_textureFile_treeitem = new QTreeWidgetItem(m_normal_property_treeitem, QStringList(STRING_ITEM_TEXTUREFILE));
	m_textureFile_edit = new QLineEdit;
	m_textureFile_edit->setText("2.jpg");
//	m_Normal_TreeWidget->setItemWidget(m_textureFile_treeitem, 1, m_textureFile_edit);
	m_file_button = new QPushButton;
	m_file_button->setText("...");

	QWidget *file_widget = new QWidget;
	QHBoxLayout *file_layout = new QHBoxLayout;
	file_layout->addWidget(m_textureFile_edit);
	file_layout->addWidget(m_file_button);
	file_widget->setLayout(file_layout);
	m_Normal_TreeWidget->setItemWidget(m_textureFile_treeitem, 1, file_widget);

	m_Normal_TreeWidget->expandAll();

	if (!data) {
		GHighLowMapNode::getDefault(baseStruct);
		data = (G_HighLowZoneMap*)baseStruct;
	}
	{
		m_input_treeitem->setText(1, QString::fromStdString(data->m_putIn));

	}

	m_form1_tab = new QTabWidget;
	m_form1_tab->setStyleSheet("QTreeWidget::item:!has-children{border:1px solid; color:black}");
	m_form1_tab->addTab(m_Normal_TreeWidget, QString(STRING_TAB_GENERAL));
	
	this->NodeType = STRING_GTYPE_HIGH_LOW;
	m_can_append_widget.push_back(STRING_WIDGET_AXIS);

	//connect(m_directtion_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(m_update_widget_property()));
	//connect(m_sectionId_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property()));
	connect(m_scaleFactor_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property()));
	connect(m_drawStyle_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_opacity_t_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property() ) );
	connect(m_showCover_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_modulation_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_suitedType_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(m_update_widget_property()));
	connect(m_textureFile_edit, SIGNAL(textChanged(QString)), this, SLOT(m_update_widget_property()));
	connect(m_file_button, SIGNAL(clicked(bool)), this, SLOT(slot_select_file()));
}

highlowmap_property_widget::~highlowmap_property_widget()
{
	delete m_form1_tab;
}

void highlowmap_property_widget::paintEvent(QPaintEvent *e)
{
	QPolygon polygon = QPolygon();
	polygon << QPoint(0, 15) << QPoint(10, 5) << QPoint(10, 0) << QPoint(this->width() - 10, 0);
	polygon << QPoint(this->width() - 10, 5) << QPoint(this->width(), 15) << QPoint(this->width() - 10, 25) << QPoint(this->width() - 10, 30);
	polygon << QPoint(10, 30) << QPoint(10, 25);
	setMask(polygon);
}

void highlowmap_property_widget::contextMenuEvent(QContextMenuEvent  *event){
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
	menu->addAction(STRING_ACTION_DELETE, receiver1, SLOT(delete_highlowmap()));
	menu->move(cursor().pos());
	menu->show();
}

void highlowmap_property_widget::delete_self()
{
	emit sig_delete_highlowmap();
}

G_HighLowZoneMap* highlowmap_property_widget::return_highlowmap_form_property_struct()
{
	getBaseStruct();

	//m_return_border_property_struct->m_direction = (CutPlaneDirection)m_directtion_combo->currentIndex();// CutPlaneDirection::D_XY;
	//m_return_border_property_struct->m_setionId = m_sectionId_edit->text().toInt();// 5;
	m_return_border_property_struct->m_scaleFactor = m_scaleFactor_edit->text().toFloat();// 1.0;
	m_return_border_property_struct->m_renderInfo.m_drawStyle = (child_draw_style)m_drawStyle_combo->currentIndex();//::Shadow_shape;
	m_return_border_property_struct->m_renderInfo.m_opacity = m_opacity_t_edit->text().toFloat();
	m_return_border_property_struct->m_mapOverlapped.m_showCover = m_showCover_combo->currentIndex();// true;
	m_return_border_property_struct->m_mapOverlapped.m_modulation = (Child_Color_Modulation)m_modulation_combo->currentIndex();// ::BlendSurfaceandOverlayColors;
	m_return_border_property_struct->m_mapOverlapped.m_suitedType = (Child_Suited_type)m_suitedType_combo->currentIndex(); //::Stretching_On_Surface;
	//m_return_border_property_struct->demFile = ;
	m_return_border_property_struct->textureFile = m_textureFile_edit->text().toStdString(); //"2.jpg"; // 
	return m_return_border_property_struct;
}

void highlowmap_property_widget::m_update_widget_property()
{
	G_HighLowZoneMap* pTemp = this->return_highlowmap_form_property_struct();
	std::shared_ptr<G_HighLowZoneMap>ptr(std::make_shared<G_HighLowZoneMap>());
	*(ptr.get()) = *pTemp;
	OperationalLayer::getOperationInstance().modifyNodeIntoProject(this->id_key, ptr);
	emit sig_vtk_camera_render();
}

void highlowmap_property_widget::slot_m_linewidth_slider_setValue(int a)
{
	disconnect(m_LineWidth_slider, 0, 0, 0);
	m_LineWidth_slider->setValue(a);
	m_update_widget_property();
	connect(m_LineWidth_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_spin_setValue(int)));
}

void highlowmap_property_widget::slot_m_linewidth_spin_setValue(int a)
{
	disconnect(m_LineWidth_spin, 0, 0, 0);
	m_LineWidth_spin->setValue(a);
	m_update_widget_property();
	connect(m_LineWidth_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_m_linewidth_slider_setValue(int)));
}

void highlowmap_property_widget::slot_change_border_color()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_show_note()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_note_font()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_font_slider_setvalue(int a)
{
	disconnect(m_FontSize_slider, 0, 0, 0);
	m_FontSize_slider->setValue(a);
	m_update_widget_property();
	connect(m_FontSize_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_font_spin_setvalue(int)));
}

void highlowmap_property_widget::slot_font_spin_setvalue(int a)
{
	disconnect(m_FontSize_spin, 0, 0, 0);
	m_FontSize_spin->setValue(a);
	m_update_widget_property();
	connect(m_FontSize_spin, SIGNAL(valueChanged(int)), this, SLOT(slot_font_slider_setvalue(int)));
}

void highlowmap_property_widget::slot_note_color_change()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_note_type_change()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_note_significant_change()
{
	m_update_widget_property();
}

void highlowmap_property_widget::slot_border_to_axis()
{
	emit sig_border_to_axis();
}

void highlowmap_property_widget::slot_select_file()
{
	QString file_name = QFileDialog::getOpenFileName(this, STRING_OPEN_TEXTUREFILE, ".", "filter (*.*)");
	if (file_name.isEmpty())
		return;
	m_textureFile_edit->setText(file_name);
}
