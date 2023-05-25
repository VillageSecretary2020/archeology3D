#include "threeDdetectSystem.h"
#include <QStyleFactory.h>

#include "axis_property_widget.h"
#include "border_property_widget.h"
#include "curve_slice_property_widget.h"
#include "source_property_widget.h"
#include "slice_property_widget.h"
#include "shape_render_property_widget.h"
#include "scatter_property_widget.h"
#include "text_property_widget.h"
#include "orthogonal_property_widget.h"
#include "annotation_property_widget.h"
#include "vector_property_widget.h"
#include "isoline_property_widget.h"
#include "isosurface_property_widget.h"
#include "surface_render_property_widget.h"
#include "transform_property_widget.h"
#include "subset_property_widget.h"
#include "exclusion_filter_property_widget.h"
#include "gridding_property_widget.h"
#include "repeat_filter_property_widget.h"
#include "highlowmap_property_widget.h"
#include "dxf_property_widget.h"
#include "geotiff_property_widget.h"
#include "picture_property_widget.h"
#include "avilib/avilib.h"
//#include "vtkAutoInit.h" 
#include <QDateTime>

#define STRING_ITEM_VERSION		tr(u8"考古地理数据三维可视化处理系统\t \n\n版本 V1.0.1010\n版权所有：鼎方")

threeDdetectSystem::threeDdetectSystem(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	QWidget* p = takeCentralWidget();
	if (p)
	{
		delete p;
	}
	setWindowIcon(QIcon(ICON_PATH"mainimg//21.ico"));
	create_menu();
	create_toolbar();
	//create_moukuai();
	create_lianluo();
	create_shuxin();
	create_tab();
	create_camera_properties();//创建相机属性窗口
	create_CaptureVideo();//创建录屏窗口
	create_property_description();//qliu
	//m_status_bar = new QStatusBar();
		//->setStyleSheet("background-color:black;");
	QLabel *permanent = new QLabel(this);
	permanent->setText("permanent message");
	this->statusBar()->setStyleSheet("background-color:white;");
	this->statusBar()->addPermanentWidget(permanent);
	m_status_label = new QLabel(this);
	m_status_label->setStyleSheet("color:black;");
	this->statusBar()->addWidget(m_status_label);
	//m_status_bar->addPermanentWidget(permanent);
	connect(m_tab, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_removeSubTab(int)));
	//connect(treewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(create_diagram_from_file()));
//	connect(m_module_manager_treewidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(slot_ClickModuleManagerGenerateForm(QTreeWidgetItem*,int )));
//	renderWindow =vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//	renderer =vtkSmartPointer<vtkRenderer>::New();
	//初始化即新建一个项目
	slot_create_project();
	return;
	QString temp_path = QString("project") + QString::number(m_tab->count());
	int8_t projectid = OperationalLayer::getOperationInstance().newProject(temp_path.toStdString());
	new_project *temp_project = new new_project(nullptr);
	if(current_new_light == nullptr)
		current_new_light = vtkSmartPointer<vtkLight>::New();//添加一个光源
	temp_project->m_proj_id = projectid;
	OperationalLayer::getOperationInstance().setVtkRender(temp_project->m_vtkrender, projectid);
	OperationalLayer::getOperationInstance().setRenderWinInteractor(temp_project->m_vtkwindowinteractor->interactor(), projectid);
	m_contact_diagram_manager_dock->setWidget(temp_project->current_lianluotu);
	m_tab->addTab(temp_project, temp_path);
	int index = m_tab->count();
	m_tab->setCurrentIndex(index - 1);
	m_property_manager_dock->setWidget(NULL);
	connect(temp_project->current_lianluotu, SIGNAL(sig_show_property_manager_message(QString, base_property_widget*)), this, SLOT(slot_show_property_manager_message(QString, base_property_widget*)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_show_status_mess(QString)), this, SLOT(slot_status_mess(QString)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_load_data()), this, SLOT(slot_open_file()));	//create_diagram_from_file
	connect(temp_project->current_lianluotu, SIGNAL(sig_paste_widget(std::string)), this, SLOT(slot_paste_widget(std::string)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_text_widget()), this, SLOT(create_text()));
	connect(temp_project->current_lianluotu, SIGNAL(sig_comment_widget()), this, SLOT(create_annotation()));
	connect(temp_project->current_lianluotu, SIGNAL(sig_manager_property_null()), this, SLOT(slot_property_manager_property_null()));
}

void threeDdetectSystem::slot_status_mess(QString a)
{
	m_label->setText(a);
}

void threeDdetectSystem::slot_translation()// m_act[15]平移 (平移和旋转是2种操作方式，同时只能选其中一种)
{
	if (m_act[13]->isChecked())
	{
		m_act[13]->setChecked(false);
	}
	m_drag_mode = 1;
	slot_vtk_camera_render();
}

void threeDdetectSystem::slot_drag_actor()//m_act[13]旋转
{
	if (m_act[15]->isChecked())
	{
		m_act[15]->setChecked(false);
	}
	m_drag_mode = 0;
	slot_vtk_camera_render();
}

void threeDdetectSystem::slot_perspective()//m_act[18]透视 （透视和投影是2种操作方式，同时只能选其中一种)
{
	if (m_act[19]->isChecked())
	{
		m_act[19]->setChecked(false);
	}
	m_projection = false;
	slot_vtk_camera_render();
}

void threeDdetectSystem::slot_projection()//m_act[19]投影
{
	if (m_act[18]->isChecked())
	{
		m_act[18]->setChecked(false);
	}
	m_projection = true;
	slot_vtk_camera_render();
}

void threeDdetectSystem::slot_toolbutton_orientation()//设置相机视角，从上下左右前后观察actor
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	switch (m_direction)
	{
	case threeDdetectSystem::OrientationT:
		//注意，在设置相机位置时可能会出现视图平面法线是平行的。不用的视角要调用新的ViewUp();
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(0, 0, 1);//上
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 0, 0);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool29.ico"));
		break;
	case threeDdetectSystem::OrientationB:
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(0, 0, -1);//下
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, -1, 0);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool31.ico"));
		break;
	case threeDdetectSystem::OrientationR:
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(1, 0, 0);//右
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 0, 1);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool32.ico"));
		break;
	case threeDdetectSystem::OrientationL:
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(-1, 0, 0);//左
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 0, 1);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool34.ico"));
		break;
	case threeDdetectSystem::OrientationF:
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(0, -1, 0);//前
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 0, 1);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool30.ico"));
		break;
	case threeDdetectSystem::OrientationA:
		current_project->m_vtkrender->GetActiveCamera()->SetPosition(0, 1, 0);//后
		current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 0, 1);
		m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool33.ico"));
		break;
	default:
		break;
	}
	/*current_project->m_vtkrender->GetActiveCamera()->SetViewUp(0, 1, 0);*/
	current_project->m_vtkrender->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	current_project->m_vtkrender->ResetCamera();
	//current_project->repaint();// update();
	current_project->m_vtkrender->Render();
	//slot_vtk_camera_render();
}

void threeDdetectSystem::slot_Orientation_Top()//上
{
	m_direction = OrientationT;
	slot_toolbutton_orientation();
}

void threeDdetectSystem::slot_Orientation_Button()//下
{
	m_direction = OrientationB;
	slot_toolbutton_orientation();
}
void threeDdetectSystem::slot_Orientation_Left()//左
{
	m_direction = OrientationL;
	slot_toolbutton_orientation();
}
void threeDdetectSystem::slot_Orientation_Right()//右
{
	m_direction = OrientationR;
	slot_toolbutton_orientation();
}
void threeDdetectSystem::slot_Orientation_Front()//前
{
	m_direction = OrientationF;
	slot_toolbutton_orientation();
}
void threeDdetectSystem::slot_Orientation_Back()//后
{
	m_direction = OrientationA;
	slot_toolbutton_orientation();
}

void threeDdetectSystem::slot_lighting_switch(bool bool_switch)
{
	if (bool_switch)
	{
		m_isLight = 1;
	}
	else {
		m_isLight = 0;
	}
	
	slot_vtk_camera_render();
}

void threeDdetectSystem::slot_camera_target()
{
	if (m_camera_properties_dialog != nullptr)
	{
		double pos[3] = { 0,0,0 };
		new_project*current_project = (new_project*)m_tab->currentWidget();
		if (m_camera_properties_dialog->isHidden())
		{
			if (current_project)//先更新一下控件上的值
			{
				current_project->m_vtkrender->GetActiveCamera()->GetPosition(pos);
				m_lineE_camera_x->setText(QString("%1").arg(pos[0]));
				m_lineE_camera_y->setText(QString("%1").arg(pos[1]));
				m_lineE_camera_z->setText(QString("%1").arg(pos[2]));
				current_project->m_vtkrender->GetActiveCamera()->GetFocalPoint(pos);
				m_lineE_target_x->setText(QString("%1").arg(pos[0]));
				m_lineE_target_y->setText(QString("%1").arg(pos[1]));
				m_lineE_target_z->setText(QString("%1").arg(pos[2]));
			}
			int return_mode = m_camera_properties_dialog->exec();
			if (return_mode == QDialog::Accepted)
			{
				if (current_project)//OK时 更新camera的位置信息
				{
					pos[0] = m_lineE_camera_x->text().toDouble();
					pos[1] = m_lineE_camera_y->text().toDouble();
					pos[2] = m_lineE_camera_z->text().toDouble();
					current_project->m_vtkrender->GetActiveCamera()->SetPosition(pos);
					pos[0] = m_lineE_target_x->text().toDouble();
					pos[1] = m_lineE_target_y->text().toDouble();
					pos[2] = m_lineE_target_z->text().toDouble();
					current_project->m_vtkrender->GetActiveCamera()->SetFocalPoint(pos);
				}
			}
			if (return_mode == QDialog::Rejected)
			{
			}
		}
	}
}

void threeDdetectSystem::slot_capture_video()
{
	if (m_capture_video_dialog != nullptr)
	{
		if (m_capture_video_dialog->isHidden())
		{
			m_capture_video_dialog->show();
		}
	}
}

void threeDdetectSystem::slot_snapshot()
{
	QScreen *screen = QApplication::primaryScreen(); //获取当前程序的主窗口对象
	QWidget *current_widget = m_tab->currentWidget();
	QPixmap map = screen->grabWindow(current_widget->winId()); //调用主窗口对象的捕捉窗口图像，并传递窗口的id号
	QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
	clipboard->setPixmap(map);
	m_status_label->setText(STRING_TIP_SNAPSHOT);
}

void threeDdetectSystem::layerCallFun(void * ptr, int8_t projectId)
{
	((threeDdetectSystem*)ptr)->getProjectProp(projectId);
	
}

void threeDdetectSystem::getProjectProp(int8_t projectId)
{
	std::list<vtkSmartPointer<vtkProp>>list1=OperationalLayer::getOperationInstance().getProjectProp(projectId);
	new_project*current_project = (new_project*)m_tab->currentWidget();
	//current_project->actor_list
	std::list<vtkSmartPointer<vtkProp>>::iterator itor;
	std::list<vtkSmartPointer<vtkProp>>::iterator itor1;
	itor = list1.begin();
	while (itor != list1.end())
	{
		int flag = 0;
		itor1 = current_project->actor_list.begin();
		while (itor1 != current_project->actor_list.end())
		{
			if (*itor == *itor1)
			{
				flag = 1;
				break;
			}
			itor1++;
		}
		if (flag == 0)
		{
			current_project->actor_list.push_back(*itor);
			//current_project->m_vtkrender->AddActor(*itor); //渲染窗口添加actor
		}
		itor++;
	}

	itor1 = current_project->actor_list.begin();
	while (itor1 != current_project->actor_list.end())
	{
		int flag = 0;
		itor = list1.begin();
		int count = 0;
		while (itor != list1.end())
		{
			count++;
			if (*itor1 == *itor)
			{
				break;
			}
			if (count == list1.size())
			{
				if (*itor != *itor)
				{
					flag = 1;
					current_project->actor_list.erase(itor);
					//current_project->m_vtkrender->RemoveActor(*itor1); //渲染窗口移除actor
					break;
				}
			}
			
			itor++;
		}
		if (flag == 1)
		{
			itor1 = list1.begin();
		}
		else
			itor1++;
		
	}
	//current_project->m_vtkwindow->Render();
}

void threeDdetectSystem::slot_ClickModuleManagerGenerateForm(QTreeWidgetItem* item,int n)
{
	QTreeWidgetItem *parent = item->parent();
	if (!parent)
	{
		//printf("没有父节点\n");
		if (item->text(0) == STRING_DATA_SOURCE)	// STRING_IMPORT_MODULE
		{
			slot_open_file();// create_diagram_from_file();
		}
		else if (item->text(0) ==tr("example"))
			printf("范例\n");
		else if (item->text(0) == tr("graphic output"))
			printf("图形输出");
	}
	else 
	{
		int col = parent->indexOfChild(item);
		if (parent->text(0) ==tr("graphic output"))
		{
			if (col == 0)
				create_slice();
			else if (col == 1)
				create_shape_render();
			else if (col == 2)
				create_scatter();
			//OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project)m_tab->currentWidget()).get_proj_id,);
			else if (col == 3)
				create_text();
			else if (col == 4)
				create_orthogonal();
			else if (col == 5)
				create_annotation();
			else if (col == 6)
				create_vector();
			else if (col == 7)
				create_isoline();
			else if (col == 8)
				create_isosurface();
			else if (col == 9)
				create_surface_render();
			else if (col == 10)
				create_axis();
			else if (col == 11)
				create_border();
		}

		else if (parent->text(0) ==tr("normal module"))
		{
			if (col == 0)
				//	create_mess();
				;
			else if (col == 1)
				//create_light();
				;
		}
		else if (parent->text(0) == STRING_DATA_SOURCE)	//tr("data source")
		{
			if (col == 0)
				//create_function_grid();
				;
			else if (col == 1)
				//create_test_grid();
				;
		}
		else if (parent->text(0) ==tr("shaft"))
		{
			if (col == 0)
				//create_shaft_data();
				;
		}
		else if (parent->text(0) == STRING_CALC_MENU)
		{
			 if (col == 0)
				create_transform();
			else if (col == 1)
				create_subset();
			else if (col == 2)
				create_exclusion_filter();
			else if (col == 3)
				create_gridding();
			else if (col == 4)
				create_repeat_filter();
		}
	}
}

