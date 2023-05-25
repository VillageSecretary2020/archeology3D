#pragma once

#include "CaculateNode.h"
#include <map>

class CRepeatFilterNode : public CaculateNode
{
public:
	CRepeatFilterNode(std::shared_ptr< XmlProject> project);
	~CRepeatFilterNode();
public:
	static void getDefault(SuperNodeStruct*);
	bool setParameter(std::shared_ptr<SuperNodeStruct> st);
	bool getParameter(C_RepeatFilter & filter);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	virtual bool getOnputData(std::vector<pointFinal> &data);
	bool run();
	
private:
	bool isPointDuplicate(int index1, int index2);

	C_RepeatFilter m_filter;
	std::vector<pointFinal> m_filterData;
};

