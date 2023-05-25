/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。 
//
// 文件功能描述：
// GAxisNode.h
// 
// 创建者：tangao 
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "GraphNode.h"

class GAxis : public GraphNode
{
public:
	GAxis(std::shared_ptr< XmlProject> project);
	~GAxis();

public:
	static void getDefault(SuperNodeStruct*);
	bool setParameter(std::shared_ptr<SuperNodeStruct> axis);
	bool getParameter(G_Axis &axis);
	std::shared_ptr<SuperNodeStruct> getParameter();	// yzw 0707
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	vtkSmartPointer < vtkPolyData> CreateSingleVectorArrowData(double* origin, double* target, double length);
	vtkSmartPointer < vtkGlyph3D> GenerateNormalsArrow(vtkSmartPointer < vtkPolyData> polyData);
	bool run();
	bool addToRender();	// yzw 0608
	bool removeFromRender(); // yzw 0606
	void getBound(double bounds[6]);
	void setBound(double* bounds);
private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> axis);
private:
	G_Axis m_axis;
	vtkSmartPointer<vtkCubeAxesActor> m_cubeAxesActor;
	//vtkSmartPointer<vtkAxesActor>		m_axisActor;
	vtkSmartPointer<vtkActor2D> m_textActor;

	vtkSmartPointer<vtkActor> xArrowActor;
	vtkSmartPointer<vtkActor> yArrowActor;
	vtkSmartPointer<vtkActor> zArrowActor;

	double m_bounds[6];
};