void threeDdetectSystem::create_menu(){
	file = menuBar()->addMenu(STRING_MENU_FILE);
	view = menuBar()->addMenu(STRING_MENU_VIEW);
	calculate = menuBar()->addMenu(STRING_CALC_MENU);
	render = menuBar()->addMenu(STRING_MENU_RENDER);
	help = menuBar()->addMenu(STRING_MENU_HELP);
	//file1[0] = new QAction(QIcon("://threeDdetectSystem//icon//menu_img/menu1.ico"), tr("open"), this);
	file1[1] = new QAction(STRING_ACTION_CLOSE, this);// QIcon(ICON_PATH"menu_img//menu2.ico"), 
	file1[2] = new QAction(STRING_ACTION_CLOSEALL, this);
	new1 = file->addMenu(STRING_MENU_NEW);
	new1->addAction(STRING_ACTION_PROJ, this, SLOT(slot_create_project()));
	//new1->addAction("work_sheet", this, SLOT(slot_create_table()));
	QMenu *open_1 = file->addMenu(QIcon(ICON_PATH"menu_img/menu1.ico"), STRING_MENU_OPEN);
	open_1->addAction(STRING_ACTION_OPENFILE, this, SLOT(slot_open_file()));
	open_1->addAction(STRING_ACTION_OPENPROJECT, this, SLOT(slot_open_project()));
	file->addAction(file1[1]);
	file->addAction(file1[2]);
	file1[3] = new QAction(QIcon(ICON_PATH"menu_img//menu2.ico"), STRING_ACTION_SAVE, this);
	file->addAction(file1[3]);
	file1[4] = new QAction(STRING_ACTION_SAVEAS, this);
	file->addAction(file1[4]);
	file1[5] = new QAction(STRING_ACTION_SAVEALL, this);
	file->addAction(file1[5]);

	view1[0] = new QAction(STRING_ACTION_ADOPTWIN, this);
	view->addAction(view1[0]);
	connect(view1[0], SIGNAL(triggered()), this, SLOT(slot_vtk_camera_render()));

	view1[1] = new QAction(STRING_ACTION_ENLARGE, this);
	//view->addAction(view1[1]);
	view1[2] = new QAction(STRING_ACTION_NARROW, this);
	//view->addAction(view1[2]);
	view1[3] = new QAction(STRING_ACTION_TRANSMODE, this);
	view->addAction(view1[3]);
	connect(view1[3], SIGNAL(triggered(bool)), this, SLOT(slot_translation()));

	view1[4] = new QAction(STRING_ACTION_ROTATEMODE, this);
	view->addAction(view1[4]);
	connect(view1[4], SIGNAL(triggered(bool)), this, SLOT(slot_drag_actor()));

	view1[5] = new QAction(STRING_ACTION_AXES, this);
	view->addAction(view1[5]);
	connect(view1[5], SIGNAL(triggered()), this, SLOT(slot_camera_target()));

	QMenu *six_view = view->addMenu(STRING_MENU_SIXVIEW);
	six_view->addAction(STRING_ACTION_VIEW_FRONT, this, SLOT(slot_Orientation_Front()));
	six_view->addAction(STRING_ACTION_VIEW_BACK, this, SLOT(slot_Orientation_Back()));
	six_view->addAction(STRING_ACTION_VIEW_LEFT, this, SLOT(slot_Orientation_Left()));
	six_view->addAction(STRING_ACTION_VIEW_RIGHT, this, SLOT(slot_Orientation_Right()));
	six_view->addAction(STRING_ACTION_VIEW_TOP, this, SLOT(slot_Orientation_Top()));
	six_view->addAction(STRING_ACTION_VIEW_BOTTOM, this, SLOT(slot_Orientation_Button()));
	view1[6] = new QAction(STRING_ACTION_FOREGLIGHT, this);
	QMenu *manager = view->addMenu(STRING_MENU_MANAGER);
	view1[7] = new QAction(QIcon(ICON_PATH"menu_img//lianluotu.ico"), STRING_ACTION_LIANMANAGE, this);
	view1[8] = new QAction(STRING_ACTION_LIANMANAGE, this);
	manager->addAction(view1[7]);
	manager->addAction(view1[8]);
	view1[8]->setVisible(false);
	view1[9] = new QAction(QIcon(ICON_PATH"menu_img//lianluotu.ico"), STRING_ACTION_PROMANAGE, this);
	view1[10] = new QAction(STRING_ACTION_PROMANAGE, this);
	manager->addAction(view1[9]);
	manager->addAction(view1[10]);
	view1[10]->setVisible(false);
	view1[11] = new QAction(QIcon(ICON_PATH"menu_img//lianluotu.ico"), STRING_ACTION_MODMANAGE, this);
	view1[12] = new QAction(STRING_ACTION_MODMANAGE, this);
	//manager->addAction(view1[11]);
	manager->addAction(view1[12]);
	view1[12]->setVisible(false);
	calculate_menu[0] = new QAction(STRING_ACTION_TRANSFORM, this);
	calculate->addAction(calculate_menu[0]);
	calculate_menu[1] = new QAction(STRING_ACTION_EXCLUFILTER, this);
	calculate->addAction(calculate_menu[1]);
	calculate_menu[2] = new QAction(STRING_ACTION_REPEFILTER, this);
	calculate->addAction(calculate_menu[2]);
	calculate_menu[3] = new QAction(STRING_ACTION_SUBSET, this);
	calculate->addAction(calculate_menu[3]);
	calculate_menu[4] = new QAction(STRING_ACTION_MESH, this);
	calculate->addAction(calculate_menu[4]);

	connect(calculate_menu[0], SIGNAL(triggered(bool)), this, SLOT(create_transform()));
	connect(calculate_menu[1], SIGNAL(triggered(bool)), this, SLOT(create_exclusion_filter()));
	connect(calculate_menu[2], SIGNAL(triggered(bool)), this, SLOT(create_repeat_filter()));
	connect(calculate_menu[3], SIGNAL(triggered(bool)), this,SLOT(create_subset()));
	connect(calculate_menu[4], SIGNAL(triggered(bool)), this, SLOT(create_gridding()));

	render_menu[0] = new QAction(STRING_ACTION_AXIS, this);
	connect(render_menu[0], SIGNAL(triggered(bool)), this, SLOT(create_axis()));
	render->addAction(render_menu[0]);
	render_menu[1] = new QAction(STRING_ACTION_BOUNDBOX, this);
	connect(render_menu[1], SIGNAL(triggered(bool)), this, SLOT(create_border()));
	render->addAction(render_menu[1]);
	render_menu[2] = new QAction(STRING_ACTION_TEXT, this);
	connect(render_menu[2], SIGNAL(triggered(bool)), this, SLOT(create_text()));
	render->addAction(render_menu[2]);
	render_menu[3] = new QAction(STRING_ACTION_ANNOTATION, this);
	connect(render_menu[3], SIGNAL(triggered(bool)), this, SLOT(create_annotation()));
	render->addAction(render_menu[3]);
	render_menu[4] = new QAction(STRING_ACTION_SLICE, this);
	connect(render_menu[4], SIGNAL(triggered(bool)), this, SLOT(create_slice()));
	//render->addAction(render_menu[4]);
	//render_menu[5] = new QAction(tr("cut"), this);
	render_menu[5] = new QAction(STRING_ACTION_PICTURE, this);
	connect(render_menu[5], SIGNAL(triggered(bool)), this, SLOT(create_picture()));
	render->addAction(render_menu[5]);
	render_menu[6] = new QAction(STRING_ACTION_SHAPRENDER, this);
	connect(render_menu[6], SIGNAL(triggered(bool)), this, SLOT(create_shape_render()));
	render->addAction(render_menu[6]);
	render_menu[7] = new QAction(STRING_ACTION_SCATTER, this);
	connect(render_menu[7], SIGNAL(triggered(bool)), this, SLOT(create_scatter()));
	render->addAction(render_menu[7]);
	render_menu[8] = new QAction(STRING_ACTION_ORTHOGRAPH, this);
	connect(render_menu[8], SIGNAL(triggered(bool)), this, SLOT(create_orthogonal()));
	render->addAction(render_menu[8]);

	render_menu[9] = new QAction(STRING_ACTION_ISOLINEDIAG, this);
	connect(render_menu[9], SIGNAL(triggered(bool)), this, SLOT(create_isoline()));
	render->addAction(render_menu[9]);

	render_menu[10] = new QAction(STRING_ACTION_ISOSURDIAG, this);
	connect(render_menu[10], SIGNAL(triggered(bool)), this, SLOT(create_isosurface()));
	render->addAction(render_menu[10]);

	render_menu[11] = new QAction(STRING_ACTION_SUFARENDER, this);
	connect(render_menu[11], SIGNAL(triggered(bool)), this, SLOT(create_surface_render()));
	render->addAction(render_menu[11]);

	render_menu[12] = new QAction(STRING_ACTION_HIANLOREA, this);
	render->addAction(render_menu[12]);

	render_menu[13] = new QAction(STRING_ACTION_LIGHTING, this);
	render_menu[13]->setCheckable(true);
	render->addAction(render_menu[13]);

	help_menu[0] = new QAction(STRING_ACTION_INTRO, this);
	help->addAction(help_menu[0]);

	help_menu[1] = new QAction(STRING_ACTION_ABOUT, this);
	help->addAction(help_menu[1]);

	connect(file1[3], SIGNAL(triggered(bool)), this, SLOT(slot_save_current_project()));
	connect(file1[5], SIGNAL(triggered(bool)), this, SLOT(slot_save_all_project()));
	connect(file1[1], SIGNAL(triggered(bool)), this, SLOT(slot_close_project()));
	connect(file1[2], SIGNAL(triggered(bool)), this, SLOT(slot_close_all_project()));
	connect(file1[4], SIGNAL(triggered(bool)), this, SLOT(slot_save_as_project()));
	connect(view1[7], SIGNAL(triggered(bool)), this, SLOT(slot_lianluotu_manager_not_show()));
	connect(view1[8], SIGNAL(triggered(bool)), this, SLOT(slot_lianluotu_manager_show()));
	connect(view1[9], SIGNAL(triggered(bool)), this, SLOT(slot_property_manager_not_show()));
	connect(view1[10], SIGNAL(triggered(bool)), this, SLOT(slot_property_manager_show()));
	connect(view1[11], SIGNAL(triggered(bool)), this, SLOT(slot_module_manager_not_show()));
	connect(view1[12], SIGNAL(triggered(bool)), this, SLOT(slot_module_manager_show()));
	connect(view1[13], SIGNAL(triggered(bool)), this, SLOT(slot_lighting_switch(bool)));

	connect(help_menu[1], SIGNAL(triggered()), this, SLOT(slot_menu_about()));
}

void threeDdetectSystem::slot_menu_about()
{
	QMessageBox::information(NULL, STRING_ITEM_ABOUT, STRING_ITEM_VERSION);
}

void threeDdetectSystem::slot_lianluotu_manager_not_show()
{
	m_contact_diagram_manager_dock->setVisible(false);
	view1[7]->setVisible(false);
	view1[8]->setVisible(true);
}

void threeDdetectSystem::slot_lianluotu_manager_show()
{
	m_contact_diagram_manager_dock->setVisible(true);
	view1[8]->setVisible(false);
	view1[7]->setVisible(true);
}

void threeDdetectSystem::slot_property_manager_not_show()
{
	m_property_manager_dock->setVisible(false);
	view1[9]->setVisible(false);
	view1[10]->setVisible(true);
}

void threeDdetectSystem::slot_property_manager_show()
{
	m_property_manager_dock->setVisible(true);
	view1[10]->setVisible(false);
	view1[9]->setVisible(true);
}

void threeDdetectSystem::slot_module_manager_not_show()
{
	m_module_manager_dock->setVisible(false);
	view1[11]->setVisible(false);
	view1[12]->setVisible(true);
}

void threeDdetectSystem::slot_module_manager_show()
{
	m_module_manager_dock->setVisible(true);
	view1[12]->setVisible(false);
	view1[11]->setVisible(true);
}

void threeDdetectSystem::create_toolbar(){
	fileToolBar = addToolBar(tr("file"));
	m_act[0] = new QAction(QIcon(ICON_PATH"tool_img//tool1.ico"), STRING_ACTION_PROJ, this);
	fileToolBar->addAction(m_act[0]);
	connect(m_act[0], SIGNAL(triggered()), this, SLOT(slot_create_project()));

	m_act[1] = new QAction(QIcon(ICON_PATH"tool_img//tool2.ico"), STRING_ACTION_TABEL, this);	// yzw 0614 tr("new")STRING_DATA_SOURCE
//	fileToolBar->addAction(m_act[1]);

	m_act[2] = new QAction(QIcon(ICON_PATH"tool_img//tool3.ico"), STRING_ACTION_OPENPROJECT, this);
	fileToolBar->addAction(m_act[2]);
	connect(m_act[2], SIGNAL(triggered()), this, SLOT(slot_open_project()));

	m_act[3] = new QAction(QIcon(ICON_PATH"tool_img//tool4.ico"), STRING_ACTION_SAVE, this);
	fileToolBar->addAction(m_act[3]);
	connect(m_act[3], SIGNAL(triggered()), this, SLOT(slot_save_current_project()));

//	fileToolBar->addAction(m_act[1]);

	m_act[4] = new QAction(QIcon(ICON_PATH"tool_img//tool5.ico"), STRING_ACTION_IMPORT, this);
	fileToolBar->addAction(m_act[4]);
	connect(m_act[4], SIGNAL(triggered()), this, SLOT(slot_open_file()));

	m_act[5] = new QAction(QIcon(ICON_PATH"tool_img//tool6.ico"), STRING_ACTION_EXPORT, this);
	fileToolBar->addAction(m_act[5]);
	connect(m_act[5], SIGNAL(triggered()), this, SLOT(slot_save_data2()));

	m_act[6] = new QAction(QIcon(ICON_PATH"tool_img//tool7.ico"), tr("print"), this);
	//fileToolBar->addAction(m_act[6]);

	m_act[7] = new QAction(QIcon(ICON_PATH"tool_img//tool8.ico"), STRING_ACTION_COPY, this);
	fileToolBar->addAction(m_act[7]);
	connect(m_act[7], SIGNAL(triggered()), this, SLOT(slot_copy_node2()));

	m_act[8] = new QAction(QIcon(ICON_PATH"tool_img//tool9.ico"), STRING_ACTION_PASTE, this);
	fileToolBar->addAction(m_act[8]);
	connect(m_act[8], SIGNAL(triggered()), this, SLOT(slot_paste_widget2()));

	m_act[9] = new QAction(QIcon(ICON_PATH"tool_img//tool10.ico"), STRING_ACTION_DELETE, this);
	fileToolBar->addAction(m_act[9]);
	connect(m_act[9], SIGNAL(triggered()), this, SLOT(slot_delete_node()));

	m_act[10] = new QAction(QIcon(ICON_PATH"tool_img//tool11.ico"), tr("revoke"), this);  // revoke: 撤销
	//fileToolBar->addAction(m_act[10]);
	m_act[11] = new QAction(QIcon(ICON_PATH"tool_img//tool12.ico"), tr("redo"), this);   // redo: 重做
	//fileToolBar->addAction(m_act[11]);
	m_act[12] = new QAction(QIcon(ICON_PATH"tool_img//tool13.ico"), tr("help"), this);
	//fileToolBar->addAction(m_act[12]);
	m_act[13] = new QAction(QIcon(ICON_PATH"tool_img//tool14.ico"), STRING_ACTION_ROTATEMODE, this);  //track:轨迹
	fileToolBar->addAction(m_act[13]);
	m_act[14] = new QAction(QIcon(ICON_PATH"tool_img//tool15.ico"), tr("zoom"), this);   //zoom:缩放
	//fileToolBar->addAction(m_act[14]);
	m_act[15] = new QAction(QIcon(ICON_PATH"tool_img//tool16.ico"), STRING_ACTION_TRANSMODE, this);  //translation:平移
	//m_act[15]->setCheckable(true);
	fileToolBar->addAction(m_act[15]);
	m_act[16] = new QAction(QIcon(ICON_PATH"tool_img//tool17.ico"), STRING_ACTION_ADOPTWIN, this); //adapt:适应
	fileToolBar->addAction(m_act[16]);
	m_act[17] = new QAction(QIcon(ICON_PATH"tool_img//tool18.ico"), tr("start"), this); //satrt:起始
	//fileToolBar->addAction(m_act[17]);
	m_act[18] = new QAction(QIcon(ICON_PATH"tool_img//tool19.ico"), STRING_ITEM_PERCTIVE, this); //perspective:透视
//	m_act[18]->setCheckable(true);
	fileToolBar->addAction(m_act[18]);
	m_act[19] = new QAction(QIcon(ICON_PATH"tool_img//tool20.ico"), STRING_ITEM_PROJECTI, this); //projection:投影
	fileToolBar->addAction(m_act[19]);
	//观察方向
	m_act[20] = new QAction(QIcon(ICON_PATH"tool_img//tool30.ico"), STRING_ACTION_VIEW_FRONT, this);	//Front:前
	m_act[28] = new QAction(QIcon(ICON_PATH"tool_img//tool33.ico"), STRING_ACTION_VIEW_BACK, this);	//Back:后
	m_act[29] = new QAction(QIcon(ICON_PATH"tool_img//tool34.ico"), STRING_ACTION_VIEW_LEFT, this);	//Left:左
	m_act[30] = new QAction(QIcon(ICON_PATH"tool_img//tool32.ico"), STRING_ACTION_VIEW_RIGHT, this);	//Right:右
	m_act[31] = new QAction(QIcon(ICON_PATH"tool_img//tool29.ico"), STRING_ACTION_VIEW_TOP, this);	//Top:上面
	m_act[32] = new QAction(QIcon(ICON_PATH"tool_img//tool31.ico"), STRING_ACTION_VIEW_BOTTOM, this);//Bottom:下面

	m_toolButton_Orientation = new QToolButton();
	QMenu* menuOrientation = new QMenu();
	menuOrientation->addAction(m_act[20]);//添加方向
	menuOrientation->addAction(m_act[28]);
	menuOrientation->addAction(m_act[29]);
	menuOrientation->addAction(m_act[30]);
	menuOrientation->addAction(m_act[31]);
	menuOrientation->addAction(m_act[32]);
	m_toolButton_Orientation->setIcon(QIcon(ICON_PATH"tool_img//tool30.ico"));
	//todo 各个方向的图标是不同的需要加载不同的icon 在各个方向的槽函数里要动态加载当前选中方向的icon
	m_toolButton_Orientation->setPopupMode(QToolButton::MenuButtonPopup);
	m_toolButton_Orientation->setMenu(menuOrientation);
	fileToolBar->addWidget(m_toolButton_Orientation);

	m_act[21] = new QAction(QIcon(ICON_PATH"tool_img//tool22.ico"), STRING_ACTION_SNAPSHOT, this);//snapshot:快照
	fileToolBar->addAction(m_act[21]);
	m_act[22] = new QAction(QIcon(ICON_PATH"tool_img//tool23.ico"), STRING_ACTION_VIDEO, this);//video:视频
	fileToolBar->addAction(m_act[22]);
	m_act[23] = new QAction(QIcon(ICON_PATH"tool_img//tool24.ico"), STRING_ITEM_RESET, this);//reset:复位
	//fileToolBar->addAction(m_act[23]);
	m_act[24] = new QAction(QIcon(ICON_PATH"tool_img//tool25.ico"), STRING_ITEM_START, this);//start：起始
	//fileToolBar->addAction(m_act[24]);
	m_act[25] = new QAction(QIcon(ICON_PATH"tool_img//tool26.ico"), STRING_ACTION_CAMERA, this);//camera:摄像
	fileToolBar->addAction(m_act[25]);
	m_act[26] = new QAction(QIcon(ICON_PATH"tool_img//tool27.ico"), STRING_ACTION_AXES, this);//coordinate:坐标
	fileToolBar->addAction(m_act[26]);
	m_act[27] = new QAction(QIcon(ICON_PATH"tool_img//tool28.ico"), STRING_ACTION_LIGHTING, this);//lighting:照明
	fileToolBar->addAction(m_act[27]);

	//实现工具栏按钮的功能，需要设置按钮熟悉以及绑定槽
	//操作模式  m_act[13]旋转  m_act[15]平移
	m_act[13]->setCheckable(true);
	m_act[13]->setChecked(true);//默认打开旋转模式
	m_act[15]->setCheckable(true);
	m_act[15]->setChecked(false);
	//观察模式  m_act[18]透视  m_act[19]投影
	m_act[18]->setCheckable(true);
	m_act[18]->setChecked(true);//默认是以透视方式观察
	m_act[19]->setCheckable(true);
	m_act[19]->setChecked(false);
	//窗口坐标的显示
	m_act[26]->setCheckable(true);
	m_act[26]->setChecked(true);
	//照明开关
	m_act[27]->setCheckable(true);
	m_act[27]->setChecked(true);
	connect_toolbar();
}

