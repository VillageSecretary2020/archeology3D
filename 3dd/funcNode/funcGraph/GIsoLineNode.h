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

class GIsoLineNode;

class GIsoLineNode : public GraphNode
{
public:
	GIsoLineNode(std::shared_ptr< XmlProject> project);
	~GIsoLineNode();

public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> isoLine=nullptr);
	bool getParameter(G_IsoLine &isoLine);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual vtkSmartPointer<vtkProp> getActor();
	bool run();
	bool addToRender(); // yzw 0610
	bool removeFromRender();
	void getBound(double bounds[6]);
	void setBound(double* bounds);
	virtual void setNewPos(double*);

	virtual void setInputData(std::vector<pointFinal> &data);
	void setLimits(double * limits)
	{
		m_excuted = false;
		memcpy_s(m_limits, sizeof(double) * 6, limits, sizeof(double) * 6);
	}

	void setResolution(int * slt)
	{
		m_excuted = false;
		memcpy_s(m_resolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	}

	virtual void setDataRange(double* dataRange);
private:
	void setCutPoint(double* p);
	void updateDisplay(std::shared_ptr<SuperNodeStruct> isoLine);
private:
	G_IsoLine m_isoLine;
	int m_resolution[3]; //解析度
	double m_limits[6];
	double m_dataRange[6];
	vtkSmartPointer<vtkActor> isoline_actor  ;
	vtkSmartPointer<vtkActor> outline_actor;

};
