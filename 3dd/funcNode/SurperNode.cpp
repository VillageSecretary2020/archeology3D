#include "SurperNode.h"
#include "XmlProject.h"

stringlist stringSplit(std::string str, std::string sp)
{
	stringlist strList;
	int ic, sz = str.size();
	int z = 0; // 搜索非分割符
	int begin = 0;
	for (ic = 0; ic < sz; ic++)
	{
		char ch = str[ic];
		//if(ch== ' ' || ch== '\t' || ch== ',')
		std::string::size_type pos = sp.find(ch);
		if (pos != std::string::npos)
		{ 
			if (0 == z)
				continue;
			else {// 块结束了
				std::string s(str, begin, ic - begin);
				strList.push_back(s);
				z = 0;
			}
		}
		else {
			if(0== z){
				begin = ic;
				z=1;	// 搜索结束符
			}
		}
	}
	if(1==z && begin  < ic) {// 最后一块结束了
		std::string s(str, begin, ic - begin);
		strList.push_back(s);
	}

	return strList;
}

bool xyz_exist(std::vector<std::string> nv,  std::string str)
{
	for (size_t i = 0; i < nv.size(); i++) {
		if (nv[i].find(str) != std::string::npos)
			return true;
	}
	return false;
}

stringlist stringSplit1(std::string str, char c)
{
	stringlist strList;
	if (str.end() == find(str.begin(), str.end(), c))
	{
		strList.push_back(str);
		return strList;
	}

	int id = str.find(c);
	int begin = 0;
	while (-1 != id)
	{
		std::string s(str, begin, id - begin);
		begin = id + 1;
		if(!s.empty())
			strList.push_back(s);
		id = str.find(c, begin);
		if (-1 == id)
		{
			if (begin < str.size())
			{
				std::string s(str, begin);
				if(!s.empty())
					strList.push_back(s);
			}
		}
	}
	return strList;
}

SurperNode::SurperNode(std::shared_ptr< XmlProject> project)
{
	m_nodeAvailable = false;
	m_checked = true;
	m_excuted = false; m_ecount = 0;
	m_nodeIndex = RETURN_ERROR;
	m_project = project;
	m_nodeName.clear();
	m_flags = 0;
}

SurperNode::~SurperNode()
{
}

NodeType SurperNode::getNodeType()
{
	return m_nodeType;
}

NodeType SurperNode::getBelongType()
{
	return m_nodeBelongType;
}

std::string SurperNode::getNodeName()
{
	return m_nodeName;
}

void SurperNode::setNodeName(std::string name)
{
	m_nodeName = name;
}

void SurperNode::setNodeAvailable(bool available)
{
	m_nodeAvailable = available;
}

bool SurperNode::getNodeAvailable()
{
	return m_nodeAvailable;
}

void SurperNode::setBaseInfo(std::shared_ptr<SuperNodeStruct> baseInfo)
{
	m_info = baseInfo->m_info;
	m_nodeName = baseInfo->m_name;
	widget = baseInfo->widget;
}

bool SurperNode::getChecked()
{
	return m_checked;
}

void SurperNode::setChecked(bool checked)	// yzw 0715
{
	m_checked= checked;
}

void SurperNode::setFocused(bool checked)	
{
}

bool SurperNode::setParameter(std::shared_ptr<SuperNodeStruct> Info)
{
	if (!Info)
		return false;
	m_info = Info->m_info;
	m_nodeName = Info->m_name;
	widget = Info->widget;
	return true;
}

std::shared_ptr<SuperNodeStruct> SurperNode::getParameter()
{
	return nullptr;
}

bool SurperNode::getParameter(std::shared_ptr<SuperNodeStruct> info)
{
	if(!info)
		return false;
	info->m_info = m_info;
	info->m_name = m_nodeName;
	info->m_nodeIndex = m_nodeIndex;
	return true;
}

modelInfo SurperNode::getModelInfo()
{ 
	return m_info;
}


void SurperNode::setNodeIndex(int8_t index)
{
	m_nodeIndex = index;
}

int8_t SurperNode::getNodeIndex()
{
	return m_nodeIndex;
}

bool SurperNode::addConnectRelation(int8_t nodeId, bool preLink)
{
	if (preLink)
	{
		for (auto id : m_nodePreNodelist)
		{
			if (id == nodeId)
			{
				return false;
			}
		}
		m_nodePreNodelist.push_back(nodeId);
	}
	else
	{
		for (auto id : m_nodeNextNodeList)
		{
			if (id == nodeId)
			{
				return false;
			}
		}
		m_nodeNextNodeList.push_back(nodeId);
		if(m_excuted)
			nextNodeRun(nodeId);
	}
	return true;
}

void SurperNode::removeConnectRelation(int8_t nodeId, bool preLink)
{
	if (preLink)
	{
		m_nodePreNodelist.remove(nodeId);
	}
	else
	{
		m_nodeNextNodeList.remove(nodeId);
	}
}

std::list<int8_t> SurperNode::getNodeLinkedList(bool preLink)
{
	if (preLink)
		return m_nodePreNodelist;
	else
		return m_nodeNextNodeList;
}

void SurperNode::setNodeLinkedList(std::list<int8_t> idList, bool preLink)
{
	if (preLink)
		m_nodePreNodelist = idList;
	else
		m_nodeNextNodeList = idList;
}

