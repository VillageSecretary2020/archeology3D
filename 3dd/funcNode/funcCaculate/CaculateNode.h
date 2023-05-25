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

#include "SurperNode.h"

class CaculateNode : public SurperNode
{
protected:
	CaculateNode(std::shared_ptr< XmlProject> project);
public:
	virtual ~CaculateNode();
	virtual std::shared_ptr<SuperNodeStruct> getParameter();
	virtual void setInputData(std::vector<pointFinal> &data);
	virtual bool getOnputData(std::vector<pointFinal> &data);
	void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0);

	double* getDataRange()
	{
		return m_dataRange;
	}
	void setDataRange(double* dataRange)
	{
		m_excuted = false;
		memcpy_s(m_dataRange, sizeof(double) * 6, dataRange, sizeof(double) * 6);
	}

	virtual int* getResolution()
	{
		return m_oldResolution;
	}

	void setResolution(int * slt)
	{
		m_excuted = false;
		memcpy_s(m_oldResolution, sizeof(int) * 3, slt, sizeof(int) * 3);
	}

	virtual double* getLimits()
	{
		return m_limits;
	}
	void setLimits(double * limits)
	{
		m_excuted = false;
		memcpy_s(m_limits, sizeof(double) * 6, limits, sizeof(double) * 6);
	}

	virtual void makeNextNodeRun();
	virtual bool nextNodeRun(int8_t nodeId);
	virtual bool saveData(std::string path);

protected:
	std::vector<pointFinal> m_data;
	double m_dataRange[6];
	double m_limits[6]; //Xmin,Ymin,Zmin,Xmax,Ymax,Zmax 
	int m_oldResolution[3];
	std::vector<pointFinal> m_caculateData;
};
