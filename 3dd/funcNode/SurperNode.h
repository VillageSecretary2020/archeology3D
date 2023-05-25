
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

#include "NodeParaters.h"

class XmlProject;
typedef std::list<std::string> stringlist;
typedef std::vector<pointFinal> POINT_FINAL_VEC;

stringlist stringSplit(std::string str, std::string sp);
stringlist stringSplit1(std::string str, char c);
bool xyz_exist(std::vector<std::string> nv, std::string str);

class SurperNode : public std::enable_shared_from_this<SurperNode>
{
protected:
	SurperNode(std::shared_ptr< XmlProject> project);
public:
	virtual ~SurperNode();
	NodeType getNodeType();
	NodeType getBelongType();
	std::string getNodeName();
	void setNodeName(std::string name);
	void setNodeAvailable(bool available = false);
	bool getNodeAvailable();
	virtual void setBaseInfo(std::shared_ptr<SuperNodeStruct> baseInfo);
	bool getChecked();
	virtual void setChecked(bool checked);
	virtual void setFocused(bool checked);	
	virtual bool setParameter(std::shared_ptr<SuperNodeStruct> Info);
	virtual void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0) = 0;
	virtual void setInputData(std::vector<pointFinal> &data) = 0;
	virtual bool getOnputData(std::vector<pointFinal> &data) = 0;
	virtual bool nextNodeRun(int8_t nodeId);
	virtual std::shared_ptr<SuperNodeStruct> getParameter();
	bool getParameter(std::shared_ptr<SuperNodeStruct> info);
	modelInfo getModelInfo();
	void setNodeIndex(int8_t index = RETURN_ERROR);
	int8_t getNodeIndex();
	bool addConnectRelation(int8_t nodeId, bool preLink = true);
	void removeConnectRelation(int8_t nodeId, bool preLink = true);
	std::list<int8_t> getNodeLinkedList(bool preLink = true);
	void setNodeLinkedList(std::list<int8_t> idList, bool preLink = true);
	virtual std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	static bool getIndexListFromstring(std::shared_ptr< XmlProject> project, std::string idstring, std::list<int8_t>& idList);
	static std::string getStrFromIndexList(std::shared_ptr<SurperNode> pNode, bool prelist = true);
	void setCompletionKey(bool completion);
	bool getCompletionKey();
	void setAvailable(bool available);
	bool getAvailable();
	void setBeshowed(bool beshow);
	bool getBeshowed();
	void setBechanged(bool bechanged);
	bool getBechanged();
	bool getExcuted() { return m_excuted; }
	virtual bool run() = 0;
	modelInfo m_info;
	std::list<int8_t> m_nodePreNodelist;
protected:
	void* widget= nullptr;
	NodeType m_nodeType;
	NodeType m_nodeBelongType;
	std::string m_nodeName;
	bool m_nodeAvailable;
	bool m_checked;	// 选中
	//bool m_statusChangeable;
	bool m_excuted; int m_ecount;
	int8_t m_nodeIndex;
	std::weak_ptr<XmlProject> m_project;
	std::list<int8_t> m_nodeNextNodeList;
	std::list<NodeType> m_nodeCanAppendNodeTypeList;
	
	uint8_t m_flags;
};
