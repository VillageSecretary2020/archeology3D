#pragma once
#include <qwidget.h>
#include "QVTKOpenGLNativeWidget.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkRenderer.h"
#include "OperationalLayer.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkAxesActor.h"
#include "myqwidget.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkPropPicker.h>

typedef std::function< void(double*)> PF_SET_POINT;
class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
	static MouseInteractorHighLightActor* New() {
		return new MouseInteractorHighLightActor();
	};
	virtual void OnLeftButtonDown()
	{
		int* clickPos = this->GetInteractor()->GetEventPosition();

		// Pick from this location.
		vtkSmartPointer<vtkPropPicker>  picker =
			vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

		double* pos = picker->GetPickPosition();
		std::cout << "Pick position (world coordinates) is: "
			<< pos[0] << " " << pos[1]
			<< " " << pos[2] << std::endl;

		std::cout << "Picked actor: " << picker->GetActor() << std::endl;

		if (active ){
			if (active == picker->GetActor())
			{// 是指定的actor则允许拖动
				move = true;
				return;
			}
		}
		if (imgActor){
			void* actor = picker->GetActor();
			if (actor== (void*)imgActor )
			{// 是指定的actor则允许拖动
				move = true;
				return;
			}
		}
		switch (m_state)
		{
		case 1:// 1 平移
			vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
			break;
		default:// 默认旋转
			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
			break;
		}
	}
	virtual void OnLeftButtonUp()
	{
		move = false;
		//printf("%s\n", "OnLeftButtonUp");
		//std::cout << "OnLeftButtonUp\n";
		vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	}
	virtual void OnMouseMove()
	{
		vtkRenderWindowInteractor *rwi = this->Interactor;
		if (move == true)
		{
			double *obj_center = active->GetCenter();
			double disp_obj_center[3], new_pick_point[4];
			double old_pick_point[4], motion_vector[3];
			this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2],
				disp_obj_center);

			this->ComputeDisplayToWorld(rwi->GetEventPosition()[0],
				rwi->GetEventPosition()[1],
				disp_obj_center[2],
				new_pick_point);

			this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
				rwi->GetLastEventPosition()[1],
				disp_obj_center[2],
				old_pick_point);

			motion_vector[0] = new_pick_point[0] - old_pick_point[0];
			motion_vector[1] = new_pick_point[1] - old_pick_point[1];
			motion_vector[2] = new_pick_point[2] - old_pick_point[2];
			if (active->GetUserMatrix() != NULL)
			{
				vtkTransform *t = vtkTransform::New();
				t->PostMultiply();
				t->SetMatrix(active->GetUserMatrix());
				t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
				active->GetUserMatrix()->DeepCopy(t->GetMatrix());
				t->Delete();
			}
			else
			{
				active->AddPosition(motion_vector[0],
					motion_vector[1],
					motion_vector[2]);
				if(pf_set_point)
					pf_set_point(active->GetPosition());	// 回送actor的坐标
			}

			rwi->Render();
		}
		vtkInteractorStyleTrackballCamera::OnMouseMove();
	}
	void set_state(int state) { m_state = state; }
	~MouseInteractorHighLightActor() {
		if (this != NULL)
			delete(this);
	};
public:
	vtkActor* active = nullptr;	// 允许拖动这个指定的actor
	vtkImageActor* imgActor = nullptr;	
	PF_SET_POINT pf_set_point= nullptr;
private:
	bool move = false;
	int m_state = 0;
};