void threeDdetectSystem::connect_toolbar()
{
	connect(m_act[13], SIGNAL(triggered(bool)), this, SLOT(slot_drag_actor()));
	connect(m_act[15], SIGNAL(triggered(bool)), this, SLOT(slot_translation()));
	connect(m_act[16], SIGNAL(triggered()), this, SLOT(slot_vtk_adapt_win()));
	connect(m_act[18], SIGNAL(triggered(bool)), this, SLOT(slot_perspective()));
	connect(m_act[19], SIGNAL(triggered(bool)), this, SLOT(slot_projection()));
	connect(m_act[27], SIGNAL(triggered(bool)), this, SLOT(slot_lighting_switch(bool)));
	connect(m_act[25], SIGNAL(triggered()), this, SLOT(slot_camera_target()));
	connect(m_act[21], SIGNAL(triggered()), this, SLOT(slot_snapshot()));
	connect(m_act[22], SIGNAL(triggered()), this, SLOT(slot_capture_video()));
	connect(m_act[26], &QAction::triggered, this, [&](bool visib) { 
		new_project*current_project = (new_project*)m_tab->currentWidget();
		current_project->axes->SetVisibility(visib);
		slot_vtk_camera_render();
	});

	//QToolButton有一个信号  void QToolButton::triggered(QAction *action)
	connect(m_toolButton_Orientation, SIGNAL(clicked()), this, SLOT(slot_toolbutton_orientation()));
	connect(m_act[20], SIGNAL(triggered()), this, SLOT(slot_Orientation_Front()));
	connect(m_act[28], SIGNAL(triggered()), this, SLOT(slot_Orientation_Back()));
	connect(m_act[29], SIGNAL(triggered()), this, SLOT(slot_Orientation_Left()));
	connect(m_act[30], SIGNAL(triggered()), this, SLOT(slot_Orientation_Right()));
	connect(m_act[31], SIGNAL(triggered()), this, SLOT(slot_Orientation_Top()));
	connect(m_act[32], SIGNAL(triggered()), this, SLOT(slot_Orientation_Button()));
}

void threeDdetectSystem::create_moukuai() 
{
	m_module_manager_dock = new QDockWidget(STRING_DOCK_MODULE, this);//
	//m_module_manager_dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	m_module_manager_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_module_manager_dock->setAllowedAreas(Qt::NoDockWidgetArea);
	// dock->setAllowedAreas(Qt::AllDockW);
	addDockWidget(Qt::LeftDockWidgetArea, m_module_manager_dock);
	m_module_manager_treewidget = new QTreeWidget(m_module_manager_dock);
	m_module_manager_treewidget->setStyle(QStyleFactory::create("windows"));
	m_module_manager_treewidget->setColumnCount(1);
	m_module_manager_treewidget->setHeaderLabel(tr("item tree"));
	m_module_manager_import = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(STRING_IMPORT_MODULE));
	QIcon load1(ICON_PATH"mainimg//1.ico");
	m_module_manager_import->setIcon(0, load1);
	QIcon  icon(ICON_PATH"mainimg//2.ico");
	m_module_manager_example = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(tr("example")));
	m_module_manager_example->setIcon(0, icon);
	m_module_manager_graphics_output = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(tr("graphic output")));
	m_module_manager_graphics_output->setIcon(0, icon);
	m_module_manager_convention_module = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(tr("normal module")));
	m_module_manager_convention_module->setIcon(0, icon);
	m_module_manager_data_source=new QTreeWidgetItem(m_module_manager_treewidget,QStringList(STRING_DATA_SOURCE));	//tr("data source")
	m_module_manager_shaft = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(tr("shaft")));
	m_module_manager_shaft->setIcon(0, icon);
	m_module_manager_calculation = new QTreeWidgetItem(m_module_manager_treewidget, QStringList(STRING_CALC_MENU));
	m_module_manager_calculation->setIcon(0, icon);
	items.append(m_module_manager_import);
	items.append(m_module_manager_example);
	items.append(m_module_manager_graphics_output);
	items.append(m_module_manager_convention_module);
	items.append(m_module_manager_data_source);
	m_module_manager_treewidget->insertTopLevelItem(0, m_module_manager_import);
	m_module_manager_treewidget->insertTopLevelItems(1, items);
	m_fIteam1a = new QTreeWidgetItem(m_module_manager_example, QStringList(QString("fla")));
	m_fIteam1b = new QTreeWidgetItem(m_module_manager_example, QStringList(QString("flb")));
	m_module_manager_example->addChild(m_fIteam1a);
	m_module_manager_example->addChild(m_fIteam1b);
	//图形输出子节点
	m_out[0] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("slice diagram")));
	//m_out[1] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("clipboard")));
	m_out[2] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("shape rendering")));
	m_out[3] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("scatter diagram")));
	m_out[4] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("text")));
	m_out[5] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("orthogonal graph")));
	m_out[6] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("annotation")));
	//m_out[7] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("streamline diagram")));
	m_out[8] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("vector graph")));
	//m_out[9] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("shaft rendering")));
	m_out[10] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("contour map")));
	m_out[11] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("isosurface map")));
	m_out[12] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("surface render")));
	m_out[13] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("axis")));
	m_out[14] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("bounding box")));
	//m_out[15] = new QTreeWidgetItem(m_module_manager_graphics_output, QStringList(tr("high and low area map")));
	//m_out[16] = new QTreeWidgetItem(m_module_manager_convention_module, QStringList(tr("message")));
	//m_out[17] = new QTreeWidgetItem(m_module_manager_convention_module, QStringList(tr("lighting")));
	//m_out[18] = new QTreeWidgetItem(m_module_manager_data_source, QStringList(tr("function grid")));
	//m_out[19] = new QTreeWidgetItem(m_module_manager_data_source, QStringList(tr("test function grid")));
	//m_out[20] = new QTreeWidgetItem(m_module_manager_shaft, QStringList(tr("shaft data")));
	//m_out[21] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("slice")));
	m_out[22] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("transform")));
	//m_out[23] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("merge")));
	m_out[24] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("subset")));
	m_out[25] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("exclusion filter")));
	//m_out[26] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("extract drawing point")));
	//m_out[27] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("change type")));
	//m_out[28] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("math")));
	//m_out[29] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("gradient")));
	m_out[30] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("gridding")));
	//m_out[31] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("filter")));
	m_out[32] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("repeat filter")));
	//m_out[33] = new QTreeWidgetItem(m_module_manager_calculation, QStringList(tr("resample")));
	QIcon  icon11(ICON_PATH"mainimg//11.ico");
	m_module_manager_graphics_output->addChild(m_out[0]);
	m_out[0]->setIcon(0, icon11);
	//QIcon icon12("://threeDdetectSystem//icon//mainimg//12.ico");
	//m_out[1]->setIcon(0, icon12);
//	m_module_manager_graphics_output->addChild(m_out[1]);
	QIcon icon13(ICON_PATH"mainimg//13.ico");
	m_out[2]->setIcon(0, icon13);
	m_module_manager_graphics_output->addChild(m_out[2]);
	QIcon  icon14(ICON_PATH"mainimg//14.ico");
	m_out[3]->setIcon(0, icon14);
	m_module_manager_graphics_output->addChild(m_out[3]);
	QIcon  icon15(ICON_PATH"mainimg//15.ico");
	m_out[4]->setIcon(0, icon15);
	m_module_manager_graphics_output->addChild(m_out[4]);
	QIcon  icon16(ICON_PATH"mainimg//16.ico");
	m_out[5]->setIcon(0, icon16);
	m_module_manager_graphics_output->addChild(m_out[5]);
	QIcon  icon17(ICON_PATH"mainimg//17.ico");
	m_out[6]->setIcon(0, icon17);
	m_module_manager_graphics_output->addChild(m_out[6]);
	//QIcon  icon18("://threeDdetectSystem//icon//mainimg//18.ico");
	//m_out[7]->setIcon(0, icon18);
	//m_module_manager_graphics_output->addChild(m_out[7]);
	QIcon  icon19(ICON_PATH"mainimg//19.ico");
	m_out[8]->setIcon(0, icon19);
	m_module_manager_graphics_output->addChild(m_out[8]);
	//QIcon  icon20(ICON_PATH"mainimg//20.ico");
	//m_out[9]->setIcon(0, icon20);
	//m_module_manager_graphics_output->addChild(m_out[9]);
	QIcon  icon21(ICON_PATH"mainimg//21.ico");
	m_out[10]->setIcon(0, icon21);
	m_module_manager_graphics_output->addChild(m_out[10]);
	QIcon  icon22(ICON_PATH"mainimg//22.ico");
	m_out[11]->setIcon(0, icon22);
	m_module_manager_graphics_output->addChild(m_out[11]);
	QIcon  icon23(ICON_PATH"mainimg//23.ico");
	m_out[12]->setIcon(0, icon23);
	QIcon  icon24(ICON_PATH"mainimg//24.ico");
	m_out[13]->setIcon(0, icon24);
	QIcon  icon25(ICON_PATH"mainimg//25.ico");
	m_out[14]->setIcon(0, icon25);
	//QIcon  icon26("://threeDdetectSystem//icon//mainimg//26.ico");
	//m_out[15]->setIcon(0, icon26);
	//QIcon icon27("://threeDdetectSystem//icon//mainimg//27.ico");
	//m_out[16]->setIcon(0, icon27);
	//QIcon icon28("://threeDdetectSystem//icon//mainimg//28.ico");
	//m_out[17]->setIcon(0, icon28);
	//QIcon icon29("://threeDdetectSystem//icon//mainimg//29.ico");
	//m_out[18]->setIcon(0, icon29);
	//QIcon icon30("://threeDdetectSystem//icon//mainimg//30.ico");
	//m_out[19]->setIcon(0, icon30);
	//QIcon icon31("://threeDdetectSystem//icon//mainimg//31.ico");
	//m_out[20]->setIcon(0, icon31);
	//QIcon  icon32("://threeDdetectSystem//icon//mainimg//32.ico");
	//m_out[21]->setIcon(0, icon32);
	QIcon  icon33(ICON_PATH"mainimg//33.ico");
	m_out[22]->setIcon(0, icon33);
	//QIcon  icon34("://threeDdetectSystem//icon//mainimg//34.ico");
	//m_out[23]->setIcon(0, icon34);
	QIcon  icon35(ICON_PATH"mainimg//35.ico");
	m_out[24]->setIcon(0, icon35);
	QIcon  icon36(ICON_PATH"mainimg//36.ico");
	m_out[25]->setIcon(0, icon36);
	//QIcon  icon37("://threeDdetectSystem//icon//mainimg//37.ico");
	//m_out[26]->setIcon(0, icon37);
	//QIcon  icon38("://threeDdetectSystem//icon//mainimg//38.ico");
	//m_out[27]->setIcon(0, icon38);
	//QIcon  icon39("://threeDdetectSystem//icon//mainimg//39.ico");
	//m_out[28]->setIcon(0, icon39);
	//QIcon  icon40("://threeDdetectSystem//icon//mainimg//40.ico");
	//m_out[29]->setIcon(0, icon40);
	QIcon  icon41(ICON_PATH"mainimg//41.ico");
	m_out[30]->setIcon(0, icon41);
	//QIcon  icon42("://threeDdetectSystem//icon//mainimg//42.ico");
	//m_out[31]->setIcon(0, icon42);
	QIcon  icon43(ICON_PATH"mainimg//43.ico");
	m_out[32]->setIcon(0, icon43);
	//QIcon  icon44("://threeDdetectSystem//icon//mainimg//44.ico");
	//m_out[33]->setIcon(0, icon44);
	m_module_manager_graphics_output->addChild(m_out[12]);
	m_module_manager_graphics_output->addChild(m_out[13]);
	m_module_manager_graphics_output->addChild(m_out[14]);
	//m_module_manager_graphics_output->addChild(m_out[15]);
	//m_module_manager_convention_module->addChild(m_out[16]);
	//m_module_manager_convention_module->addChild(m_out[17]);
	//m_module_manager_data_source->addChild(m_out[18]);
	//m_module_manager_data_source->addChild(m_out[19]);
	//m_module_manager_shaft->addChild(m_out[20]);
	//m_module_manager_calculation->addChild(m_out[21]);
	m_module_manager_calculation->addChild(m_out[22]);
	//m_module_manager_calculation->addChild(m_out[23]);
	m_module_manager_calculation->addChild(m_out[24]);
	m_module_manager_calculation->addChild(m_out[25]);
	//m_module_manager_calculation->addChild(m_out[26]);
	//m_module_manager_calculation->addChild(m_out[27]);
	//m_module_manager_calculation->addChild(m_out[28]);
	//m_module_manager_calculation->addChild(m_out[29]);
	m_module_manager_calculation->addChild(m_out[30]);
	//m_module_manager_calculation->addChild(m_out[31]);
	m_module_manager_calculation->addChild(m_out[32]);
	//m_module_manager_calculation->addChild(m_out[33]);
