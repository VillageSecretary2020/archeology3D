/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
//
//
// 创建者：tangAo
//
// 修改人：LiMing
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include <string>
#include "ui_threeDdetectSystem.h"
#include <vtkSmartPointer.h>
//#include <vtkVectorText.h>
//#include <vtkLinearExtrusionFilter.h>
//#include <vtkTriangleFilter.h>
//#include <vtkDataSetMapper.h>
//#include <vtkActor.h>
//#include <vtkProperty.h>
//#include <vtkCamera.h>
//#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
//#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
//#include "QVTKOpenGLNativeWidget.h"

#include <QObject>
#include <QMainWindow>
#include <QTableWidget>
#include <QSplitter>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QWidget>
#include <QPalette>
#include <QTreeWidget>
#include <QList>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QLabel>
#include <QTabWidget>
#include <QDebug>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QFileDialog>
#include <QMouseEvent>
#include <QScrollBar>
#include <QKeyEvent>
#include <QStatusBar>
#include <QVector>
#include <QToolButton>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QScreen>
#include <QClipboard>
#include <QBuffer>
#include <QTimer>
#include "mytable.h"
#include "new_project.h"
#include "avilib/avilib.h"

class base_property_widget;   //前置声明，避免头文件循环嵌套
class myqwidget;  //同上

#define ICON_PATH "://threeDdetectSystem//icon//"

class threeDdetectSystem : public QMainWindow
{
    Q_OBJECT
public:
    threeDdetectSystem(QWidget *parent = Q_NULLPTR);
	static void layerCallFun(void * ptr, int8_t projectId);
	inline QTabWidget* return_m_tab() 
	{
		return m_tab;
	}
	void nodePos(SuperNodeStruct* pNode, base_property_widget* pW);	// yzw 0629

//	QVector<QVector<int>>project_widget_id_list; //每一个项目对应的list，里面存放控件
private:
	void getProjectProp(int8_t projectId);
	void tabUpdate(new_project*current_project, uint16_t id_key);
	std::shared_ptr<SurperNode> getCurrentNode();

public:
	QTabWidget *m_tab;

private:
	QMenu *file;
	QMenu *edit;
	QMenu *view;
	QMenu *tool;
	QMenu *calculate;
	QAction *calculate_menu[5];
	QMenu *render;
	QAction *render_menu[14];
	QMenu *window1;
	QMenu *help;
	QAction *help_menu[2];
	QAction *file1[10];
	QAction *view1[13];
	QMenu *new1;
	QToolBar *fileToolBar;
	QAction *m_act[28 + 5];//QAction *m_act[28]; 加上Orientation观测方向
	QToolButton *m_toolButton_Orientation;
	QDockWidget *m_module_manager_dock;  //模块管理器 dock窗口
	QTreeWidget *m_module_manager_treewidget;  //模块管理器 树形节点
	QList<QTreeWidgetItem*> items;
	QTreeWidgetItem *m_module_manager_import;   //模块管理器导入
	QTreeWidgetItem *m_module_manager_example; //模块管理器范例
	QTreeWidgetItem *m_module_manager_graphics_output;  //模块管理器图形输出
	QTreeWidgetItem *m_module_manager_convention_module; //模块管理器常规模块
	QTreeWidgetItem *m_module_manager_data_source;  //模块管理器数据源
	QTreeWidgetItem *m_module_manager_shaft;      //模块管理器竖井
	QTreeWidgetItem *m_module_manager_calculation;  //模块管理器计算
	QTreeWidgetItem *m_fIteam1a;
	QTreeWidgetItem *m_fIteam1b;
	QTreeWidgetItem *m_out[34];
	QDockWidget *m_contact_diagram_manager_dock;  //联络图管理器 dock窗口
	QDockWidget *m_property_manager_dock;      //属性管理器dock窗口
	QDockWidget *m_property_description_dock;   //属性说明窗口
	QLabel *m_label;					// yzw 0614 详细描述
	QDockWidget *m_vtk_dock;                   //右侧渲染dock
	QVector<Mytable*>table_list;  //新建表格列表，存储新建的表格
	QVector<new_project*>project_list;
	myqwidget *m_myqwidget;  //自定义控件(轴，散点图等控件)的父界面
	QStatusBar m_status_bar;
	QLabel *m_status_label;

