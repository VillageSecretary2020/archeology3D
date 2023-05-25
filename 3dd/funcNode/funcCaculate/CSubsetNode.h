// CSubsetNode.h

#pragma once

#include "CaculateNode.h"
#include <map>

class CSubsetNode : public CaculateNode
{
public:
	CSubsetNode(std::shared_ptr< XmlProject> project);
	~CSubsetNode();
public:
	static void getDefault(SuperNodeStruct* st);
	bool setParameter(std::shared_ptr<SuperNodeStruct> st);
	bool getParameter(C_SubsetFilter & filter);
	std::shared_ptr<SuperNodeStruct> getParameter();
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);

	virtual int* getResolution()
	{
		return m_newResolution;
	}
	virtual bool getOnputData(std::vector<pointFinal> &data);

	bool run();
	void makeNextNodeRun1();

	void setMeshData(std::vector<pointFinal>* meshData, int resolution[3]);

private:
	/* Weak pointer */
	int m_newResolution[3];
	C_SubsetFilter m_filter;
	std::vector<pointFinal> m_filterData;
};