//	m_module_manager_dock->setWidget(m_module_manager_treewidget);
}

void threeDdetectSystem::create_lianluo() //创建联络图管理器窗口
{
	m_contact_diagram_manager_dock = new QDockWidget(STRING_DOCK_DIAGRAM, this);
	//m_contact_diagram_manager_dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	//m_contact_diagram_manager_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	m_contact_diagram_manager_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_contact_diagram_manager_dock->setAllowedAreas(Qt::NoDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, m_contact_diagram_manager_dock);
	//m_myqwidget = new myqwidget(this);
	m_contact_diagram_manager_dock->setWidget(nullptr);
	//connect(m_myqwidget, SIGNAL(sig_show_property_manager_message(QString,base_property_widget*)), this, SLOT(slot_show_property_manager_message(QString,base_property_widget*)));
}

void threeDdetectSystem::create_shuxin()  //创建属性管理器窗口
{
	m_property_manager_dock = new QDockWidget(STRING_DOCK_PROPERTY, this);
	//m_property_manager_dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	//m_property_manager_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_property_manager_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	//addDockWidget(Qt::LeftDockWidgetArea, m_property_manager_dock);
	//addDockWidget(Qt::LeftDockWidgetArea, m_module_manager_dock);
	//m_status_dock->setFeatures(QDockWidget::DockWidgetMovable);
	//splitDockWidget(m_module_manager_dock, m_contact_diagram_manager_dock, Qt::Horizontal);
	//splitDockWidget(m_contact_diagram_manager_dock, m_property_manager_dock, Qt::Vertical);
	//splitDockWidget(m_property_manager_dock, m_status_dock, Qt::Vertical);
}

void threeDdetectSystem::create_property_description()
{
	m_property_description_dock = new QDockWidget(STRING_DOCK_DESCRIP, this);
	m_property_description_dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	QWidget* property_description_widget = new QWidget;
	QVBoxLayout* property_description_layout = new QVBoxLayout;
	m_label = new QLabel;
	property_description_layout->addWidget(m_label);
	property_description_layout->addStretch();
	property_description_widget->setLayout(property_description_layout);
	m_property_description_dock->setWidget(property_description_widget);
	m_property_description_dock->setFeatures(QDockWidget::DockWidgetMovable);
	splitDockWidget(m_property_manager_dock, m_property_description_dock, Qt::Vertical);

	QList<QDockWidget*> temp_docklist;
	temp_docklist << m_contact_diagram_manager_dock;
	temp_docklist << m_property_manager_dock;
	temp_docklist << m_property_description_dock;
	QList<int> temp_sizelist;
	temp_sizelist << static_cast<int>(this->geometry().width() * 0.6);
	temp_sizelist << static_cast<int>(this->geometry().width() * 0.3);
	temp_sizelist << static_cast<int>(this->geometry().width() * 0.1);
	this->resizeDocks(temp_docklist, temp_sizelist, Qt::Vertical);//Horizontal
}


void threeDdetectSystem::create_tab()  //创建右侧tab页面
{
	m_vtk_dock = new QDockWidget(this);
	addDockWidget(Qt::RightDockWidgetArea, m_vtk_dock);
	m_tab = new QTabWidget(this);
	m_tab->setTabsClosable(true);
	m_vtk_dock->setWidget(m_tab);
#if 0
	m_module_manager_dock->setMaximumWidth(this->width()*0.4);
	addDockWidget(Qt::LeftDockWidgetArea, m_property_manager_dock);
	addDockWidget(Qt::LeftDockWidgetArea, m_module_manager_dock);

	splitDockWidget(m_module_manager_dock, m_contact_diagram_manager_dock, Qt::Horizontal);
	splitDockWidget(m_contact_diagram_manager_dock, m_vtk_dock, Qt::Horizontal);
	splitDockWidget(m_contact_diagram_manager_dock, m_property_manager_dock, Qt::Vertical);
#else
	//m_module_manager_dock->setMaximumWidth(this->width()*0.4);
	addDockWidget(Qt::LeftDockWidgetArea, m_property_manager_dock);
	//addDockWidget(Qt::LeftDockWidgetArea, m_module_manager_dock);

//	splitDockWidget(m_module_manager_dock, m_contact_diagram_manager_dock, Qt::Horizontal);
	splitDockWidget(m_contact_diagram_manager_dock, m_vtk_dock, Qt::Horizontal);
	splitDockWidget(m_contact_diagram_manager_dock, m_property_manager_dock, Qt::Vertical);
#endif
	QList<QDockWidget*> temp_docklist;
	//temp_docklist << m_module_manager_dock;
	temp_docklist << m_contact_diagram_manager_dock;
	temp_docklist << m_vtk_dock;
	QList<int> temp_sizelist;
	//temp_sizelist << static_cast<int>(this->geometry().width() * 0);
	temp_sizelist << static_cast<int>(this->geometry().width() * 0.4);
	temp_sizelist << static_cast<int>(this->geometry().width() * 0.6);
	this->resizeDocks(temp_docklist, temp_sizelist, Qt::Horizontal);//


	connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(slot_show_current_lianluotu()));
	//m_vtk = new QVTKOpenGLNativeWidget(this);
	//m_tab->addTab(m_vtk, QString("QVTKOpenGLNativeWidget"));
}

void threeDdetectSystem::create_camera_properties()
{
	if (m_camera_properties_dialog == nullptr)
		m_camera_properties_dialog = new QDialog(this);//创建相机位置属性管理窗口
	m_camera_properties_dialog->setWindowTitle(STRING_TITLE_CAMERA);
	new_project* current_project = (new_project*)m_tab->currentWidget();
	QGroupBox *groupBox;
	QWidget *gridLayoutWidget;
	QGridLayout *gridLayout;
	QLabel *m_label_camera;
	QLabel *m_label_target;
	QDialogButtonBox* m_buttonBox = nullptr; //获取窗口返回结果

	m_camera_properties_dialog->resize(365, 180);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_camera_properties_dialog->sizePolicy().hasHeightForWidth());
	m_camera_properties_dialog->setSizePolicy(sizePolicy);
	groupBox = new QGroupBox(m_camera_properties_dialog);
	groupBox->setTitle(STRING_TITLE_GROUPBOX);
	groupBox->setGeometry(QRect(10, 10, 341, 131));
	gridLayoutWidget = new QWidget(groupBox);
	gridLayoutWidget->setGeometry(QRect(9, 19, 321, 101));
	gridLayout = new QGridLayout(gridLayoutWidget);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	m_label_camera = new QLabel(gridLayoutWidget);
	m_label_camera->setText(STRING_LABLE_CAMERA);
	gridLayout->addWidget(m_label_camera, 0, 0, 1, 1);

	m_label_target = new QLabel(gridLayoutWidget);
	m_label_target->setText(STRING_LABLE_TARGET);
	gridLayout->addWidget(m_label_target, 1, 0, 1, 1);

	m_lineE_camera_x = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_camera_x, 0, 1, 1, 1);

	m_lineE_camera_y = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_camera_y, 0, 2, 1, 1);

	m_lineE_camera_z = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_camera_z, 0, 3, 1, 1);


	m_lineE_target_x = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_target_x, 1, 1, 1, 1);

	m_lineE_target_y = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_target_y, 1, 2, 1, 1);

	m_lineE_target_z = new QLineEdit(gridLayoutWidget);
	gridLayout->addWidget(m_lineE_target_z, 1, 3, 1, 1);

	m_buttonBox = new QDialogButtonBox(m_camera_properties_dialog);
	m_buttonBox->setGeometry(QRect(10, 140, 341, 32));
	m_buttonBox->setOrientation(Qt::Horizontal);
	m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
	m_buttonBox->button(QDialogButtonBox::Ok)->setText(STRING_BUTTON_OK);
	m_buttonBox->button(QDialogButtonBox::Cancel)->setText(STRING_BUTTON_CANCEL);
	QObject::connect(m_buttonBox, SIGNAL(accepted()), m_camera_properties_dialog, SLOT(accept()));
	QObject::connect(m_buttonBox, SIGNAL(rejected()), m_camera_properties_dialog, SLOT(reject()));


	m_camera_properties_dialog->hide();
}

void threeDdetectSystem::create_CaptureVideo()
{
	if (m_capture_video_dialog == nullptr)
		m_capture_video_dialog = new QDialog(this);
	QPushButton *m_pButton_chooseFilePath;
	QSpinBox *m_spinBox_frameRate;
	QSlider *m_hSlider_quality;
	QLabel *m_label_quality;
	QPushButton *m_pButton_startCapture;
	QPushButton *m_pButton_stopCapture;
	QPushButton *m_pButton_close;
	QLineEdit *m_lineE_filePath;//录制的存储路径
	QLabel *m_label_time;
	QLabel *m_label_size;
	QLabel *m_label_frames;
	QLabel *label_1;
	QLabel *label_2;
	QLabel *label_3;
	QLabel *label_4;
	QLabel *label_5;
	QLabel *label_6;
	qint64 m_start_time = 0;
	int m_numberOf_frames = 0;//记录录屏帧数
	unsigned long m_numberOf_size = 0;//记录录屏数据大小

	m_timer_capture =new QTimer();
	m_timer_capture->setTimerType(Qt::PreciseTimer);
	m_timer_capture->setInterval(1000 / 30);//设置默认帧率

	m_capture_video_dialog->setWindowTitle(STRING_TITLE_CAPTURE);
	m_capture_video_dialog->resize(400, 210);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_capture_video_dialog->sizePolicy().hasHeightForWidth());
	m_capture_video_dialog->setSizePolicy(sizePolicy);
	label_1 = new QLabel(m_capture_video_dialog);
	label_1->setGeometry(QRect(10, 10, 61, 21));
	m_lineE_filePath = new QLineEdit(m_capture_video_dialog);
	m_lineE_filePath->setGeometry(QRect(70, 10, 241, 20));
	m_lineE_filePath->setText(QDir::currentPath());
	m_pButton_chooseFilePath = new QPushButton(m_capture_video_dialog);
	m_pButton_chooseFilePath->setGeometry(QRect(320, 10, 75, 23));
	label_2 = new QLabel(m_capture_video_dialog);
	label_2->setGeometry(QRect(20, 50, 71, 21));
	m_spinBox_frameRate = new QSpinBox(m_capture_video_dialog);
	m_spinBox_frameRate->setGeometry(QRect(80, 50, 71, 22));
	m_spinBox_frameRate->setMinimum(1);
	m_spinBox_frameRate->setSingleStep(10);
	m_spinBox_frameRate->setMaximum(100);
	m_spinBox_frameRate->setValue(30);
	label_3 = new QLabel(m_capture_video_dialog);
	label_3->setGeometry(QRect(10, 100, 61, 16));
	m_hSlider_quality = new QSlider(m_capture_video_dialog);
	m_hSlider_quality->setGeometry(QRect(80, 100, 111, 22));
	m_hSlider_quality->setMaximum(100);
	m_hSlider_quality->setValue(100);
	m_hSlider_quality->setSingleStep(10);
	m_hSlider_quality->setOrientation(Qt::Horizontal);
	m_label_quality = new QLabel(m_capture_video_dialog);
	m_label_quality->setGeometry(QRect(210, 100, 54, 21));
	m_pButton_startCapture = new QPushButton(m_capture_video_dialog);
	m_pButton_startCapture->setGeometry(QRect(100, 140, 75, 23));
	m_pButton_stopCapture = new QPushButton(m_capture_video_dialog);
	m_pButton_stopCapture->setGeometry(QRect(210, 140, 75, 23));
	m_pButton_stopCapture->setDisabled(true);
	m_pButton_close = new QPushButton(m_capture_video_dialog);
	m_pButton_close->setGeometry(QRect(150, 170, 75, 23));
	label_4 = new QLabel(m_capture_video_dialog);
	label_4->setGeometry(QRect(234, 40, 81, 16));
	label_5 = new QLabel(m_capture_video_dialog);
	label_5->setGeometry(QRect(210, 60, 121, 16));
	label_6 = new QLabel(m_capture_video_dialog);
	label_6->setGeometry(QRect(258, 80, 61, 16));
	m_label_time = new QLabel(m_capture_video_dialog);
	m_label_time->setGeometry(QRect(320, 40, 54, 16));
	m_label_size = new QLabel(m_capture_video_dialog);
	m_label_size->setGeometry(QRect(320, 60, 54, 16));
	m_label_frames = new QLabel(m_capture_video_dialog);
	m_label_frames->setGeometry(QRect(320, 80, 54, 16));

	label_1->setText(STRING_LABLE_PATH);
	label_2->setText(STRING_LABLE_FRAMERATE);
	label_3->setText(STRING_LABLE_QUALITY);
	label_4->setText(STRING_LABLE_TIMES);
	label_5->setText(STRING_LABLE_SIZE);
	label_6->setText(STRING_LABLE_FRAMRATE);
	m_pButton_chooseFilePath->setText(STRING_BUTTON_CHOOSEFILE);
	m_label_quality->setText("100%");
	m_pButton_startCapture->setText(STRING_BUTTON_STARTCAPTURE);
	m_pButton_stopCapture->setText(STRING_BUTTON_STOPCAPTURE);
	m_pButton_close->setText(STRING_BUTTON_CLOSE);
	m_label_time->setText("0.0");
	m_label_size->setText("0.0");
	m_label_frames->setText("0");

	m_capture_video_dialog->hide();
	
	connect(m_pButton_chooseFilePath, &QPushButton::clicked, m_capture_video_dialog, 
		[&, m_lineE_filePath]() {
		QString fileName = QFileDialog::getExistingDirectory(m_capture_video_dialog, STRING_ITEM_SELECT_FILE, QDir::currentPath());
		m_lineE_filePath->setText(fileName + "/tddsVideo.avi");
	});//设置存储路径

	connect(m_hSlider_quality, &QSlider::valueChanged, m_capture_video_dialog, [m_hSlider_quality, m_label_quality]() {
		m_label_quality->setText(QString("%1%").arg(m_hSlider_quality->value()));
	});//关联m_hSlider_quality m_label_quality

	connect(m_spinBox_frameRate, &QSpinBox::editingFinished, this, [&, m_spinBox_frameRate]() {
		m_timer_capture->setInterval(1000 / m_spinBox_frameRate->value());//设置帧率
	});//关联m_spinBox_frameRate 帧速率

	connect(m_pButton_startCapture, &QPushButton::clicked, m_capture_video_dialog, 
		[&, m_hSlider_quality, m_spinBox_frameRate, m_pButton_chooseFilePath, m_pButton_startCapture, m_pButton_stopCapture, m_lineE_filePath]() {
		QFileInfo dir(m_lineE_filePath->text());
		//if (dir.isFile()){
			m_timer_capture->start();
			m_numberOf_frames = 0;
			m_numberOf_size = 0;
			QString str_filePath(m_lineE_filePath->text());
			char*  ch = str_filePath.toLatin1().data();
			temp_avi_file = AVI_open_output_file(ch); //创建一个操作输出文件的对象
			AVI_set_video(temp_avi_file, 600, 600, m_spinBox_frameRate->value(), "MJPG");  //设置视频文件的分辨率， 多少帧一秒， 编码方式
			m_lineE_filePath->setDisabled(true);
			m_spinBox_frameRate->setDisabled(true);
			m_hSlider_quality->setDisabled(true);
			m_pButton_chooseFilePath->setDisabled(true);
			m_pButton_startCapture->setDisabled(true);
			m_pButton_stopCapture->setDisabled(false);
			m_start_time = QDateTime::currentDateTime().toTime_t();
		//}
	});//开始录制

	connect(m_timer_capture, &QTimer::timeout, this, 
		[&, m_hSlider_quality, m_label_time, m_label_size, m_label_frames]() {
		//这里只捕获每次定时的屏幕，在结束录制是保存所有帧，写入AVI文件
		QScreen *screen = QApplication::primaryScreen(); //获取当前程序的主窗口对象
		QWidget *current_widget = m_tab->currentWidget();
		QPixmap map = screen->grabWindow(current_widget->winId());
		QByteArray temp_byteArry;
		QBuffer bf(&temp_byteArry);
		map.save(&bf, "jpg", m_hSlider_quality->value()); // 30表示压宿率，值从0 – 100, 值越小表示编码出来的图像文件就越小，当然也就越不清晰 ---质量
		
		m_numberOf_frames++;
		AVI_write_frame(temp_avi_file,
			temp_byteArry.data(),
			temp_byteArry.size(),
			1); //保存一帧图像, keyframe表示是否为关键帧(即是否完整的一帧图像数据， jpg每张都是完整的)
		//更新属性
		m_numberOf_size += temp_byteArry.size();
		qint64 temp_time = QDateTime::currentDateTime().toTime_t();
		m_label_time->setText(QString("%1").arg(temp_time - m_start_time));
		m_label_size->setText(QString("%1").arg(m_numberOf_size / 1024 / 1024));
		m_label_frames->setText(QString("%1").arg(m_numberOf_frames));
	});//触发定时 保存每一帧

	connect(m_pButton_stopCapture, &QPushButton::clicked, this, 
		[&, m_hSlider_quality, m_spinBox_frameRate, m_pButton_chooseFilePath, m_pButton_startCapture, m_pButton_stopCapture, m_lineE_filePath]() {
		m_timer_capture->stop();
		AVI_close(temp_avi_file); //结束保存视频，并会写入文件索引的。
		m_lineE_filePath->setDisabled(false);
		m_spinBox_frameRate->setDisabled(false);
		m_hSlider_quality->setDisabled(false);
		m_pButton_chooseFilePath->setDisabled(false);
		m_pButton_startCapture->setDisabled(false);
		m_pButton_stopCapture->setDisabled(true);
	});//停止录制

	connect(m_pButton_close, SIGNAL(clicked()), m_capture_video_dialog, SLOT(reject()));//关闭窗口
}

