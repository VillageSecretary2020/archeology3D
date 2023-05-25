/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
//
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "GraphNode.h"

class GIsoSurfaceNode : public GraphNode
{
public:
	GIsoSurfaceNode(std::shared_ptr< XmlProject> project);
	~GIsoSurfaceNode();
public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> isoSurface);
	bool getParameter(G_IsoSurface &isoSurface);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender(); // yzw 0610
	bool removeFromRender();
	
	void getBound(double bounds[6]);
	void setBound(double* bounds);

	virtual void setInputData(std::vector<pointFinal> &data);
	void setDataRange(double* dataRange)
	{
		m_excuted = false;
		memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	}
	void setResolution(int * slt)
	{
		m_excuted = false;
		memcpy_s(m_resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	}

private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> isoSurface);
	void makeNextNodeRun();
private:
	G_IsoSurface m_isoSurface;
	int m_resolution[3]; //解析度
	double m_dataRange[6];
	double m_scalarRange[2];

	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkClipVolume> m_clipper;
	vtkSmartPointer<vtkDataSetSurfaceFilter> m_surfaceFilter;
	vtkSmartPointer<vtkTriangleFilter> m_triangleFilter;
	vtkSmartPointer<vtkMassProperties> m_mass;
	vtkSmartPointer<vtkMarchingCubes> m_marchingCubes;
	vtkSmartPointer<vtkPolyDataMapper> m_mapper;
	vtkSmartPointer<vtkProperty> m_prop;
	vtkSmartPointer<vtkActor> m_isosurfaceActor;
};