	QLineEdit *m_lineE_camera_y;//获取相机位置
	QLineEdit *m_lineE_camera_x;
	QLineEdit *m_lineE_camera_z;
	QLineEdit *m_lineE_target_x;
	QLineEdit *m_lineE_target_y;
	QLineEdit *m_lineE_target_z;
	QTimer* m_timer_capture = nullptr; //录制视频定时器
private:
	void tips(int );

signals:
	void sig_close_current_new_table(Mytable*);
	void sig_close_exist_table(Mytable*, QString);
private slots:
	void slot_create_project();
	void slot_open_project();
	void slot_create_table();
	void slot_open_file1();  //菜单的打开功能
	void slot_save_current_project();
	void slot_save_as_project();
	void slot_save_all_project();
	void slot_close_all_project();
	void slot_close_project();
	void slot_removeSubTab(int);
	void slot_show_property_manager_message(QString,base_property_widget *);
	void slot_ClickModuleManagerGenerateForm(QTreeWidgetItem *,int);
	void slot_show_current_lianluotu();
	void slot_status_mess(QString);

	//toolbar button slots
	void slot_drag_actor();
	void slot_translation();
	void slot_perspective();
	void slot_projection();
	void slot_toolbutton_orientation(); //改变camera位置
	void slot_Orientation_Front();		//当相机位于前面
	void slot_Orientation_Back();		//后
	void slot_Orientation_Left();		//左
	void slot_Orientation_Right();		//右
	void slot_Orientation_Top();		//上
	void slot_Orientation_Button();		//下
	void slot_lighting_switch(bool);
	void slot_camera_target();
	void slot_capture_video();
	void slot_snapshot();

	void slot_lianluotu_manager_not_show();
	void slot_lianluotu_manager_show();
	void slot_property_manager_not_show();
	void slot_property_manager_show();
	void slot_module_manager_not_show();
	void slot_module_manager_show();
	void slot_menu_about();

	void create_menu();  //创建菜单函数
	void create_toolbar();  //创建工具栏函数
	void connect_toolbar(); //绑定槽函数到工具栏上的按钮
	void create_moukuai();  //创建模块管理器
	void create_lianluo();  //创建联络图管理器
	void create_shuxin();  //创建属性管理器
	void create_tab();    //创建tab页面
	void create_camera_properties();	//创建相机位置属性设置窗口
	void create_CaptureVideo();			//创建录屏窗口
	void create_property_description();//创建属性描述管理器
public slots:
	void create_curve_slice();
	void delete_curve_slice();
	void create_slice();
	void delete_slice();
	void create_shape_render();
	void delete_shape_render();
	void create_scatter();
	void delete_scatter();
	void create_text();
	void delete_text();
	void create_orthogonal();
	void delete_orthogonal();	
	void create_annotation();
	void delete_annotation();
	void create_streamline();
	void create_vector();
	void create_isoline();
	void delete_isoline(); 
	void create_isosurface();
	void delete_isosurface();
	void create_surface_render();
	void delete_surface_render();
	void create_axis();
	void delete_axis();
	void create_border();
	void delete_border(); 
	void create_highlowmap();
	void delete_highlowmap();
	void create_dxf();
	void delete_dxf();
	void create_geotiff();
	void delete_geotiff();
	void create_picture();
	void delete_picture();
	void create_transform();
	void delete_transform();
	void create_subset();
	void delete_subset();
	void create_exclusion_filter();
	void delete_exclusion_filter();
	void create_gridding();
	void delete_gridding();
	void create_repeat_filter();
	void delete_repeat_filter();
	void delete_source();
	void slot_open_file();// create_diagram_from_file();
	void slot_copy_node(uint16_t id_key);
	void slot_copy_node2();
	void slot_delete_node();
	void slot_save_data(uint16_t id_key);
	void slot_save_data2();
	void slot_tab_update(uint16_t id_key);
	void slot_paste_widget(std::string);
	void slot_paste_widget2();
	void slot_property_manager_property_null();
	void slot_vtk_camera_render();
	void slot_vtk_adapt_win();

private:
    Ui::threeDdetectSystemClass ui;
private:
	enum CamOrientation {//相机视图方向
		OrientationT,	//上
		OrientationB,	//下
		OrientationR,	//右
		OrientationL,	//左
		OrientationF,	//前
		OrientationA	//后
	};
	int m_drag_mode = 0;  //VTK窗口拖拽方式 旋转或者平移
	int m_projection = 0;  //VTK窗口是否投影
	int m_isLight = 1;		//场景中灯的开关
	avi_t* temp_avi_file = nullptr;
    CamOrientation m_direction = OrientationF;
	vtkSmartPointer<vtkLight> current_new_light = nullptr;
	QDialog* m_camera_properties_dialog = nullptr;
	QDialog* m_capture_video_dialog = nullptr;
};

#define TIPS_NO_PROJECT 1