void threeDdetectSystem::slot_show_current_lianluotu()
{
	int count = m_tab->count();
	if (count == 0)
	{
		m_contact_diagram_manager_dock->setWidget(NULL);
		m_property_manager_dock->setWidget(NULL);
	}
	else 
	{
		new_project*current_project = (new_project*)m_tab->currentWidget();
		myqwidget* current_lianluotu = current_project->current_lianluotu;
		m_contact_diagram_manager_dock->setWidget(current_lianluotu);
		if (current_lianluotu->get_m_record_highlight_form1())
			m_property_manager_dock->setWidget(current_lianluotu->get_m_record_highlight_form1()->return_m_form1_tab());
		else
			m_property_manager_dock->setWidget(NULL);
	}
}

void threeDdetectSystem::slot_create_project()   //创建空项目
{
	QString temp_path = QString("project") + QString::number(m_tab->count());
	//后面调用后端的save接口里面有创建文件并写入，故下面不用自己先创建文件。指定一个路径即可，不给则为默认路径
	//QFile file(path);
	//file.open(QIODevice::ReadWrite | QIODevice::Text);
	//file.close();
	int8_t projectid = OperationalLayer::getOperationInstance().newProject(temp_path.toStdString());
	new_project *temp_project = new new_project(this);
	temp_project->m_proj_id = projectid;
	//project_list.append(temp_project);
	OperationalLayer::getOperationInstance().setVtkRender(temp_project->m_vtkrender, projectid);
	OperationalLayer::getOperationInstance().setRenderWinInteractor(temp_project->m_vtkwindowinteractor->interactor(), projectid);
	m_contact_diagram_manager_dock->setWidget(temp_project->current_lianluotu);
	m_tab->addTab(temp_project, temp_path);
	int index = m_tab->count();
	m_tab->setCurrentIndex(index - 1);
	m_property_manager_dock->setWidget(NULL);
	connect(temp_project->current_lianluotu, SIGNAL(sig_show_property_manager_message(QString, base_property_widget*)), this, SLOT(slot_show_property_manager_message(QString, base_property_widget*)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_show_status_mess(QString)), this, SLOT(slot_status_mess(QString)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_load_data()), this, SLOT(slot_open_file()));	//create_diagram_from_file
	connect(temp_project->current_lianluotu, SIGNAL(sig_paste_widget(std::string)), this, SLOT(slot_paste_widget(std::string)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_text_widget()), this, SLOT(create_text()));
	connect(temp_project->current_lianluotu, SIGNAL(sig_comment_widget()), this, SLOT(create_annotation()));
	connect(temp_project->current_lianluotu, SIGNAL(sig_manager_property_null()), this, SLOT(slot_property_manager_property_null()));
}

void threeDdetectSystem::slot_open_project()
{
	QString file_name = QFileDialog::getOpenFileName(this, "open_file", ".", "xml(*.xml)");
	if (0 == file_name.length())	// yzw 0704
		return;
	int8_t projectId = OperationalLayer::getOperationInstance().openProject(file_name.toStdString());
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	new myqwidget(this);
	new_project *temp_project = new new_project(nullptr);
	myqwidget* lianluotu = temp_project->current_lianluotu;
	temp_project->m_proj_id = projectId;
	temp_project->new_flag = 0;
	OperationalLayer::getOperationInstance().setVtkRender(temp_project->m_vtkrender, projectId);
	OperationalLayer::getOperationInstance().setRenderWinInteractor(temp_project->m_vtkwindowinteractor->interactor(), projectId);
	m_contact_diagram_manager_dock->setWidget(temp_project->current_lianluotu);
	m_tab->addTab(temp_project, file_name);
	int index = m_tab->count();
	m_tab->setCurrentIndex(index - 1);
	connect(temp_project->current_lianluotu, SIGNAL(sig_show_property_manager_message(QString, base_property_widget*)), this, SLOT(slot_show_property_manager_message(QString, base_property_widget*)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_load_data()), this, SLOT(slot_open_file()));	//create_diagram_from_file
	connect(temp_project->current_lianluotu, SIGNAL(sig_paste_widget(std::string)), this, SLOT(slot_paste_widget(std::string)));
	connect(temp_project->current_lianluotu, SIGNAL(sig_text_widget()), this, SLOT(create_text()));
	connect(temp_project->current_lianluotu, SIGNAL(sig_comment_widget()), this, SLOT(create_annotation()));
	std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> ProjectNodeList =OperationalLayer::getOperationInstance().getProjectNodeList(projectId);
	std::multimap<uint16_t,uint16_t>connect_list=OperationalLayer::getOperationInstance().getProjectNodeListConnection(projectId);
	
	std::list<int> nida;
	int alone_id = 0;
	for (auto var : ProjectNodeList)
	{
		int left_x = var.second->m_info.m_leftTopPosX;
		int left_y = var.second->m_info.m_leftTopPosY;
		int length = var.second->m_info.m_length;
		int width = var.second->m_info.m_width;
		std::shared_ptr<SurperNode> node = project->getSpecifiedNode(var.first);
		uint16_t id= getKeyById(projectId, var.first);
		base_property_widget* pw = nullptr;
		base_property_widget* new1 = nullptr;
		//std::cout << left_x << " " << left_y << std::endl;
		if (var.second->m_nodeType == NODE_D_DATASOURCE_TYPE)
		{
			D_DataInfo* data_info_struct = (D_DataInfo*)var.second.get();
			new1 = new source_property_widget(QString::fromStdString(data_info_struct->m_filePath), data_info_struct);
			nida.push_back(node->getNodeIndex());
			connect(new1, SIGNAL(sig_delete_source()), this, SLOT(delete_source()));
		}
		else if (var.second->m_nodeType == NODE_G_CURVE_SLICE_TYPE)
		{
			G_Curve_Slice* slice_struct = (G_Curve_Slice*)var.second.get();
			new1 = new curve_slice_property_widget(0,slice_struct);
			connect(new1, SIGNAL(sig_delete_curve_slice()), this, SLOT(delete_curve_slice()));
		}
		else if (var.second->m_nodeType == NODE_G_DXF_TYPE)
		{
			G_Picture* slice_struct = (G_Picture*)var.second.get();
			QString fp(slice_struct->picFilePath.c_str());
			new1 = new dxf_property_widget(0, fp);
			connect(new1, SIGNAL(sig_delete_dxf()), this, SLOT(delete_dxf()));
		}
		else if (var.second->m_nodeType == NODE_G_PICTURE_TYPE)
		{
			G_Picture* slice_struct = (G_Picture*)var.second.get();
			QString fp(slice_struct->picFilePath.c_str());
			new1 = new picture_property_widget(0, fp);
			connect(new1, SIGNAL(sig_delete_picture()), this, SLOT(delete_picture()));
		}
		else if (var.second->m_nodeType == NODE_G_SLICEMAP_TYPE)
		{
			G_SliceMap* slice_struct = (G_SliceMap*)var.second.get();
			new1 = new slice_property_widget(0,slice_struct);
			connect(new1, SIGNAL(sig_delete_slice()), this, SLOT(delete_slice()));
		}
		else if (var.second->m_nodeType == NODE_G_VOLUMERENDERING_TYPE)	// yzw 0704
		{
			G_VolumeRender* volume_render_struct = (G_VolumeRender*)var.second.get();
			new1 = new shape_render_property_widget(0,volume_render_struct);
			connect(new1, SIGNAL(sig_delete_shape_render()), this, SLOT(delete_shape_render()));
		}
		else if (var.second->m_nodeType == NODE_G_SCATTERMAP_TYPE)
		{
			G_ScatterMap* scatter_struct = (G_ScatterMap*)var.second.get();
			new1 = new scatter_property_widget(0,scatter_struct);
			connect(new1, SIGNAL(sig_delete_scatter()), this, SLOT(delete_scatter()));
		}
		else if (var.second->m_nodeType == NODE_G_TEXT_TYPE)
		{
			G_Text* text_struct = (G_Text*)var.second.get();
			new1 = new text_property_widget(0,text_struct);
			connect(new1, SIGNAL(sig_delete_text()), this, SLOT(delete_text()));
		}
		else if (var.second->m_nodeType == NODE_G_ORTHOGONALFACTOR_TYPE)
		{
			G_Orthogonal_Factor* orthogonal_struct = (G_Orthogonal_Factor*)var.second.get();
			new1 = new orthogonal_property_widget(0,orthogonal_struct);
			connect(new1, SIGNAL(sig_delete_orthogonal()), this, SLOT(delete_orthogonal()));	
		}
		else if (var.second->m_nodeType == NODE_G_COMMENT_TYPE)
		{
			G_Comment* comment_struct = (G_Comment*)var.second.get();
			new1 = new annotation_property_widget(0,comment_struct);
			alone_id= node->getNodeIndex();
			connect(new1, SIGNAL(sig_delete_annotation()), this, SLOT(delete_annotation()));
		}
		else if (var.second->m_nodeType == NODE_G_HIGHANDLOWDIAGRAM_TYPE)
		{
			G_HighLowZoneMap* node_struct = (G_HighLowZoneMap*)var.second.get();
			new1 = new highlowmap_property_widget(0, QString::fromStdString(node_struct->demFile));
			connect(new1, SIGNAL(sig_delete_highlowmap()), this, SLOT(delete_highlowmap()));
		}
		else if (var.second->m_nodeType == NODE_G_ISOLINE_TYPE)
		{
			G_IsoLine* isoline_struct = (G_IsoLine*)var.second.get();
			new1 = new isoline_property_widget(0,isoline_struct);
			connect(new1, SIGNAL(sig_delete_isoline()), this, SLOT(delete_isoline()));
		}
		else if (var.second->m_nodeType == NODE_G_ISOSURFACE_TYPE)
		{
			G_IsoSurface* isosurface_struct = (G_IsoSurface*)var.second.get();
			new1 = new isosurface_property_widget(0,isosurface_struct);
			connect(new1, SIGNAL(sig_delete_isosurface()),this, SLOT(delete_isosurface()));
		}
		else if (var.second->m_nodeType == NODE_G_SURFACERENDER_TYPE)
		{
			G_SurfaceRender* surface_struct = (G_SurfaceRender*)var.second.get();
			new1 = new surface_render_property_widget(0,surface_struct);
			connect(new1, SIGNAL(sig_delete_surface_render()), this, SLOT(delete_surface_render()));
		}
		else if (var.second->m_nodeType == NODE_G_AXIS_TYPE)
		{
			G_Axis* axis_struct = (G_Axis*)var.second.get();
			new1 = new axis_property_widget(0,axis_struct);
			connect(new1, SIGNAL(sig_delete_axis()), this, SLOT(delete_axis()));
		}
		else if (var.second->m_nodeType == NODE_G_BOUNDINGBOX_TYPE)
		{
			G_BoundingBox* bound = (G_BoundingBox*)var.second.get();//智能指针转化为普通指针，再强制转化为特定对象类型指针
			new1 = new border_property_widget(0,bound);
			connect(new1, SIGNAL(sig_delete_border()), this, SLOT(delete_border()));
		}
		else if (var.second->m_nodeType == NODE_C_MESH_TYPE)
		{
			C_Mesh* mesh=(C_Mesh*)var.second.get();
			new1 = new gridding_property_widget(0, mesh);
			connect(new1, SIGNAL(sig_delete_gridding()), this, SLOT(delete_gridding()));
		}
		else if (var.second->m_nodeType == NODE_C_SUBSET_TYPE)
		{
			new1 = new subset_property_widget(0, var.second.get());
		connect(new1, SIGNAL(sig_delete_subset()), this, SLOT(delete_subset()));	
		}
		else if (var.second->m_nodeType == NODE_C_ELIMINATIONFILTER_TYPE)
		{
			C_EliminationFiler* ndata=(C_EliminationFiler*)var.second.get();
			new1 = new exclusion_filter_property_widget(0, ndata);
			connect(new1, SIGNAL(sig_delete_exclusion()), this, SLOT(delete_exclusion_filter()));
		}
		else if (var.second->m_nodeType == NODE_C_REPEATFILER)
		{
			C_RepeatFilter* param = (C_RepeatFilter*)var.second.get();
			new1 = new repeat_filter_property_widget(0, param);
			connect(new1, SIGNAL(sig_delete_repeat()), this, SLOT(delete_repeat_filter()));
		}
		else if (var.second->m_nodeType == NODE_C_CONVERSION_TYPE)
		{
			C_Conversion* param = (C_Conversion*)var.second.get();
			new1 = new transform_property_widget(0, param);
			connect(new1, SIGNAL(sig_delete_transform()), this, SLOT(delete_transform()));		
		}
		if(new1){
			lianluotu->paint_widget(new1, var.second->m_info); // left_x, left_y, length, width);
			m_property_manager_dock->setWidget(new1->return_m_form1_tab());
			new1->id_key = id;
			new1->checked(node->getChecked());	// 是否选中
			lianluotu->add_form1_to_m_record_all_form1(new1);
			if (!node->m_nodePreNodelist.empty()) {
				char preId = *(node->m_nodePreNodelist.begin());
				base_property_widget* prepw = lianluotu->get_form1_by_id(preId);
				lianluotu->set_m_record_highlight_form1(prepw);
				lianluotu->add_form1_button_link(new1);	// yzw 0706
				temp_project->actorNumChanged();
				new1->show_highlight(lianluotu, nullptr);
			}new1->m_update_base_info();
		lianluotu->update();
		}
		if (var.second->m_nodeType == NODE_G_SURFACERENDER_TYPE)
		{
			surface_render_property_widget* new_wid = (surface_render_property_widget*)new1;
			OperationalLayer::getOperationInstance().setSurfaceRender_cb(new_wid->id_key, new_wid->xyz_range, new_wid->update_property, new_wid);
		}
	}
	if (!nida.empty()) {
		int nid = * nida.begin();
		nida.pop_front();
		std::shared_ptr<DDataNode> dnode= std::dynamic_pointer_cast<DDataNode>( project->getSpecifiedNode(nid));
		dnode->run();
		//dnode->makeNextNodeRun();	// 运行数据节点后的散点图、边界框
	}
	slot_Orientation_Front();
}

