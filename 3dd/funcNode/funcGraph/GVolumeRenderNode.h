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

class GVolumeRenderNode : public GraphNode
{
public:
	GVolumeRenderNode(std::shared_ptr< XmlProject> project);
	~GVolumeRenderNode();
public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> volumeRender);
	bool getParameter(G_VolumeRender &volumeRender);
	std::shared_ptr<SuperNodeStruct> getParameter();	// yzw 0629
	//void setBaseInfo(std::shared_ptr<SuperNodeStruct> baseInfo);
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender();	// yzw 0608
	bool removeFromRender();
	void makeNextNodeRun();
	void getBound(double bounds[6]);
	void setBound(double* bounds);
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
	void updateDisplay(std::shared_ptr<SuperNodeStruct> volumeRender);
private:
	G_VolumeRender m_volumeRender;
	int m_resolution[3]; //解析度
	double m_dataRange[6];

	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkPiecewiseFunction> m_compositeOpacity;
	vtkSmartPointer<vtkColorTransferFunction> m_color;
	vtkSmartPointer<vtkVolumeProperty> m_volumeProperty;
	vtkSmartPointer<vtkSmartVolumeMapper> m_mapper;
	vtkSmartPointer<vtkVolume> m_volume;
};
