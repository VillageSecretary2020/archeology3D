#pragma once

#include "CaculateNode.h"

class CExclusionFilterNode : public CaculateNode
{
public:
	CExclusionFilterNode(std::shared_ptr< XmlProject> project);
	~CExclusionFilterNode();
public:
	static void getDefault(SuperNodeStruct*);
	bool setParameter(std::shared_ptr<SuperNodeStruct> st);
	bool getParameter(C_EliminationFiler& filter);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual bool getOnputData(std::vector<pointFinal> &data);
	bool run();
	bool run1();

private:

	C_EliminationFiler m_filter;
	std::vector<pointFinal> m_filterData;
};

