#include "new_project.h"
#include <QHBoxLayout>
#include "vtkScalarBarActor.h"
#include "vtkTextProperty.h"
#include "vtkProperty2D.h"
#include <vtkOutputWindow.h>
#include <vtkFileOutputWindow.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int new_project::m_vtkwindow_count = 0;
new_project::new_project(QWidget *parent) :QWidget(parent)
{
	vtkOutputWindow::SetGlobalWarningDisplay(0);//不弹出vtkOutputWindow窗口
	new_flag = 1; //新建标志位
	current_lianluotu = new myqwidget(nullptr);//生成了联络图的空窗口
	//m_proj_id = RETURN_ERROR;
	m_vtkrender = vtkSmartPointer<vtkRenderer>::New();
	m_vtkrender->SetBackground(0.4, 0.5, 0.6);

	m_vtkwindow=vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_vtkwindow->AddRenderer(m_vtkrender);
	m_vtkwindow->Render();

	m_vtkwindowinteractor = new QVTKOpenGLNativeWidget(this);
	m_vtkwindowinteractor->setRenderWindow(m_vtkwindow);
	vtkRenderWindowInteractor* iren = m_vtkwindowinteractor->interactor();
	m_vtkwindow->SetInteractor(iren);
	style = MouseInteractorHighLightActor::New();
	style->SetDefaultRenderer(m_vtkrender);
	iren->SetInteractorStyle(style);

	axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetXAxisLabelText("X");
	axes->SetYAxisLabelText("Y");
	axes->SetZAxisLabelText("Z");
	a1 = vtkOrientationMarkerWidget::New();
	a1->SetOrientationMarker(axes);
	a1->SetInteractor(iren);
	a1->SetEnabled(true);
	a1->InteractiveOn();
	//this->m_vtkwindow->GetInteractor()->Start();

	QHBoxLayout *project_layout = new QHBoxLayout(this);
	project_layout->addWidget(m_vtkwindowinteractor);
	this->setLayout(project_layout);
	m_vtkwindow_count++;
	//test();
}

void new_project::lightOn(bool m_isLight)
{
	vtkLightCollection* originalLights = m_vtkrender->GetLights();
	if (originalLights->GetNumberOfItems() < 1)
		return;
	vtkLight* current_light = (vtkLight*)originalLights->GetItemAsObject(0);
	if (!current_light)
		return;
	vtkActorCollection * allActor = m_vtkrender->GetActors();
	vtkCollectionSimpleIterator pit;
	vtkActor *currActor = nullptr;
	current_light->SetSwitch(m_isLight);
	for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
	{
		currActor->GetProperty()->SetDiffuse(m_isLight?1:0);
	}
}

void new_project::actorNumChanged()
{
	int num = m_vtkrender->GetActors()->GetNumberOfItems();
	if(0== actor_num&& 0< num)
		m_vtkrender->ResetCamera();
	if(0== actor_num|| 0==num||num!=actor_num){
		m_vtkwindow->Render();
		//m_vtkrender->Render();
	}
	update();
	actor_num= num;
}

void new_project::updateView(bool light, int drag_mode, int projection)
{
	style->set_state(drag_mode);//设置拖拽方式
	m_vtkwindow->Render();
	m_vtkrender->GetActiveCamera()->SetParallelProjection(projection);
	lightOn(light);
	m_vtkrender->Render();
	update();
}
