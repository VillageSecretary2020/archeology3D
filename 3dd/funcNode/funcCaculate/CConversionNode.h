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

#include "CaculateNode.h"

class CConversionNode : public CaculateNode
{
public:
	CConversionNode(std::shared_ptr< XmlProject> project);
	~CConversionNode();
public:
	static void getDefault(SuperNodeStruct*);		// 取默认参数
	bool setParameter(std::shared_ptr<SuperNodeStruct> st);
	bool getParameter(C_Conversion & converson);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual bool getOnputData(std::vector<pointFinal> &data);
	bool run();
	//void makeNextNodeRun();
private:
	void getTransformMatrix(double matrix[4][4]);
	C_Conversion m_conversion;
	std::vector<pointFinal> m_transferData;
};
