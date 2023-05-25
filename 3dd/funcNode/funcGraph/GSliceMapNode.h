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

class GSliceMapNode : public GraphNode
{
public:
	GSliceMapNode(std::shared_ptr< XmlProject> project);
	~GSliceMapNode();
public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> sliceMap);
	bool getParameter(G_SliceMap &sliceMap);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender();	// yzw 0608
	bool removeFromRender();
	void getBound(double bounds[6]);
	void setBound(double* bounds);
	virtual void setNewPos(double*);

	virtual void setInputData(std::vector<pointFinal> &data);
	virtual void setOriginData(POINT_FINAL_VEC *data);
	virtual void setDataRange(double* dataRange);
	void setResolution(int * slt)
	{
		memcpy_s(m_resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	}

private:
	void setCutPoint(double* p);
	void updateDisplay(std::shared_ptr<SuperNodeStruct> sliceMap);
	void makeNextNodeRun();
private:
	G_SliceMap m_sliceMap;
	int m_resolution[3]; //解析度
	double m_dataRange[6];
	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkPlane> m_centerPlane;
	vtkSmartPointer<vtkCutter> m_cutter;
	//vtkSmartPointer<vtkLookupTable> m_lut;// moved to  GraphNode 
	vtkSmartPointer<vtkPolyDataMapper> m_sliceMapper;
	vtkSmartPointer<vtkActor> m_actor;
	vtkSmartPointer<vtkPolyData> originalPolyData;
	vtkSmartPointer<vtkDelaunay3D> delaunay3D;
	vtkSmartPointer<vtkDataSetMapper> delaunayMapper;
	vtkSmartPointer<vtkActor> delaunayActor;
	vtkSmartPointer<vtkLoopSubdivisionFilter> subDiv;
	bool bfprobe_updated = false;
};