void threeDdetectSystem::slot_close_project()
{
	int count = m_tab->count();
	if (!count)
		return;
	slot_removeSubTab(m_tab->currentIndex());
}

void threeDdetectSystem::slot_create_table() //创建空白表格
{
	Mytable *temp_table = new Mytable(100,this);
	if (Mytable::m_number_of_this_table == 1)
		m_tab->addTab(temp_table, QString("sheet"));
	else
		m_tab->addTab(temp_table, QString("sheet") + QString::number(Mytable::m_number_of_this_table));
	int index = m_tab->count();
	m_tab->setCurrentIndex(index - 1);
	table_list.append(temp_table);
}

void threeDdetectSystem::slot_save_current_project()
{
	int count = m_tab->count();
	if (!count)
		return;
//	slot_removeSubTab(m_tab->currentIndex());
	new_project*current_project = (new_project*)m_tab->currentWidget();
	int8_t projectId = current_project->get_proj_id();
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	QString filename="";
	if (!project->checkProjectSaved()) {
		 filename = QFileDialog::getSaveFileName(this, tr("save as"), "", tr("xml(*.xml)"));
	}
	OperationalLayer::getOperationInstance().saveProject(projectId, filename.toStdString());
}

void threeDdetectSystem::slot_save_as_project()
{
	int count = m_tab->count();
	if (!count)
		return;
	new_project*current_project = (new_project*)m_tab->currentWidget();
	int8_t current_projectid = current_project->get_proj_id();
	QString filename = QFileDialog::getSaveFileName(this, tr("save as"), "", tr("xml(*.xml)"));
	OperationalLayer::getOperationInstance().saveProject(current_projectid, filename.toStdString());
	return;
}

void threeDdetectSystem::slot_close_all_project() 
{
	if (!m_tab->count())
		return;
	for (int count1 = 0; count1 < m_tab->count(); count1++)
	{
		new_project*current_project = (new_project*)m_tab->widget(count1);
		int8_t current_projectid = current_project->get_proj_id();
		std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> ProjectNodeList = OperationalLayer::getOperationInstance().getProjectNodeList(current_projectid);
		if (current_project->new_flag == 1) //当前项目是新建的项目
		{
			if (!ProjectNodeList.size())  //项目内容为空
			{
				OperationalLayer::getOperationInstance().closeProject(current_projectid);
				continue;
			}
			else
			{
				QString filename = QFileDialog::getSaveFileName(this, STRING_ACTION_SAVEAS, "", tr("xml(*.xml)"));
				if (!filename.isNull())
				{
					OperationalLayer::getOperationInstance().saveProject(current_projectid, filename.toStdString());
				}
				OperationalLayer::getOperationInstance().closeProject(current_projectid);
				continue;
			}
		}
		OperationalLayer::getOperationInstance().saveProject(current_projectid);
	}
	int tab_count = m_tab->count();
	while (tab_count > 0)
	{
		m_tab->removeTab(tab_count - 1);
		tab_count = m_tab->count();
	}
	m_contact_diagram_manager_dock->setWidget(NULL);
	m_property_manager_dock->setWidget(NULL);
}

void threeDdetectSystem::slot_save_all_project()   //全部保存
{
	if (!m_tab->count())
		return;
	for (int count1 = 0; count1 < m_tab->count(); count1++)
	{
		new_project*current_project = (new_project*)m_tab->widget(count1);
		int8_t current_projectid = current_project->get_proj_id();
		std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> ProjectNodeList = OperationalLayer::getOperationInstance().getProjectNodeList(current_projectid);
		if (current_project->new_flag == 1) //当前项目是新建的项目
		{
			if (!ProjectNodeList.size())  //项目内容为空
			{
				OperationalLayer::getOperationInstance().closeProject(current_projectid);
				continue;
			}
			else
			{
				QString filename = QFileDialog::getSaveFileName(this, STRING_ACTION_SAVEAS, "", tr("xml(*.xml)"));
				if (!filename.isNull())
				{
					OperationalLayer::getOperationInstance().saveProject(current_projectid, filename.toStdString());
				}
				OperationalLayer::getOperationInstance().closeProject(current_projectid);
				continue;
			}
		}
		OperationalLayer::getOperationInstance().saveProject(current_projectid);
	}
}

void threeDdetectSystem::slot_open_file1()   //打开文件
{
	QString file_name = QFileDialog::getOpenFileName(this, "open_file", ".", "dat(*.dat)");
	if (file_name.isEmpty() == false)
	{
		QFile file(file_name);
		if (file.open(QIODevice::ReadOnly) == true)
		{
			QTextStream text(&file);
			text.setCodec("UTF-8");   //设置编码格式
			QString str = text.readAll();
			QStringList list0 = str.split("\r\n");
			Mytable *table0;
			if (list0.length() >= 100)
				table0 = new Mytable(list0.length(), this);
			else
				table0 = new Mytable(100, this);
			//Mytable *table0 = new Mytable(list0.length(),this);
			table0->set_open_file_name(file_name);
			//table0->setRowCount(list0.length());
			//table0->vector1.resize(list0.length());
			//table0->vector2.resize(list0.length());
			for (int i = 0; i < list0.length(); i++)
			{
				QStringList temp = list0[i].split("\t");
				for (int j = 0; j < temp.length(); j++)
				{
					table0->setItem(i, j, new QTableWidgetItem(temp[j]));
					//table0->vector1[i].append(temp[j]);
					//table0->vector2[i].append(temp[j]);
				}
			}
			m_tab->addTab(table0, QFileInfo(file_name).fileName());
			int index = m_tab->count();
			m_tab->setCurrentIndex(index - 1);
		}
	}
}

void threeDdetectSystem::slot_removeSubTab(int index) //关闭tab当前页的项目(鼠标点击tab页的关闭按钮)
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	int8_t current_projectid = current_project->get_proj_id();
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(current_projectid);
	if (!project->checkProjectSaved())
		OperationalLayer::getOperationInstance().saveProject(current_projectid);
	m_tab->removeTab(index);
	int tab_count = m_tab->count();
	
	if (tab_count == 0)
	{
		m_contact_diagram_manager_dock->setWidget(NULL);
		m_property_manager_dock->setWidget(NULL);
	}
	else
	{
		m_tab->setCurrentIndex(tab_count - 1);
		new_project* project1 = (new_project*)m_tab->currentWidget();
		myqwidget* lianluotu = project1->current_lianluotu;
		m_contact_diagram_manager_dock->setWidget(lianluotu);
		if (lianluotu->get_m_record_highlight_form1())
			m_property_manager_dock->setWidget(lianluotu->get_m_record_highlight_form1()->return_m_form1_tab());
		else
			m_property_manager_dock->setWidget(NULL);
	}
	delete current_project->current_lianluotu; 
	delete current_project;
}

void threeDdetectSystem::delete_source()// yzw 0705
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	source_property_widget *widget = (source_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		//current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::slot_open_file()//create_diagram_from_file()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	if (!current_project)
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	QString file_name = QFileDialog::getOpenFileName(this, STRING_ITEM_OPEN_SOURCE, ".", "dat(*.dat);;txt(*.txt);;dem(*.dem)");
	if (file_name.isEmpty())
		return;
	{
		QFile    file(file_name);
		if (file.open(QIODevice::ReadOnly) == true)
		{
			char *ch;
			QByteArray ba = file_name.toLatin1();
			ch = ba.data();
			//yzw 0615 int num = current_lianluotu->return_widget_type_num(STRING_TYPE_SOURCE);
			source_property_widget *new1 = new source_property_widget(file_name);
			//yzw 0614 new1->NodeType = "data source";
			new1->setParent(current_lianluotu);
			new1->desc = ch;
			connect(new1, SIGNAL(sig_delete_source()), this, SLOT(delete_source()));
#if 0
			connect(new1, SIGNAL(sig_data_to_scatter()), this, SLOT(create_scatter()));
			connect(new1, SIGNAL(sig_data_to_vector()), this, SLOT(create_vector()));
			connect(new1, SIGNAL(sig_data_to_axis()), this, SLOT(create_axis()));
			connect(new1, SIGNAL(sig_data_to_border()), this, SLOT(create_border()));
			connect(new1, SIGNAL(sig_data_to_gridding()), this, SLOT(create_gridding()));
			if (current_lianluotu->get_m_record_highlight_form1())
			{
				QPalette pal =current_lianluotu->get_m_record_highlight_form1()->palette();
				pal.setColor(QPalette::Background, QColor(160, 160, 160, 255));
				current_lianluotu->get_m_record_highlight_form1()->setPalette(pal);
				current_lianluotu->get_m_record_highlight_form1()->setAutoFillBackground(true);
			}
			current_lianluotu->set_m_record_highlight_form1(new1);
			QPalette pal = new1->palette();
			pal.setColor(QPalette::Background, QColor(0, 255, 160, 255));
			new1->setPalette(pal);
			new1->setAutoFillBackground(true);
#else
			current_lianluotu->dehighlight_form1();
			new1->form_layout(current_lianluotu, new1);
			new1->show();
			new1->show_highlight(current_lianluotu, new1);
#endif
			current_lianluotu->add_form1_to_m_record_all_form1(new1);
			m_property_manager_dock->setWidget(new1->return_m_form1_tab());
			D_DataInfo* pTemp = new1->return_data_source_property_struct();
			pTemp->m_name = new1->get_label_text();

			std::shared_ptr<D_DataInfo>ptr(std::make_shared<D_DataInfo>());
			*(ptr.get()) = *pTemp;
			new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(current_project->get_proj_id(), ptr);
			new1->m_update_base_info();
		}
	}
}

void threeDdetectSystem::slot_show_property_manager_message(QString a,base_property_widget *b)
{
	m_property_manager_dock->setWidget(b->return_m_form1_tab());	// yzw 0614：切换到属性页面
	m_label->setText(a);
	m_status_label->setText(a);	// yzw 0614 状态栏
}

void threeDdetectSystem::slot_property_manager_property_null()
{
	m_property_manager_dock->setWidget(NULL);
}


void threeDdetectSystem::delete_slice()	// yzw 0526
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	slice_property_widget *widget = (slice_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GSliceMapNode> Node = std::dynamic_pointer_cast<GSliceMapNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	/*	if(node->m_nodePreNodelist.size()){
			uint8_t firstId = *(node->m_nodePreNodelist.begin());
			OperationalLayer::getOperationInstance().removeNodeConnection(getKeyById(projectId, firstId), key);
		}
		OperationalLayer::getOperationInstance().deleteNodeFromProject(key);
	if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		//current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::tabUpdate(new_project*current_project, uint16_t id_key)
{
	/*int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
	current_project->actorNumChanged();
	OperationalLayer::getOperationInstance().aloneNodesRun(id_key);
}

void threeDdetectSystem::slot_tab_update(uint16_t id_key)
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	tabUpdate(current_project, id_key);
}

void threeDdetectSystem::create_slice()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	slice_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_SLICE);
	new1 = new slice_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_slice_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_slice_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_vtk_camera_render()), this, SLOT(slot_vtk_camera_render()));
	connect(new1, SIGNAL(sig_delete_slice()), this, SLOT(delete_slice()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	G_SliceMap* pTemp = (G_SliceMap*)new1->return_slice_property_struct();
	std::shared_ptr<G_SliceMap>ptr(std::make_shared<G_SliceMap>());
	*(ptr.get())= *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);	// yzw 0706
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	new1->show_highlight(current_lianluotu, new1);
	// yzw 0728
	//tabUpdate(current_project, new1->id_key);
}

void threeDdetectSystem::delete_curve_slice()	
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	curve_slice_property_widget *widget = (curve_slice_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GCurveSliceNode> Node = std::dynamic_pointer_cast<GCurveSliceNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
}

void threeDdetectSystem::create_curve_slice()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	curve_slice_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_CURVE_SLICE);
	new1 = new curve_slice_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_vtk_camera_render()), this, SLOT(slot_vtk_camera_render()));
	connect(new1, SIGNAL(sig_delete_curve_slice()), this, SLOT(delete_curve_slice()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	G_Curve_Slice* pTemp = (G_Curve_Slice*)new1->return_slice_property_struct();
	std::shared_ptr<G_Curve_Slice>ptr(std::make_shared<G_Curve_Slice>());
	*(ptr.get())= *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	new1->show_highlight(current_lianluotu, new1);

	//tabUpdate(current_project, new1->id_key);
}

void threeDdetectSystem::delete_shape_render()
{
	myqwidget* lianluotu= (myqwidget*)m_contact_diagram_manager_dock->widget();
	if(!lianluotu) return;
	shape_render_property_widget *widget= (shape_render_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GVolumeRenderNode> volumeNode = std::dynamic_pointer_cast<GVolumeRenderNode>(node);
	volumeNode->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
}

void threeDdetectSystem::create_shape_render()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	uint16_t proj_id = current_project->get_proj_id(); // yzw 0525
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	shape_render_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_SHAPE);
	new1 = new shape_render_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_shape_render()), this, SLOT(delete_shape_render())); 
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	G_VolumeRender* pTemp = new1->return_shape_render_struct();
	std::shared_ptr<G_VolumeRender>ptr(std::make_shared<G_VolumeRender>());
	*(ptr.get())= *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(proj_id, ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey){
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_scatter()	// yzw 0607
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	scatter_property_widget *widget = (scatter_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GScatterMapNode> Node = std::dynamic_pointer_cast<GScatterMapNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	//slot_vtk_camera_render();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::tips(int tips)
{
	QMessageBox::information(NULL, STRING_TIPS_ERROR, STRING_TIPS_NO_PROJ);
}

void threeDdetectSystem::create_scatter()
{
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	scatter_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_SCATTER);
	new1 = new scatter_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
//	connect(new1, SIGNAL(sig_scatter_to_cut()), this, SLOT(create_cut()));
	connect(new1, SIGNAL(sig_scatter_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_scatter_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_delete_scatter()), this, SLOT(delete_scatter()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	G_ScatterMap* pTemp = new1->return_scatter_property_struct();
	std::shared_ptr<G_ScatterMap>ptr(std::make_shared<G_ScatterMap>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_text()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	text_property_widget *widget = (text_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GTextNode> Node = std::dynamic_pointer_cast<GTextNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	slot_vtk_camera_render();
}

void threeDdetectSystem::create_text()
{	
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	text_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_TEXT);
	new1 = new text_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_text()), this, SLOT(delete_text()));	
	connect(new1, SIGNAL(sig_text_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_text_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_new_project_update()), this, SLOT(slot_vtk_camera_render()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());

	G_Text* pTemp = new1->return_text_property_struct();
	std::shared_ptr<G_Text>ptr(std::make_shared<G_Text>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	new1->show_highlight(current_lianluotu, new1);
	OperationalLayer::getOperationInstance().aloneNodesRun(new1->id_key);
	slot_vtk_camera_render();
}

void threeDdetectSystem::delete_orthogonal()	// yzw 0607
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	orthogonal_property_widget *widget = (orthogonal_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GOrthonalFactorNode> Node = std::dynamic_pointer_cast<GOrthonalFactorNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_orthogonal()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	orthogonal_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_ORTHOGO);
	new1 = new orthogonal_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_orthogonal()), this, SLOT(delete_orthogonal()));	
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	G_Orthogonal_Factor *pTemp = new1->return_orthogonal_struct();
	std::shared_ptr<G_Orthogonal_Factor>ptr(std::make_shared<G_Orthogonal_Factor>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->m_vtkrender->Render();
	}
	slot_vtk_camera_render();*/
}

