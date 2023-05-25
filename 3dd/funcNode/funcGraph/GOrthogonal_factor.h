/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。 
//
// 文件功能描述：
// GOrthogonal_factor.h
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

class GOrthonalFactorNode : public GraphNode
{
public:
	GOrthonalFactorNode(std::shared_ptr<XmlProject> project);
	~GOrthonalFactorNode();

public:
	static void getDefault(SuperNodeStruct*);
	bool setParameter(std::shared_ptr<SuperNodeStruct> orthogonalfactor);
	bool getParameter(std::shared_ptr<SuperNodeStruct> orthogonalfactor);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor() { return vtkSmartPointer<vtkProp>(); }
	bool run();
	bool addToRender();	 // yzw 0610
	bool removeFromRender(); // yzw 0610
	void makeNextNodeRun();
	void getBound(double bounds[6]) {}
	void setBound(double* bounds) {}

	/*void setDataRange(double* dataRange)
	{
		memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	}*/
	/*void setSpacing(double * spacing)
	{
		//memcpy_s(m_spacing, sizeof(double) * 3, spacing, sizeof(double) * 6); //6是不是应该改为3
		memcpy_s(m_spacing, sizeof(double) * 3, spacing, sizeof(double) * 3);
	}*/
	void setLimits(double * limits)
	{
		m_excuted = false;
		memcpy_s(m_limits, sizeof(double) * 6, limits, sizeof(double) * 6);
	}

	virtual void setResolution(int * slt);

	void setDataRange(double* dataRange)
	{
		m_excuted = false;
		memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	}

private:
	void updateDisplay(std::shared_ptr<SuperNodeStruct> orthogonalFactor);

private:
	G_Orthogonal_Factor m_orthogonalFactor;
	int m_resolution[3]; //解析度
	double m_dataRange[6];
	//double m_spacing[3];
	double m_limits[6];

	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkExtractVOI> m_voi;
	//vtkSmartPointer<vtkLookupTable> m_lut;
	vtkSmartPointer<vtkDataSetMapper> m_mapper;
	vtkSmartPointer<vtkActor> m_orthoimage;
};
