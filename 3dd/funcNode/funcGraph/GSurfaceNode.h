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

class GSurfaceNode : public GraphNode
{
	typedef void (XYZLimits_cb)(int* limits, void* widget);
	typedef void (updata_info_cb)(int num_blocks, float average_blocks, float volume, void* widget);
public:
	GSurfaceNode(std::shared_ptr<XmlProject> project);
	~GSurfaceNode();

public:
	void setCallbackFun(XYZLimits_cb* limits_func, updata_info_cb* info_func, void* ptr);
	bool setParameter(std::shared_ptr<SuperNodeStruct> surface);
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }

	bool run();
	bool addToRender();	// yzw 0608
	bool removeFromRender();

	void makeNextNodeRun();
	void getBound(double bounds[6])
	{

	}
	void setBound(double* bounds) {}

	void setDataRange(double* dataRange)
	{
		m_excuted = false;
		memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	}
	void setResolution(int * slt)
	{
		m_excuted = false;
		memcpy_s(m_resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
		if(m_call_fun)
			m_call_fun(slt, m_ptr);
	}
private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> scatterMap);
	double caculateMeanBlocks(int* X_limits, int* Y_limits, int* Z_limits);
private:
	G_SurfaceRender m_surfaceRender;
	////vtk parameters
	/*后期转为共有*/ std::map<int, vtkSmartPointer<vtkLookupTable>> m_lutMap;
	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkExtractVOI> m_surfaceVOI;
	//vtkSmartPointer<vtkLookupTable> m_lut;
	vtkSmartPointer<vtkLookupTable> m_edgeLut;
	vtkSmartPointer<vtkDataSetMapper> m_dataSetMapper;
	vtkSmartPointer<vtkActor> m_surfaceActor;
	vtkSmartPointer<vtkActor> m_edgeActor;
	vtkSmartPointer<vtkDataSetMapper> m_edgeMapper;
	int m_resolution[3]; //解析度
	double m_dataRange[6];

	XYZLimits_cb* m_call_fun = nullptr;
	updata_info_cb* m_info_cb = nullptr;
	void * m_ptr = nullptr;
};
