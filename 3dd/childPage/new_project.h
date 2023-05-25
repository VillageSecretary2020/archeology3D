#pragma once

#include "istyle.h"

class new_project :public QWidget//创建项目类
{
	Q_OBJECT;
public:
	new_project(QWidget *parent = nullptr);
	void actorNumChanged();
	int8_t get_proj_id() { return m_proj_id; }
	void lightOn(bool);
	void updateView(bool light, int drag_mode, int projection);

public:
	QVTKOpenGLNativeWidget *m_vtkwindowinteractor;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow>m_vtkwindow;
	vtkSmartPointer<vtkRenderer>m_vtkrender;
	vtkSmartPointer<vtkAxesActor>axes;
	vtkOrientationMarkerWidget *a1;
	MouseInteractorHighLightActor* style;
	static int m_vtkwindow_count;
	int8_t m_proj_id;
	myqwidget* current_lianluotu;
	int new_flag ;
	std::list<vtkSmartPointer<vtkProp>>actor_list;
	int actor_num = 0;
private:
	
};