std::shared_ptr<TiXmlNode> SurperNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> node(new TiXmlElement("Node"));
	std::shared_ptr<TiXmlElement> pElement = (std::dynamic_pointer_cast<TiXmlElement>(node));
	pElement->SetAttribute("type", m_nodeType);
	pElement->SetAttribute("name", m_nodeName.data());
	pElement->SetAttribute("index", m_nodeIndex);
	pElement->SetAttribute("checked", m_checked);
	std::shared_ptr<TiXmlElement> preNodeList(new TiXmlElement("PreNode"));
	preNodeList->SetAttribute("preNodeIndex", getStrFromIndexList(shared_from_this(), true).data());
	std::shared_ptr<TiXmlElement> pNextNodeList(new TiXmlElement("NextNode"));
	pNextNodeList->SetAttribute("nextNodeIndex", getStrFromIndexList(shared_from_this(), false).data());
	std::shared_ptr<TiXmlElement> pPosInfo(std::make_shared<TiXmlElement>("Info"));
	//pPosInfo->SetAttribute("leftTopPosX", m_info.m_leftTopPosX);
	//pPosInfo->SetAttribute("leftTopPosY", m_info.m_leftTopPosY);
	//pPosInfo->SetAttribute("width", m_info.m_width);
	//pPosInfo->SetAttribute("length", m_info.m_length);
	m_info.toXml(pPosInfo);	// yzw 0701
	std::shared_ptr<TiXmlElement> pExtraInfo(std::make_shared<TiXmlElement>("ExtraInfo"));
	pExtraInfo->SetAttribute("NodeAvailable", m_nodeAvailable);
	node->InsertEndChild(*preNodeList);
	node->InsertEndChild(*pNextNodeList);
	node->InsertEndChild(*pPosInfo);
	node->InsertEndChild(*pExtraInfo);
	return node;
}

bool SurperNode::readParameterFromXmlNode(TiXmlNode* node)
{
	if(!node)
		return false;
	TiXmlElement* pElement = (TiXmlElement*)node;
	m_nodeName = pElement->Attribute("name");
	if (TIXML_SUCCESS != pElement->QueryIntAttribute("index", (int*)&m_nodeIndex))
		return false;
	if (TIXML_SUCCESS != pElement->QueryBoolAttribute("checked", &m_checked))
		return false;
	TiXmlElement* pPreNode = pElement->FirstChildElement("PreNode");
	if (!pPreNode)
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	std::string NodeList = pPreNode->Attribute("preNodeIndex");
	if (!getIndexListFromstring(m_project.lock(), NodeList, m_nodePreNodelist))
		return false;
	NodeList.clear();
	TiXmlElement* pNextNode = (pPreNode->NextSiblingElement("NextNode"));
	if (!pNextNode)
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	NodeList = pNextNode->Attribute("nextNodeIndex");
	if (!getIndexListFromstring(m_project.lock(), NodeList, m_nodeNextNodeList))
		return false;
	TiXmlElement* pPosInfo = pNextNode->NextSiblingElement("Info");
	if (!pPosInfo)
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	// yzw 0701
	if (TIXML_SUCCESS != m_info.fromXml(pPosInfo))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	/*
	if (TIXML_SUCCESS != pPosInfo->QueryIntAttribute("leftTopPosX", &m_info.m_leftTopPosX))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	if (TIXML_SUCCESS != pPosInfo->QueryIntAttribute("leftTopPosY", &m_info.m_leftTopPosY))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	if (TIXML_SUCCESS != pPosInfo->QueryIntAttribute("width", &m_info.m_width))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	if (TIXML_SUCCESS != pPosInfo->QueryIntAttribute("length", &m_info.m_length))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}*/
	TiXmlElement* pExtraInfo = pPosInfo->NextSiblingElement("ExtraInfo");
	if (!pExtraInfo)
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	if(TIXML_SUCCESS != pExtraInfo->QueryBoolAttribute("NodeAvailable", &m_nodeAvailable))
	{
		m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return false;
	}
	return true;
}

bool SurperNode::getIndexListFromstring(std::shared_ptr<XmlProject> project, std::string idstring, std::list<int8_t>& idList)
{
	idList.clear();
	if (idstring.empty())
	{
		return true;
	}
	stringlist ls = stringSplit(idstring, ",");
	for  (std::string str : ls)
	{
		try
		{
			idList.push_back(std::stoi(str));
		}
		catch (const std::invalid_argument&)
		{
			project->setProjectError(ProjectErrorType::ERROR_PARSE_XML_PREID_LIST);
			return false;
		}
	}
	return true;
}

std::string SurperNode::getStrFromIndexList(std::shared_ptr<SurperNode> pNode, bool prelist)
{
	std::string str;
	str.clear();
	std::list<int8_t> idlist;
	idlist.clear();
	if (prelist)
		idlist = pNode->getNodeLinkedList();
	else
		idlist = pNode->getNodeLinkedList(false);

	if (idlist.empty())
		return str;
    for  (auto var : idlist)//for each (int var in idlist)
	{
		str += std::to_string(var);
		str += ',';
	}
	return str;
}

void SurperNode::setCompletionKey(bool completion)
{

}

bool SurperNode::getCompletionKey()
{
	return false;
}

void SurperNode::setAvailable(bool available)
{
	m_nodeAvailable = available;
}

bool SurperNode::getAvailable()
{
	return false;
}

void SurperNode::setBeshowed(bool beshow)
{
}

bool SurperNode::getBeshowed()
{
	return false;
}

void SurperNode::setBechanged(bool bechanged)
{
}

bool SurperNode::getBechanged()
{
	return false;
}

bool SurperNode::nextNodeRun(int8_t nodeId)
{
	return true;
}
