/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
// CMeshNode.h
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "CaculateNode.h"

class CMeshNode : public CaculateNode
{
public:
	CMeshNode(std::shared_ptr< XmlProject> project);
	~CMeshNode();

public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> mesh);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	bool getOnputData(std::vector<pointFinal> &data);
	double* getDataRange()
	{
		return m_dataRange;
	}
	int* getResolution()
	{
		return m_mesh.m_geometry.resolution;
	}
	virtual double* getLimits()
	{
		return m_mesh.m_geometry.limits;
	}
	double* getspacing()
	{
		return m_mesh.m_geometry.space;
	}
	bool run();
	
private:
	C_Mesh m_mesh;
	
	bool set_progress(int range, int pos, const char*);
};
