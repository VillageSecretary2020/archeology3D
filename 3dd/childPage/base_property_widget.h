// base_property_widget.h

#pragma once
#include <memory>
#include <list>

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QFontComboBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <qDebug>
#include <QLabel>
#include <QGridLayout>
#include <QString>
#include <QMainWindow>
#include <QMouseEvent>
#include <QEvent>
#include <QAction>
#include <QContextMenuEvent>
#include <QStringList>
#include <QTimer>
#include <QTreeWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>
#include <QTabWidget>
#include <QDoubleSpinBox>
#include <QSlider.h>

//#include "threeDdetectSystem.h"
#include "Nodeparaters.h"
#include "myqwidget.h"
#include "custom_button_left.h"
#include "custom_button_right.h"
#include "OperationalLayer.h"


QString  crgb2name(Child_colorRGB crgb);
// color name to Child_colorRGB
Child_colorRGB name2crgb(QString t);

/*
这是所有界面上的控件的基类(即联络图上的控件)
*/

class base_property_widget:public QWidget
{
	Q_OBJECT
public:
	base_property_widget(QWidget *parent = nullptr, QString str = "");
	std::string NodeType = "base_widget";
	uint16_t id_key;
	std::vector<std::string>m_can_append_widget; //可连接的后续节点
	static int first_widget_can_append_second_widget(base_property_widget*, base_property_widget*);
	base_property_widget* m_pre_widget = nullptr;  //前节点
	std::vector<base_property_widget*>m_suff_widget_list;  //后续节点vector

	void create_2buttons(QWidget *parent, const QString label);
	void create_rbuttons(QWidget *parent, const QString label);
	void checked(bool);		// 设置是否选中
	void remove_widget_suff_widget(base_property_widget*, uint16_t);
	void show_highlight(myqwidget *, base_property_widget *);
	void highlight1(bool hl);
	void color_choose(QComboBox*);
	void gradient_color(QComboBox*);
	std::string get_label_text();
	//bool data_full=false; //yzw 0802 
	bool close_progress();
	virtual void end_progress();
	void running(bool r=true);
	virtual void delete_self() =0;
	void setLabel(QString* p=nullptr);
protected:
	QCheckBox *m_form_check;  //控件的单选框按钮
	QLabel *m_form_label;     //控件的label标签
	custom_button_left *m_btn_left;
	custom_button_right* m_btn_right;
	QTabWidget *m_form1_tab;//在主窗口的属性管理器中添加tab页面
	QTreeWidgetItem *m_pre_treeitem;
	QTreeWidgetItem *m_input_treeitem;	// 0802 属性管理器中，说明 此node的前置node 的文本
	QTreeWidgetItem *m_input_data_count_treeitem= nullptr;
	QTreeWidgetItem *m_field_index_treeitem= nullptr;
	virtual void contextMenuEvent(QContextMenuEvent  *);
	void mouseMoveEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void recursion_add_actor(base_property_widget*); //该函数主要是控件连接时候，将其后续节点actor都添加到界面显示.
	void recursion_delete_actor(base_property_widget *new1);
private:
	QString m_label;
	QPoint m_mouse_pressPoint;  //记录鼠标按下的位置坐标，用于移动控件  
	QTimer *timer;
public:
	SuperNodeStruct* baseStruct;
	void getBaseStruct();
	void base_info_to_widget();
	void data_count_to_widget(int, int);
	void m_update_base_info();
	virtual void m_update_widget_property();
	inline QString return_m_form1_label_text()  //返回控件的标签文本内容
	{
		return m_form_label->text();
	}
	int isvalid(base_property_widget*, int x1, int y1, int x2, int y2);
	void layout1(int*, int*,int*,int*);  //新建的控件在页面上的布局
	void form_layout(myqwidget*, base_property_widget*);
	virtual void set_color_bar_actor(bool);
	virtual void update_display(std::shared_ptr<SuperNodeStruct>);

	char *desc;
	QTabWidget* return_m_form1_tab();  //返回 m_form1_tab控件
	QTimer ia_tmr;
signals:
	void sig_copy_node(uint16_t id_key);
	void sig_save_data(uint16_t id_key);
	void sig_tab_update(uint16_t id_key);
	void mysignal(QString);
	void emit_create_table_dock2();
	void sig_vtk_camera_render();
	void sig_property_manager_dock_null();
	void sig_close_progress();
public slots:
	void slot_delete_self_widget(); // 控件的删除
protected slots:
	virtual void slot_checked_actor(int);
	virtual void slot_cut_plane_property_changed(Child_CutingPlane*, int type);
	void slot_rename_control();
	void slot_ClickButtonDrawLine();
	void slot_draw_line(int index);
	void slot_connect_input();
	void slot_connect_output();
	void slot_copy_widget();
	void slot_save_widget();
private slots:
	void slot_time_out();
	void slot_close_progress();
};

// 计算节点的可连接的渲染节点
#define CAN_APPEND_WIDGET_GTYPE_ALL(p1)						\
p1.push_back(STRING_GTYPE_SLICE);	\
p1.push_back(STRING_WIDGET_SHAPE);		\
p1.push_back(STRING_WIDGET_SCATTER);				\
p1.push_back(STRING_WIDGET_ORTHOGO);		\
p1.push_back(STRING_WIDGET_ISOLINE);				\
p1.push_back(STRING_WIDGET_ISOSURF);			\
p1.push_back(STRING_WIDGET_AXIS);					\
p1.push_back(STRING_WIDGET_BORDER);				\
p1.push_back(STRING_GTYPE_PICTURE);				\
p1.push_back(STRING_GTYPE_CURVE_SLICE);				\
p1.push_back(STRING_GTYPE_DXF);				\
p1.push_back(STRING_WIDGET_SURFACE)

// 计算节点的渲染图形菜单
#define MENU_RENDER_CALC_NODE(out_put, receiver1)	\
out_put->addAction(STRING_ACTION_PICTURE, receiver1, SLOT(create_picture())); 	\
out_put->addAction(STRING_ACTION_GEOTIFF, receiver1, SLOT(create_geotiff())); 	\
out_put->addAction(STRING_ACTION_DXF, receiver1, SLOT(create_dxf())); 	\
out_put->addAction(STRING_ACTION_BOUNDBOX, receiver1, SLOT(create_border())); 	\
out_put->addAction(STRING_ACTION_SHAPRENDER, receiver1, SLOT(create_shape_render()));	\
out_put->addAction(STRING_ACTION_SLICE, receiver1, SLOT(create_slice())); \
out_put->addAction(STRING_ACTION_CURVE_SLICE, receiver1, SLOT(create_curve_slice())); \
out_put->addAction(STRING_ITEM_SCATDIAG, receiver1, SLOT(create_scatter())); \
out_put->addAction(STRING_ACTION_ORTHOGRAPH, receiver1, SLOT(create_orthogonal()));	\
out_put->addAction(STRING_ITEM_ISOLINE, receiver1, SLOT(create_isoline())); \
out_put->addAction(STRING_ISOSURF_MENU, receiver1, SLOT(create_isosurface()));	\
out_put->addAction(STRING_ACTION_SUFARENDER, receiver1, SLOT(create_surface_render()));	\
out_put->addAction(STRING_ACTION_AXIS, receiver1, SLOT(create_axis()))