void threeDdetectSystem::delete_annotation()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	annotation_property_widget *widget = (annotation_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GCommentNode> Node = std::dynamic_pointer_cast<GCommentNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget(); 
	slot_vtk_camera_render();
}


void threeDdetectSystem::create_annotation()
{
	annotation_property_widget *new1;
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_ANNOTATION);
	new1 = new annotation_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_annotation()), this, SLOT(delete_annotation()));
	connect(new1, SIGNAL(sig_note_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_note_to_border()), this, SLOT(create_border()));
	// yzw 0616 GCommentNode 未参与vtk，不需从XmlProject中删除
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu,new1);
	new1->show_highlight(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());

	G_Comment* pTemp = new1->return_annotation_property_struct();
	std::shared_ptr<G_Comment>ptr(std::make_shared<G_Comment>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	OperationalLayer::getOperationInstance().aloneNodesRun(new1->id_key);
	slot_vtk_camera_render();
}


void threeDdetectSystem::create_streamline()
{
#if 0
	base_form *new1;
	if (slice_form::m_number_of_this_form == 0)
		new1 = new streamline_form(tr("streamline diagram"));
	else
		new1 = new streamline_form(tr("streamline diagram") + QString::number(streamline_form::m_number_of_this_form + 1));
	new1->setParent(m_myqwidget);
	new1->form_layout(m_myqwidget, new1);
	new1->show();
	m_myqwidget->add_form1_to_m_record_all_form1(new1);
#endif
}


void threeDdetectSystem::create_vector()
{
	vector_property_widget *new1;
	new1 = new vector_property_widget(tr("vector diagram"));
	connect(new1, SIGNAL(sig_vector_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_vector_to_border()), this, SLOT(create_border()));
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show_highlight(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
}

void threeDdetectSystem::delete_isoline()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	isoline_property_widget *widget = (isoline_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GIsoLineNode> Node = std::dynamic_pointer_cast<GIsoLineNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}
 
void threeDdetectSystem::create_isoline()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	isoline_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_ISOLINE);
	new1 = new isoline_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_isoline_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_isoline_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_delete_isoline()),this, SLOT(delete_isoline()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
 // yzw 0610
	G_IsoLine *pTemp = new1->return_isoline_property_struct();
	std::shared_ptr<G_IsoLine>ptr(std::make_shared<G_IsoLine>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_isosurface()	 // yzw 0610
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	isosurface_property_widget *widget = (isosurface_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GIsoSurfaceNode> Node = std::dynamic_pointer_cast<GIsoSurfaceNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_isosurface()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	isosurface_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_ISOSURF);
	new1 = new isosurface_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_vtk_interactor_render()),this, SLOT(slot_vtk_camera_render()));
	connect(new1, SIGNAL(sig_delete_isosurface()),this, SLOT(delete_isosurface()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	G_IsoSurface *pTemp = new1->return_isosurface_property_struct();
	std::shared_ptr<G_IsoSurface>ptr(std::make_shared<G_IsoSurface>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_surface_render()// yzw 0607
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	surface_render_property_widget *widget = (surface_render_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GSurfaceNode> Node = std::dynamic_pointer_cast<GSurfaceNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();

	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_surface_render()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_SURFACE);
	surface_render_property_widget *new1;
	new1 = new surface_render_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_surface_render()), this, SLOT(delete_surface_render()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	G_SurfaceRender *pTemp = new1->return_surface_render_property_struct();
	std::shared_ptr<G_SurfaceRender>ptr(std::make_shared<G_SurfaceRender>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	OperationalLayer::getOperationInstance().setSurfaceRender_cb(new1->id_key, new1->xyz_range, new1->update_property, new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_axis()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	axis_property_widget *widget = (axis_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GAxis> Node = std::dynamic_pointer_cast<GAxis>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	slot_vtk_camera_render();
}

void threeDdetectSystem::create_axis()
{
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_AXIS);
	axis_property_widget *new1;
	new1 = new axis_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_axis_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_delete_axis()), this, SLOT(delete_axis()));
	connect(new1, SIGNAL(sig_vtk_camera_render()), this, SLOT(slot_vtk_camera_render()));	// 
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu,new1);
	new1->show();
	G_Axis* pTemp = new1->return_axis_property_struct();
	std::shared_ptr<G_Axis>ptr(std::make_shared<G_Axis>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey){
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

// yzw 0610
void threeDdetectSystem::delete_border()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	border_property_widget *widget = (border_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GBoundingBoxNode> Node = std::dynamic_pointer_cast<GBoundingBoxNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	/*	if(node->m_nodePreNodelist.size()){
			uint8_t firstId = *(node->m_nodePreNodelist.begin());
			OperationalLayer::getOperationInstance().removeNodeConnection(getKeyById(projectId, firstId), key);
		}
		OperationalLayer::getOperationInstance().deleteNodeFromProject(key);
	if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_border()
{
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	int num = current_lianluotu->return_widget_type_num(STRING_WIDGET_BORDER);
	border_property_widget *new1;
	new1 = new border_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_border_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_delete_border()), this, SLOT(delete_border()));
	connect(new1, SIGNAL(sig_vtk_camera_render()), this, SLOT(slot_vtk_camera_render()));	// 
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	G_BoundingBox* pTemp = new1->return_border_form_property_struct();
	std::shared_ptr<G_BoundingBox>ptr(std::make_shared<G_BoundingBox>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	new1->show_highlight(current_lianluotu, new1);
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}
	slot_vtk_camera_render();*/
}

void threeDdetectSystem::delete_highlowmap() {
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	highlowmap_property_widget *widget = (highlowmap_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GHighLowMapNode> Node = std::dynamic_pointer_cast<GHighLowMapNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	slot_vtk_camera_render();
}

void threeDdetectSystem::create_highlowmap() {
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget* pw = current_lianluotu->get_m_record_highlight_form1();
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_HIGH_LOW);
//	QString file_name = QFileDialog::getOpenFileName(this, "open_file", ".", "filter (*.*)");
//	if (file_name.isEmpty())
//		return;file_name
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(current_project->m_proj_id);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(pw->id_key);	// yzw 0712 
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);

	highlowmap_property_widget *new1 = new highlowmap_property_widget(num, QString::fromStdString(node1->m_readInFileName));
	connect(new1, SIGNAL(sig_delete_highlowmap()), this, SLOT(delete_highlowmap()));	// yzw 0610
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	G_HighLowZoneMap* pTemp = new1->return_highlowmap_form_property_struct();
	pTemp->m_name = new1->get_label_text();
	std::shared_ptr<G_HighLowZoneMap>ptr(std::make_shared<G_HighLowZoneMap>());
	*ptr = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (0 &cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/

	OperationalLayer::getOperationInstance().nextGraphNodeRun(new1->id_key);
	current_project->m_vtkrender->ResetCamera();
	current_project->m_vtkrender->Render();
	current_project->m_vtkwindow->Render();
}

void threeDdetectSystem::delete_dxf() 
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	dxf_property_widget *widget = (dxf_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GDxfNode> Node = std::dynamic_pointer_cast<GDxfNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	//slot_vtk_camera_render();
	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_dxf() 
{
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget* pw = current_lianluotu->get_m_record_highlight_form1();
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_DXF);
	QString file_name = QFileDialog::getOpenFileName(this, "select picture", ".", "dxf Files (*.dxf);");
	if (file_name.isEmpty())
		return;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(current_project->m_proj_id);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(pw->id_key);
//	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);

	dxf_property_widget *new1 = new dxf_property_widget(num, file_name);
	connect(new1, SIGNAL(sig_delete_dxf()), this, SLOT(delete_dxf()));	
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	G_Picture* pTemp = new1->return_picture_form_property_struct();
	pTemp->m_name = new1->get_label_text();
	std::shared_ptr<G_Picture>ptr(std::make_shared<G_Picture>());
	*ptr = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);	
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());

	/*int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_geotiff() 
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	geotiff_property_widget *widget = (geotiff_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GGeotiffNode> Node = std::dynamic_pointer_cast<GGeotiffNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	//slot_vtk_camera_render();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_geotiff() 
{
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget* pw = current_lianluotu->get_m_record_highlight_form1();
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_GEOTIFF);
	QString file_name = QFileDialog::getOpenFileName(this, "select picture", ".", "tiff Files (*.tif);");
	if (file_name.isEmpty())
		return;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(current_project->m_proj_id);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(pw->id_key);
//	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);

	geotiff_property_widget *new1 = new geotiff_property_widget(num, file_name);
	connect(new1, SIGNAL(sig_delete_geotiff()), this, SLOT(delete_geotiff()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	G_Geotiff* pTemp = new1->return_picture_form_property_struct();
	pTemp->m_name = new1->get_label_text();
	std::shared_ptr<G_Geotiff>ptr(std::make_shared<G_Geotiff>());
	*ptr = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());

	/*int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if (cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_picture() {
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	picture_property_widget *widget = (picture_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<GPictureNode> Node = std::dynamic_pointer_cast<GPictureNode>(node);
	Node->removeFromRender();
	widget->slot_delete_self_widget();
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->actorNumChanged();
	//slot_vtk_camera_render();
	/*if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::create_picture() {
	if (!m_tab->count())
	{
		tips(TIPS_NO_PROJECT);
		return;
	}
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget* pw = current_lianluotu->get_m_record_highlight_form1();
	int num = current_lianluotu->return_widget_type_num(STRING_GTYPE_PICTURE);
	QString file_name = QFileDialog::getOpenFileName(this, "select picture", ".", "filter (*.*)");
	if (file_name.isEmpty())
		return;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(current_project->m_proj_id);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(pw->id_key);	// yzw 0712 
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);

	picture_property_widget *new1 = new picture_property_widget(num, file_name);
	connect(new1, SIGNAL(sig_delete_picture()), this, SLOT(delete_picture()));	// yzw 0610
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	G_Picture* pTemp = new1->return_picture_form_property_struct();
	pTemp->m_name = new1->get_label_text();
	std::shared_ptr<G_Picture>ptr(std::make_shared<G_Picture>());
	*ptr = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	current_lianluotu->add_form1_button_link(new1);
	current_project->actorNumChanged();

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	/*
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(new1->id_key);
	if ( cnKey) {
		current_project->m_vtkrender->ResetCamera();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->Render();
	}*/
}

void threeDdetectSystem::delete_transform()	
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	transform_property_widget *widget = (transform_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<CConversionNode> Node = std::dynamic_pointer_cast<CConversionNode>(node);

	widget->slot_delete_self_widget();
	/*	if(node->m_nodePreNodelist.size()){
			uint8_t firstId = *(node->m_nodePreNodelist.begin());
			OperationalLayer::getOperationInstance().removeNodeConnection(getKeyById(projectId, firstId), key);
		}
		OperationalLayer::getOperationInstance().deleteNodeFromProject(key);*/
	if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		//current_project->update();
		current_project->m_vtkrender->Render();
	}
}

void threeDdetectSystem::create_transform()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	transform_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_CTYPE_TRANSFORM);
	new1 = new transform_property_widget(num);
#if 0
#else
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_vtk_camera_render()), this, SLOT(slot_vtk_camera_render()));
	connect(new1, SIGNAL(sig_delete_transform()), this, SLOT(delete_transform()));

#endif

	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	C_Conversion* pTemp = (C_Conversion*)new1->return_transform_property_struct();
	std::shared_ptr<C_Conversion>ptr(std::make_shared<C_Conversion>());
	*(ptr.get())= *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);	

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	new1->show_highlight(current_lianluotu, new1);
	// yzw 0728
	tabUpdate(current_project, new1->id_key);
}

void threeDdetectSystem::delete_subset()	
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	subset_property_widget *widget = (subset_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<CSubsetNode> Node = std::dynamic_pointer_cast<CSubsetNode>(node);
	//Node->removeFromRender();
	widget->slot_delete_self_widget();
	/*	if(node->m_nodePreNodelist.size()){
			uint8_t firstId = *(node->m_nodePreNodelist.begin());
			OperationalLayer::getOperationInstance().removeNodeConnection(getKeyById(projectId, firstId), key);
		}
		OperationalLayer::getOperationInstance().deleteNodeFromProject(key);*/
	if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}
}


void threeDdetectSystem::create_subset()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	subset_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_CTYPE_SUBSET);

	gridding_property_widget* mesh_widget = dynamic_cast<gridding_property_widget*>(current_lianluotu->get_m_record_highlight_form1());
	if (mesh_widget == NULL) {
		return;
	}

	C_Mesh* mesh = mesh_widget->return_mesh_property_struct();

	C_SubsetFilter param;
	param.m_xStart = 0;
	param.m_xEnd = mesh->m_geometry.resolution[0];
	param.m_xStep = 1;

	param.m_yStart = 0;
	param.m_yEnd = mesh->m_geometry.resolution[1];
	param.m_yStep = 1;

	param.m_zStart = 0;
	param.m_zEnd = mesh->m_geometry.resolution[2];
	param.m_zStep = 1;

	new1 = new subset_property_widget(num, &param);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_subset_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_subset_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_delete_subset()), this, SLOT(delete_subset()));	
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);

	C_SubsetFilter* pTemp = (C_SubsetFilter*)new1->return_property_struct();
	std::shared_ptr<C_SubsetFilter>ptr(std::make_shared<C_SubsetFilter>());
	*(ptr.get()) = *pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(((new_project*)m_tab->currentWidget())->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);	// yzw 0706

	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	new1->show_highlight(current_lianluotu, new1);
	// yzw 0728
	tabUpdate(current_project, new1->id_key);
}

void threeDdetectSystem::delete_exclusion_filter()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	exclusion_filter_property_widget *widget = (exclusion_filter_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<CExclusionFilterNode> Node = std::dynamic_pointer_cast<CExclusionFilterNode>(node);
	//Node->removeFromRender();
	widget->slot_delete_self_widget();
}

void threeDdetectSystem::create_exclusion_filter()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	exclusion_filter_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_CTYPE_FILTER_EXCLU);
	new1 = new exclusion_filter_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_exclusion()), this, SLOT(delete_exclusion_filter()));

	connect(new1, SIGNAL(sig_exclusion_filter_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_scatter()), this, SLOT(create_scatter()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_transform()), this, SLOT(create_transform()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_exclusion_filter()), SLOT(create_exclusion_filter()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_gridding()), this, SLOT(create_gridding()));
	connect(new1, SIGNAL(sig_exclusion_filter_to_repeat_filter()), this, SLOT(create_repeat_filter()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
	
	C_EliminationFiler* pTemp = new1->return_exclusion_filter_property_struct();
	std::shared_ptr<C_EliminationFiler>ptr(std::make_shared<C_EliminationFiler>());
	*(ptr.get())=*pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(current_project->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);	// yzw 0706
	//new1->show_highlight(current_lianluotu, new1);
}

void threeDdetectSystem::delete_gridding()// yzw 0705
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	gridding_property_widget *widget = (gridding_property_widget *)lianluotu->get_m_record_highlight_form1();
	if(widget->isRunning()){
		QMessageBox::information(NULL, STRING_TIP_DELETE_NODE, STRING_INFO_NODE_RUNNING);
		return;
	}
	
	uint16_t key = widget->id_key;
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	widget->slot_delete_self_widget();

	if (cnKey) {
		new_project*current_project = (new_project*)m_tab->currentWidget();
		OperationalLayer::getOperationInstance().nextNodeRun(cnKey);
		current_project->m_vtkrender->ResetCamera();
		current_project->update();
		current_project->m_vtkrender->Render();
	}
}

