/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。 
//
// 文件功能描述：
//
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

class GScatterMapNode :public GraphNode
{
public:
	GScatterMapNode(std::shared_ptr<XmlProject> project);
	~GScatterMapNode();
public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> scatterMap);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }

	bool run();
	bool addToRender();	// yzw 0608
	bool removeFromRender();	// yzw 0607
	void makeNextNodeRun();
	void getBound(double bounds[6]);
	void setBound(double* bounds);

private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> scatterMap);
private:
	G_ScatterMap m_scatterMap;
	////vtk parameters
	/*后期转为共有*/ std::map<int, vtkSmartPointer<vtkLookupTable>> m_lutMap;
	vtkSmartPointer<vtkPolyData> m_polyData;
	vtkSmartPointer<vtkSphereSource> m_sphereSource;
	vtkSmartPointer<vtkCubeSource> m_cubeSource;
	int m_sourceType = 0;//0:spheresource 1:cubeSource
	vtkSmartPointer<vtkGlyph3D> m_glyph3d;
	//vtkSmartPointer<vtkLookupTable> m_lut;
	vtkSmartPointer<vtkPolyDataMapper> m_polyMapper;
	vtkSmartPointer<vtkActor> m_ScatterActor;
	//vtkSmartPointer<vtkScalarBarActor> m_scalarBar; // 0810 移至GraphNode
	vtkSmartPointer<vtkScalarBarWidget> m_scalarBarWidget;
	vtkSmartPointer<vtkLabeledDataMapper> m_pointLabelMapper;
	vtkSmartPointer<vtkActor2D> m_actor2dPointLabel;
};