void threeDdetectSystem::create_gridding()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	uint16_t proj_id = current_project->get_proj_id();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	gridding_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_CTYPE_GRIDDING);	// ("mesh_widget");
	new1 = new gridding_property_widget(num);
	connect(new1, SIGNAL(sig_delete_gridding()), this, SLOT(delete_gridding()));
	connect(new1, SIGNAL(sig_vtk_adapt_win()), this, SLOT(slot_vtk_adapt_win()));
	
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());

	
	C_Mesh* pTemp = new1->return_mesh_property_struct();
	//pTemp->m_name = new1->get_label_text();
	std::shared_ptr<C_Mesh>ptr(std::make_shared<C_Mesh>());
	*(ptr.get())=*pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(proj_id, ptr);
	new1->m_update_base_info();
	/**/
#if 0
	if (current_lianluotu->get_m_record_highlight_form1())
	{
			if (base_property_widget::first_widget_can_append_second_widget(current_lianluotu->get_m_record_highlight_form1(), new1))
			{
				current_lianluotu->add_form1_to_m_save_all_first_form1(current_lianluotu->get_m_record_highlight_form1());
				current_lianluotu->add_form1_to_m_save_all_second_form1(new1);
				current_lianluotu->add_number_to_m_save_first_form1_button_number(2);
				current_lianluotu->add_number_to_m_save_second_form1_button_number(1);
				current_lianluotu->get_m_record_highlight_form1()->m_suff_widget_list.push_back(new1);
				new1->m_pre_widget = current_lianluotu->get_m_record_highlight_form1();
				OperationalLayer::getOperationInstance().setNodeConnection(current_lianluotu->get_m_record_highlight_form1()->id_key, new1->id_key);
				//网格化控件加入，不用管前节点是否有数据，直接加入就行。只有点击网格化控件的开始网格化按钮，才做后续变换
				//if (current_lianluotu->get_m_record_highlight_form1()->data_full)
				//{
					//new1->data_full = true;
					//vtkSmartPointer <vtkProp> pointActor = OperationalLayer::getOperationInstance().getActorFromProject(new1->id_key >> 8);
					//current_project->m_vtkrender->ResetCamera();
				//}
			}
			current_lianluotu->update();
	}
	new1->show_highlight(current_lianluotu, new1);
#else
	current_lianluotu->add_form1_button_link(new1);	// yzw 0706
#endif
}

void threeDdetectSystem::delete_repeat_filter()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) return;
	repeat_filter_property_widget *widget = (repeat_filter_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	int16_t cnKey = OperationalLayer::getOperationInstance().getExcutedCaculateNode(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	std::shared_ptr<CRepeatFilterNode> Node = std::dynamic_pointer_cast<CRepeatFilterNode>(node);
	//Node->removeFromRender();
	widget->slot_delete_self_widget();
}

void threeDdetectSystem::create_repeat_filter()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	repeat_filter_property_widget *new1;
	int num = current_lianluotu->return_widget_type_num(STRING_CTYPE_FILTER_REPEAT);
	new1 = new repeat_filter_property_widget(num);
	connect(new1, SIGNAL(sig_property_manager_dock_null()), this, SLOT(slot_property_manager_property_null()));
	connect(new1, SIGNAL(sig_delete_repeat()), this, SLOT(delete_repeat_filter()));

	connect(new1,SIGNAL(sig_repeat_filter_to_scatter()), this, SLOT(create_scatter()));
	connect(new1,SIGNAL(sig_repeat_filter_to_axis()), this, SLOT(create_axis()));
	connect(new1, SIGNAL(sig_repeat_filter_to_border()), this, SLOT(create_border()));
	connect(new1, SIGNAL(sig_repeat_filter_to_transform()), this, SLOT(create_transform()));
	connect(new1, SIGNAL(sig_repeat_filter_to_exclusion_filter()), this, SLOT(create_exclusion_filter()));
	connect(new1, SIGNAL(sig_repeat_filter_to_gridding()), this, SLOT(create_gridding()));
	connect(new1, SIGNAL(sig_repeat_filter_to_repeat_filter()), this, SLOT(create_repeat_filter()));
	new1->setParent(current_lianluotu);
	new1->form_layout(current_lianluotu, new1);
	new1->show();
	current_lianluotu->add_form1_to_m_record_all_form1(new1);
	m_property_manager_dock->setWidget(new1->return_m_form1_tab());
		
	C_RepeatFilter* pTemp = new1->return_repeat_filter_property_struct();
	std::shared_ptr<C_RepeatFilter>ptr(std::make_shared<C_RepeatFilter>());
	*(ptr.get())=*pTemp;
	new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(current_project->get_proj_id(), ptr);
	new1->m_update_base_info();

	current_lianluotu->add_form1_button_link(new1);	// yzw 0706
	//new1->show_highlight(current_lianluotu, new1);
}

void threeDdetectSystem::slot_vtk_camera_render()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->updateView(m_isLight, m_drag_mode, m_projection); 
}

void threeDdetectSystem::slot_vtk_adapt_win()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	current_project->update();
	current_project->m_vtkrender->ResetCamera();
	current_project->m_vtkrender->Render();
}

void threeDdetectSystem::slot_copy_node2()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget*  cw = current_lianluotu->get_m_record_highlight_form1();
	if (!cw)
		return;
	slot_copy_node(cw->id_key);
}

void threeDdetectSystem::slot_copy_node(uint16_t id_key)
{
	int8_t projectId = getProjectId(id_key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	OperationalLayer::setCopyNodeXml(project->getSpecifiedNode(getNodeId(id_key))->CreateXmlNode());
}

void threeDdetectSystem::slot_delete_node()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget*  cw = current_lianluotu->get_m_record_highlight_form1();
	if (!cw)
		return;
	cw->delete_self();
}

void threeDdetectSystem::slot_paste_widget2()
{
	slot_paste_widget("");
}

void threeDdetectSystem::slot_paste_widget(std::string a)
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* lianluotu = current_project->current_lianluotu;
	int8_t proj_id= current_project->get_proj_id();
		auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(proj_id); ;
	std::shared_ptr<TiXmlElement> pElement = std::dynamic_pointer_cast<TiXmlElement>(OperationalLayer::getCopyNodeXml());
	std::shared_ptr<SurperNode> node= project->createNodeFromElement(pElement.get());
	std::shared_ptr<SuperNodeStruct> st= project->getNodeParameter(node);
	st->m_info.m_leftTopPosX = lianluotu->getMousePoint().x();
	st->m_info.m_leftTopPosY =  lianluotu->getMousePoint().y();	
	base_property_widget* new1 = nullptr;
	if (st->m_nodeType == NODE_D_DATASOURCE_TYPE)
	{
		D_DataInfo* data_info_struct = (D_DataInfo*)st.get();
		new1 = new source_property_widget(QString::fromStdString(data_info_struct->m_filePath));
		connect(new1, SIGNAL(sig_delete_source()), this, SLOT(delete_source()));
	}
	else if (st->m_nodeType == NODE_G_CURVE_SLICE_TYPE)
	{
		//G_SliceMap* slice_struct = (G_SliceMap*)st.get();
		new1 = new curve_slice_property_widget(0, st.get());// slice_struct);
		connect(new1, SIGNAL(sig_delete_slice()), this, SLOT(delete_slice()));
	}
	else if (st->m_nodeType == NODE_G_DXF_TYPE)
	{
		G_Picture* slice_struct = (G_Picture*)st.get();
			QString fp(slice_struct->picFilePath.c_str());
			new1 = new dxf_property_widget(0, fp);
			connect(new1, SIGNAL(sig_delete_dxf()), this, SLOT(delete_dxf()));
	}
	else if (st->m_nodeType == NODE_G_PICTURE_TYPE)
	{
			G_Picture* slice_struct = (G_Picture*)st.get();
			QString fp(slice_struct->picFilePath.c_str());
			new1 = new picture_property_widget(0, fp);
			connect(new1, SIGNAL(sig_delete_picture()), this, SLOT(delete_picture()));
	}
	else if (st->m_nodeType == NODE_G_SLICEMAP_TYPE)
	{
		//G_SliceMap* slice_struct = (G_SliceMap*)st.get();
		new1 = new slice_property_widget(0, st.get());// slice_struct);
		connect(new1, SIGNAL(sig_delete_slice()), this, SLOT(delete_slice()));
	}
	else if (st->m_nodeType == NODE_G_VOLUMERENDERING_TYPE)	
	{
		G_VolumeRender* volume_render_struct = (G_VolumeRender*)st.get();
		new1 = new shape_render_property_widget(0, volume_render_struct);
		connect(new1, SIGNAL(sig_delete_shape_render()), this, SLOT(delete_shape_render()));
	}
	else if (st->m_nodeType == NODE_G_SCATTERMAP_TYPE)
	{
		G_ScatterMap* scatter_struct = (G_ScatterMap*)st.get();
		new1 = new scatter_property_widget(0, scatter_struct);
		connect(new1, SIGNAL(sig_delete_scatter()), this, SLOT(delete_scatter()));
	}
	else if (st->m_nodeType == NODE_G_TEXT_TYPE)
	{
		G_Text* text_struct = (G_Text*)st.get();
		new1 = new text_property_widget(0, text_struct);
		connect(new1, SIGNAL(sig_delete_text()), this, SLOT(delete_text()));
	}
	else if (st->m_nodeType == NODE_G_ORTHOGONALFACTOR_TYPE)
	{
		G_Orthogonal_Factor* orthogonal_struct = (G_Orthogonal_Factor*)st.get();
		new1 = new orthogonal_property_widget(0, orthogonal_struct);
		connect(new1, SIGNAL(sig_delete_orthogonal()), this, SLOT(delete_orthogonal()));	
	}
	else if (st->m_nodeType == NODE_G_COMMENT_TYPE)
	{
		G_Comment* comment_struct = (G_Comment*)st.get();
		new1 = new annotation_property_widget(0, comment_struct);
		connect(new1, SIGNAL(sig_delete_annotation()), this, SLOT(delete_annotation()));
	}
	else if (st->m_nodeType == NODE_G_HIGHANDLOWDIAGRAM_TYPE)
	{
		G_HighLowZoneMap* isoline_struct = (G_HighLowZoneMap*)st.get();
		new1 = new highlowmap_property_widget(0, QString::fromStdString(""));		// todo
		connect(new1, SIGNAL(sig_delete_highlowmap()), this, SLOT(delete_highlowmap()));
	}
	else if (st->m_nodeType == NODE_G_ISOLINE_TYPE)
	{
		G_IsoLine* isoline_struct = (G_IsoLine*)st.get();
		new1 = new isoline_property_widget(0, isoline_struct);
		connect(new1, SIGNAL(sig_delete_isoline()), this, SLOT(delete_isoline()));
	}
	else if (st->m_nodeType == NODE_G_ISOSURFACE_TYPE)
	{
		G_IsoSurface* isosurface_struct = (G_IsoSurface*)st.get();
		new1 = new isosurface_property_widget(0, isosurface_struct);
		connect(new1, SIGNAL(sig_delete_isosurface()),this, SLOT(delete_isosurface()));
	}
	else if (st->m_nodeType == NODE_G_SURFACERENDER_TYPE)
	{
		G_SurfaceRender* surface_struct = (G_SurfaceRender*)st.get();
		new1 = new surface_render_property_widget(0, surface_struct);
		connect(new1, SIGNAL(sig_delete_surface_render()), this, SLOT(delete_surface_render()));
	}
	else if (st->m_nodeType == NODE_G_AXIS_TYPE)
	{
		G_Axis* axis_struct = (G_Axis*)st.get();
		new1 = new axis_property_widget(0, axis_struct);
		connect(new1, SIGNAL(sig_delete_axis()), this, SLOT(delete_axis()));
	}
	else if (st->m_nodeType == NODE_G_BOUNDINGBOX_TYPE)
	{
		G_BoundingBox* bound = (G_BoundingBox*)st.get();//智能指针转化为普通指针，再强制转化为特定对象类型指针
		new1 = new border_property_widget(0, bound);
		connect(new1, SIGNAL(sig_delete_border()), this, SLOT(delete_border()));
	}
	else if (st->m_nodeType == NODE_C_MESH_TYPE)
	{
		C_Mesh* mesh = (C_Mesh*)st.get();
		new1 = new gridding_property_widget(0, mesh);
		connect(new1, SIGNAL(sig_delete_gridding()), this, SLOT(delete_gridding()));
	}
	else if (st->m_nodeType == NODE_C_SUBSET_TYPE)
	{
		C_SubsetFilter* param = (C_SubsetFilter*)st.get();
		new1 = new subset_property_widget(0, param);
		connect(new1, SIGNAL(sig_delete_subset()), this, SLOT(delete_subset()));	
	}
	else if (st->m_nodeType == NODE_C_ELIMINATIONFILTER_TYPE)
	{
		C_EliminationFiler* param = (C_EliminationFiler*)st.get();
		new1 = new exclusion_filter_property_widget(0, param);
		connect(new1, SIGNAL(sig_delete_exclusion()), this, SLOT(delete_exclusion_filter()));
	}
	else if (st->m_nodeType == NODE_C_REPEATFILER)
	{
		C_RepeatFilter* param = (C_RepeatFilter*)st.get();
		new1 = new repeat_filter_property_widget(0, param);
		connect(new1, SIGNAL(sig_delete_repeat()), this, SLOT(delete_repeat_filter()));
	}
	else if (st->m_nodeType == NODE_C_CONVERSION_TYPE)
	{
		C_Conversion* param = (C_Conversion*)st.get();
		new1 = new transform_property_widget(0, param);
		connect(new1, SIGNAL(sig_delete_transform()), this, SLOT(delete_transform()));		
	}
	if (new1) {
		lianluotu->paint_widget(new1, st->m_info); // left_x, left_y, length, width);
		m_property_manager_dock->setWidget(new1->return_m_form1_tab());
		new1->id_key = OperationalLayer::getOperationInstance().addNodeIntoProject(proj_id, st);
		new1->m_update_base_info();
		new1->checked(node->getChecked());	// 是否选中
		lianluotu->add_form1_to_m_record_all_form1(new1);
		lianluotu->update();
	}
	 if (st->m_nodeType == NODE_G_SURFACERENDER_TYPE)
	{
		surface_render_property_widget* new_wid =  (surface_render_property_widget*)new1;
		OperationalLayer::getOperationInstance().setSurfaceRender_cb(new_wid->id_key, new_wid->xyz_range, new_wid->update_property, new_wid);
	}
}

void threeDdetectSystem::slot_save_data(uint16_t id_key)
{
	QString file_name = QFileDialog::getSaveFileName(this, STRING_ACTION_SAVEAS, ".", "dat(*.dat)");
	if (0 == file_name.length())	// yzw 0704
		return; 
	bool sok= false;
	int8_t projectId = getProjectId(id_key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(id_key));
	if (NODE_C_TYPE == node->getBelongType()) {
		std::shared_ptr<CaculateNode> gnode = std::dynamic_pointer_cast<CaculateNode> (node);
		sok =gnode->saveData(file_name.toStdString());
	}
	else if(NODE_G_TYPE ==node->getBelongType()){
		std::shared_ptr<GraphNode> gnode = std::dynamic_pointer_cast<GraphNode> (node);
		sok =gnode->saveData(file_name.toStdString());
	}
	QMessageBox msgBox;
	msgBox.setText(sok ?"OK.":"Failed");;
	msgBox.exec();
}

void threeDdetectSystem::slot_save_data2()
{
	new_project*current_project = (new_project*)m_tab->currentWidget();
	myqwidget* current_lianluotu = current_project->current_lianluotu;
	base_property_widget*  cw = current_lianluotu->get_m_record_highlight_form1();
	if (!cw)
		return;
	slot_save_data(cw->id_key);
}


std::shared_ptr<SurperNode> threeDdetectSystem::getCurrentNode()
{
	myqwidget* lianluotu = (myqwidget*)m_contact_diagram_manager_dock->widget();
	if (!lianluotu) 
		return nullptr;
	base_property_widget *widget = (base_property_widget *)lianluotu->get_m_record_highlight_form1();
	uint16_t key = widget->id_key;
	int8_t projectId = getProjectId(key);
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	return project->getSpecifiedNode(getNodeId(key));
}
