#include "XmlProject.h"
#include "OperationLogic.h"
#include "OperationalLayer.h"

XmlProject::XmlProject(int8_t ProjectId, callBack* call)
{
	m_errorList.clear();
	m_nodeMap.clear();
	m_readXmlMapTable.clear();
	m_ListNodeNum = 0;
	m_isSaved = false;
	m_projectId = ProjectId;
	m_callBack = call;
	m_ProjectXml.clear();
	m_projectName.clear();
	m_error = RETURN_NORMAL;
	m_addPropList.clear();
	m_removePropList.clear();
	m_taskList.clear();
}

XmlProject::XmlProject(std::string xmlPath, int8_t ProjectId, callBack* call)
{
	m_nodeMap.clear();
	m_readXmlMapTable.clear();
	m_ListNodeNum = 0;
	m_isSaved = false;
	m_projectId = ProjectId;
	m_ProjectXml = xmlPath;
	m_error = RETURN_NORMAL;
	m_callBack = call;
	m_projectName.clear();
	m_taskList.clear();
	m_workThreadObj = new workRunThread;
	m_workThreadObj->setProjectPtr(this);
}

XmlProject::~XmlProject()
{
	m_nodeMap.clear();
	m_taskList.clear();
}

void XmlProject::setRender(vtkSmartPointer<vtkRenderer> render)
{
	m_render = render;
	if(!m_workThreadObj)	// yzw 0525 可能已经有了
		m_workThreadObj = new workRunThread;
	m_workThreadObj->setProjectPtr(this);
}

void XmlProject::setRenderWinInteractor(vtkSmartPointer<vtkRenderWindowInteractor> winInteractor)
{
	m_WinInteractor = winInteractor;
}

vtkSmartPointer<vtkRenderer> XmlProject::getRender()
{
	return m_render;
}

vtkSmartPointer<vtkRenderWindowInteractor> XmlProject::getWinInteractor()
{
	return m_WinInteractor;
}

int8_t XmlProject::getProjectId()
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	return m_projectId;
}

int8_t XmlProject::addNode(std::shared_ptr<SuperNodeStruct> pNodeInfo)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	std::shared_ptr<SurperNode> pNode = createSurperNode(pNodeInfo);
	pNode->setParameter(pNodeInfo);
	setProjectSavedStatus();
	pNode->setNodeIndex(++m_ListNodeNum);
	m_nodeMap.insert(std::pair<int8_t, std::shared_ptr<SurperNode>>(pNode->getNodeIndex(), pNode));
	return pNode->getNodeIndex();
}

void XmlProject:: addFuncTask(BOOL_FUNC pf)
{
	m_workThreadObj->addFuncTask(pf);
}

void XmlProject::caculateMesh(int meshId)
{
	//业务上保证每次同一个网格化分支只能执行一次，可在界面做处理，也可在后端处理，
	//此处未做判断处理，可同时添加多次相同计算分支，会被连续计算多次，无线程风险，但操作重复
	std::shared_ptr<SurperNode> meshNode = getSpecifiedNode(meshId);
	std::shared_ptr<CMeshNode> meshnode = std::dynamic_pointer_cast<CMeshNode>(meshNode);
	if (!meshnode)
		return;
	std::function<bool()> _func = std::bind(&CMeshNode::run, meshnode.get());
	m_workThreadObj->addFuncTask(_func);
}
void XmlProject::initTaskList()
{}

int XmlProject::addNode(std::shared_ptr<SurperNode> pNode)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	setProjectSavedStatus();
	pNode->setNodeIndex(++m_ListNodeNum);
	m_nodeMap.insert(std::pair<int8_t, std::shared_ptr<SurperNode>>(pNode->getNodeIndex(), pNode));
	return RETURN_NORMAL;
}

int XmlProject::removeNode(int8_t nodeId)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	setProjectSavedStatus();

	removeNodeAndConnectRelation(nodeId);
	return RETURN_NORMAL;
}
#if 0
int XmlProject::addConnectRelation(int8_t firstId, int8_t secondId)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	setProjectSavedStatus();

	std::shared_ptr<SurperNode> firstNode = getNodePtr(firstId);
	std::shared_ptr<SurperNode> secondNode = getNodePtr(secondId);
	firstNode->addConnectRelation(secondId, false);
	secondNode->addConnectRelation(firstId);
	initTaskList();
	return RETURN_NORMAL;
}
#endif
int XmlProject::removeConnectRelation(int8_t firstId, int8_t secondId)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	setProjectSavedStatus();
	std::shared_ptr<SurperNode> firstNode = getNodePtr(firstId);
	std::shared_ptr<SurperNode> secondNode = getNodePtr(secondId);
	firstNode->removeConnectRelation(secondId, false);
	secondNode->removeConnectRelation(firstId);
	initTaskList();
	return RETURN_NORMAL;
}

bool XmlProject::checkProjectSaved()
{
	if (!checkProjectHasError())
	{
		return false;
	}
	if (m_ProjectXml.empty())
	{
		return false;
	}
	return m_isSaved;
}

int8_t XmlProject::saveProject(std::string xmlPath)
{
	if (!checkProjectHasError())
	{
		return RETURN_ERROR;
	}
	if (RETURN_NORMAL != saveNodeList(xmlPath))
	{
		return RETURN_ERROR;
	}
	setProjectSavedStatus(true);
	return RETURN_NORMAL;
}

stringlist XmlProject::getProjectErrorList()
{
	stringlist errorlist = m_errorList;
	updateProjectError();
	return errorlist;
}

bool XmlProject::setProjectError(ProjectError errorCode)
{
	switch (errorCode)
	{
	case RETURN_NORMAL:
	{
		m_errorList.push_back("no error");
	}
	break;
	case ProjectErrorType::ERROR_PARSE_XML_PREID_LIST:
	{
		m_errorList.push_back("parse xml node preId list failed.");
	}
	break;
	case ProjectErrorType::ERROR_PARSE_XML_NEXTID_LIST:
	{
		m_errorList.push_back("parse xml node nextId list failed.");
	}
	break;
	case ProjectErrorType::ERROR_PARSE_MESH_PARAMETER:
	{
		m_errorList.push_back("parse xml mesh node parameters failed.");
	}
	break;
	case ProjectErrorType::ERROR_WRITE_MESH_PARAMETER:
	{
		m_errorList.push_back("write xml mesh node parameters failed.");
	}
	break;
	case ProjectErrorType::ERROR_NO_PATH_TO_SAVE:
	{
		m_errorList.push_back("save project error: no path to save");
	}
	break;
	case ProjectErrorType::ERROR_XML_PARSE_FAILED:
	{
		m_errorList.push_back("parse xml failed");
	}
	break;
	case ProjectErrorType::ERROR_XML_SAVE_FAILED:
	{
		m_errorList.push_back("save xml failed");
	}
	break;
	case ProjectErrorType::ERROR_XML_NO_NODE:
	{
		m_errorList.push_back("xml file has no node to parse");
	}
	break;
	default:
	{
		m_errorList.push_back("this error code no define, please add it");
	}
	break;
	}
	if (m_hasError)
	{
		m_callBack(m_pLogic, true);
		m_hasError = true;
	}
	return true;//无意义
}

void XmlProject::setProjectName(std::string projectName)
{
	m_projectName = projectName;
}

std::string XmlProject::getProjectName()
{
	return m_projectName;
}

bool XmlProject::run(std::shared_ptr<SurperNode> runNode)
{
	//todo
	return false;
}

int8_t XmlProject::getNextAloneNodeId(int8_t nodeId)
{
	while(1){
		auto itr = m_nodeMap.find(++nodeId);
		if (m_nodeMap.end() == itr)
			return 0;
		if (itr->second->m_nodePreNodelist.size())
			continue;
		return nodeId;	
	}
	return 0;
}

std::shared_ptr<SurperNode> XmlProject::getSpecifiedNode(int8_t nodeId)
{
	auto itr = m_nodeMap.find(nodeId);
	if (m_nodeMap.end() == itr)
		return std::shared_ptr<SurperNode>();
	return itr->second;
}

void XmlProject::updateProjectError()
{
	m_errorList.clear();
	m_callBack(m_pLogic, false);
}

void XmlProject::setProjectSavedStatus(bool isSave)
{
	m_isSaved = isSave;
}

void XmlProject::removeNodeAndConnectRelation(int8_t nodeId)
{
	std::shared_ptr<SurperNode> tempNode = m_nodeMap[nodeId];
	m_nodeMap.erase(nodeId);
	if (tempNode)
	{
		std::list<int8_t> preLinkList = tempNode->getNodeLinkedList();
		std::list<int8_t> lastLinkList = tempNode->getNodeLinkedList(false);

		for (auto node : m_nodeMap)
		{
			if (!preLinkList.empty())
			{
				for (auto id : preLinkList)
				{
					if (node.second->getNodeIndex() == id)
					{
						node.second->removeConnectRelation(tempNode->getNodeIndex(), false);
						break;
					}
				}
			}
			if (!lastLinkList.empty())
			{
				for (auto id : lastLinkList)
				{
					if (node.second->getNodeIndex() == id)
					{
						node.second->removeConnectRelation(tempNode->getNodeIndex());
						break;
					}
				}
			}

		}
	}
	return;
}

int XmlProject::saveNodeList(std::string xmlPath)
{
	if (xmlPath.empty() && m_ProjectXml.empty())
	{
		setProjectError(ProjectErrorType::ERROR_NO_PATH_TO_SAVE);

		return RETURN_ERROR;
	}
	const char* demoStart =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<project>\n"
		"<info></info>"
		"<file></file>"
		"</project>";
	if (xmlPath.empty())
	{
		xmlPath = m_ProjectXml;
	}
	TiXmlDocument doc(xmlPath.data());
	doc.Parse(demoStart);
	if (doc.Error())
	{
		setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return RETURN_ERROR;
	}
	TiXmlElement* curNode = doc.FirstChildElement("project");
	if (!curNode)
	{
		setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return RETURN_ERROR;
	}

	for (auto var : m_nodeMap)
	{
		std::shared_ptr<TiXmlNode> node = var.second->CreateXmlNode();
		std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(node);
		curNode = (TiXmlElement*)doc.InsertAfterChild(curNode, *pElemNode);	// yzw 0704 按加入顺序
	}
	if (!doc.SaveFile())
	{
		//std::cout << doc.ErrorDesc() << std::endl;
		setProjectError(ProjectErrorType::ERROR_XML_SAVE_FAILED);
		return RETURN_ERROR;
	}
	return RETURN_NORMAL;
}

int XmlProject::initNodeList(std::string xmlPath)
{
	TiXmlDocument doc(xmlPath.data());
	if (!doc.LoadFile())
	{
		setProjectError(ProjectErrorType::ERROR_XML_PARSE_FAILED);
		return RETURN_ERROR;
	}
	TiXmlPrinter printer;
	doc.Accept(&printer);
	fprintf(stdout, "%s", printer.CStr());
	if (RETURN_NORMAL != initProjectInfo(doc))
	{
		return RETURN_ERROR;
	}
	bool hasNode = false;
	TiXmlElement * pElement = doc.FirstChildElement("Node");
	while (pElement)
	{
		if (!pElement)
		{
			break;
		}
		int type = RETURN_ERROR;
		if (TIXML_SUCCESS != pElement->QueryIntAttribute("type", &type))
		{
			setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
			return RETURN_ERROR;
		}
		std::shared_ptr<SurperNode> pNode = createNodeFromElement(pElement);
		if (!pNode)
		{
			setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
			return RETURN_ERROR;
		}
		{
			int8_t _id = pNode->getNodeIndex();
			addNode(pNode);
			m_readXmlMapTable.insert(std::pair<int8_t, int8_t>(_id, pNode->getNodeIndex()));
		}
		pElement = pElement->NextSiblingElement("Node");
	}
	updateNodePreNextIdList();
	return RETURN_NORMAL;
}

std::shared_ptr<SurperNode> XmlProject::createNodeFromElement(TiXmlElement* element)
{
	std::shared_ptr<SurperNode> pNode;
	int type = RETURN_ERROR;
	element->QueryIntAttribute("type", &type);
	switch (type)
	{
	case NodeType::NODE_C_CONVERSION_TYPE:
	{
		pNode = std::make_shared<CConversionNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_C_ELIMINATIONFILTER_TYPE:
	{
		pNode = std::make_shared<CExclusionFilterNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_C_FILTER:
	{
		//pNode = std::make_shared<>(shared_from_this());
	}
	break;
	case NodeType::NODE_C_MERGE_TYPE:
	{
	}
	break;
	case NodeType::NODE_C_MESH_TYPE:
	{
		pNode = std::make_shared<CMeshNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_C_REPEATFILER:
	{
		pNode = std::make_shared<CRepeatFilterNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_C_SUBSET_TYPE:
	{
		pNode = std::make_shared<CSubsetNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_D_DATASOURCE_TYPE:
	{
		pNode = std::make_shared<DDataNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_AXIS_TYPE:
	{
		pNode = std::make_shared<GAxis>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_BOUNDINGBOX_TYPE:
	{
		pNode = std::make_shared<GBoundingBoxNode>(shared_from_this());
	}
	break;
	//case NodeType::NODE_G_CLIPBOARD_TYPE:
	//{
	//}
	//break;
	case NodeType::NODE_G_COMMENT_TYPE:
	{
		pNode = std::make_shared<GCommentNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_CURVE_SLICE_TYPE:
	{
		pNode = std::make_shared<GCurveSliceNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_HIGHANDLOWDIAGRAM_TYPE:
	{
		pNode = std::make_shared<GHighLowMapNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_ISOLINE_TYPE:
	{
		pNode = std::make_shared<GIsoLineNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_ISOSURFACE_TYPE:
	{
		pNode = std::make_shared<GIsoSurfaceNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_ORTHOGONALFACTOR_TYPE:
	{
		pNode = std::make_shared<GOrthonalFactorNode>(shared_from_this());
	}
	break;
	case NODE_G_DXF_TYPE: {
			pNode = std::make_shared<GDxfNode>(shared_from_this());
	}break;
	case NODE_G_GEOTIFF_TYPE: {
			pNode = std::make_shared<GGeotiffNode>(shared_from_this());
	}break;
	case NODE_G_PICTURE_TYPE: {
			pNode = std::make_shared<GPictureNode>(shared_from_this());
	}break;
	case NodeType::NODE_G_SCATTERMAP_TYPE:
	{
		pNode = std::make_shared<GScatterMapNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_SLICEMAP_TYPE:
	{
		pNode = std::make_shared<GSliceMapNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_SURFACERENDER_TYPE:
	{
		pNode = std::make_shared<GSurfaceNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_TEXT_TYPE:
	{
		pNode = std::make_shared<GTextNode>(shared_from_this());
	}
	break;
	case NodeType::NODE_G_VOLUMERENDERING_TYPE:
	{
		pNode = std::make_shared<GVolumeRenderNode>(shared_from_this());
	}
	break;
	default:
	{
		setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	}
	break;
	}
	if (pNode)
	{
		pNode->readParameterFromXmlNode(element);
	}
	return pNode;
}

int XmlProject::initProjectInfo(TiXmlDocument & doc)
{
	//todo
	return RETURN_NORMAL;
}

void XmlProject::updateNodePreNextIdList()
{
	auto func = [&](std::list<int8_t> IdList, std::pair<int8_t, std::shared_ptr<SurperNode>> nodePair, bool preLink = true)
	{
		std::list<int8_t>::iterator it;
		if (!IdList.empty())
		{
			for (it = IdList.begin(); it != IdList.end(); it++)
			{
				*it = transformId(*it);
			}
			nodePair.second->setNodeLinkedList(IdList, preLink);
		}
	};
	for (auto nodePair : m_nodeMap)
	{
		std::list<int8_t> IdList = nodePair.second->getNodeLinkedList();
		func(IdList, nodePair);
		IdList.clear();
		IdList = nodePair.second->getNodeLinkedList(false);
		func(IdList, nodePair, false);
	}
}

int8_t XmlProject::transformId(int8_t oldId)
{
	return m_readXmlMapTable[oldId];
}

std::shared_ptr<SurperNode> XmlProject::createSurperNode(std::shared_ptr<SuperNodeStruct> pNodeInfo)
{
	std::shared_ptr<SurperNode> pNode;
	switch (pNodeInfo->m_nodeType)
	{
	case NODE_NODEFINE_TYPE:
		break;
	case NODE_D_DATASOURCE_TYPE:
	{
		std::shared_ptr<DDataNode> Node = std::make_shared<DDataNode>(shared_from_this());
		//Node->setParameter(pNodeInfo);
		pNode = Node;
	}
	break;
	case NODE_G_TYPE:
		break;
	case NODE_G_CURVE_SLICE_TYPE:
	{
		std::shared_ptr<GCurveSliceNode> Node = std::make_shared<GCurveSliceNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_SLICEMAP_TYPE:
	{
		std::shared_ptr<GSliceMapNode> Node = std::make_shared<GSliceMapNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_VOLUMERENDERING_TYPE:
	{
		std::shared_ptr<GVolumeRenderNode> Node = std::make_shared<GVolumeRenderNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_SCATTERMAP_TYPE:
	{
		std::shared_ptr<GScatterMapNode> Node = std::make_shared<GScatterMapNode>(shared_from_this());
		pNode = Node;
	}
	break;
	case NODE_G_TEXT_TYPE:
	{
		std::shared_ptr<GTextNode> Node = std::make_shared<GTextNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_ORTHOGONALFACTOR_TYPE:
	{
		std::shared_ptr<GOrthonalFactorNode> Node = std::make_shared<GOrthonalFactorNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_COMMENT_TYPE:
	{
		std::shared_ptr<GCommentNode> Node = std::make_shared<GCommentNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_ISOLINE_TYPE:
	{ // yzw 0610
		std::shared_ptr<GIsoLineNode> Node = std::make_shared<GIsoLineNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_ISOSURFACE_TYPE:
	{
		std::shared_ptr<GIsoSurfaceNode> Node = std::make_shared<GIsoSurfaceNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_SURFACERENDER_TYPE:
	{
		std::shared_ptr<GSurfaceNode> Node = std::make_shared<GSurfaceNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_AXIS_TYPE:
	{	
		std::shared_ptr<GAxis> Node = std::make_shared<GAxis>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_BOUNDINGBOX_TYPE:
	{
		std::shared_ptr<GBoundingBoxNode> Node = std::make_shared<GBoundingBoxNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_HIGHANDLOWDIAGRAM_TYPE:
	{
		std::shared_ptr<GHighLowMapNode> Node = std::make_shared<GHighLowMapNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_G_DXF_TYPE: {
		std::shared_ptr<GDxfNode> Node = std::make_shared<GDxfNode>(shared_from_this());
		pNode = Node;
	}break;
	case NODE_G_GEOTIFF_TYPE: {
		std::shared_ptr<GGeotiffNode> Node = std::make_shared<GGeotiffNode>(shared_from_this());
		pNode = Node;
	}break;
	case NODE_G_PICTURE_TYPE://NODE_G_CLIPBOARD_TYPE:
	{
		std::shared_ptr<GPictureNode> Node = std::make_shared<GPictureNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_C_TYPE:
		break;
	case NODE_C_CONVERSION_TYPE:
	{
		std::shared_ptr<CConversionNode> Node = std::make_shared<CConversionNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_C_MERGE_TYPE:
		break;
	case NODE_C_SUBSET_TYPE:
	{
		std::shared_ptr<CSubsetNode> Node = std::make_shared<CSubsetNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_C_ELIMINATIONFILTER_TYPE:
	{
		std::shared_ptr<CExclusionFilterNode> Node = std::make_shared<CExclusionFilterNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_C_MESH_TYPE:
	{
		std::shared_ptr<CMeshNode> Node = std::make_shared<CMeshNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_C_FILTER:
		break;
	case NODE_C_REPEATFILER:
	{
		std::shared_ptr<CRepeatFilterNode> Node = std::make_shared<CRepeatFilterNode>(shared_from_this());
		pNode = Node;
	}
		break;
	case NODE_O_TYPE:
		break;
	case NODE_O_COLORBAR_TYPE:
		break;
	case NODE_O_CUTTINGPLANE_TYPE:
		break;
	default:
		break;
	}

	return pNode;
}

std::list<std::shared_ptr<SuperNodeStruct>> XmlProject::getNodeListInfo()
{
	std::list<std::shared_ptr<SuperNodeStruct>> list;
	list.clear();
	for (auto var : m_nodeMap)
	{
		list.push_back(getNodeParameter(var.second));
	}
	return list;
}

std::multimap<int8_t, int8_t> XmlProject::getNodeListConnection()
{
	std::multimap<int8_t, int8_t> _map;
	_map.clear();
	for (auto var : m_nodeMap)
	{
		std::list<int8_t> nextLinkedIdList = var.second->getNodeLinkedList(false);
		for  (auto obj : nextLinkedIdList)
		{
			_map.insert(std::pair<int8_t, int8_t>(var.first, obj));
		}
	}
	return _map;
}

std::shared_ptr<SuperNodeStruct> XmlProject::getNodeParameter(std::shared_ptr<SurperNode> node)
{
	return node->getParameter();
	switch (node->getNodeType())
	{
	case NODE_NODEFINE_TYPE:
		break;
	case NODE_D_DATASOURCE_TYPE:
	{
		std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
	break;
	case NODE_G_TYPE:
		break;
	case NODE_G_CURVE_SLICE_TYPE:
	{
		std::shared_ptr<GCurveSliceNode> node1 = std::dynamic_pointer_cast<GCurveSliceNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_SLICEMAP_TYPE:
	{
		std::shared_ptr<GSliceMapNode> node1 = std::dynamic_pointer_cast<GSliceMapNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_VOLUMERENDERING_TYPE:{	
		std::shared_ptr<GVolumeRenderNode> node1 = std::dynamic_pointer_cast<GVolumeRenderNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
		}break;
	case NODE_G_SCATTERMAP_TYPE:
	{
		std::shared_ptr<GScatterMapNode> node1 = std::dynamic_pointer_cast<GScatterMapNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
	break;
	case NODE_G_TEXT_TYPE:
	{
		std::shared_ptr<GTextNode> node1 = std::dynamic_pointer_cast<GTextNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_ORTHOGONALFACTOR_TYPE:
	{
		std::shared_ptr<GOrthonalFactorNode> node1 = std::dynamic_pointer_cast<GOrthonalFactorNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_COMMENT_TYPE:
	{
		std::shared_ptr<GCommentNode> node1 = std::dynamic_pointer_cast<GCommentNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_ISOLINE_TYPE:
	{
		std::shared_ptr<GIsoLineNode> node1 = std::dynamic_pointer_cast<GIsoLineNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_ISOSURFACE_TYPE:
	{
		std::shared_ptr<GIsoSurfaceNode> node1 = std::dynamic_pointer_cast<GIsoSurfaceNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_SURFACERENDER_TYPE:
	{
		std::shared_ptr<GSurfaceNode> node1 = std::dynamic_pointer_cast<GSurfaceNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_AXIS_TYPE:
	{
		std::shared_ptr<GAxis> node1 = std::dynamic_pointer_cast<GAxis>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_BOUNDINGBOX_TYPE:
	{
		std::shared_ptr<GBoundingBoxNode> node1 = std::dynamic_pointer_cast<GBoundingBoxNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_HIGHANDLOWDIAGRAM_TYPE:
	{
		std::shared_ptr<GHighLowMapNode> node1 = std::dynamic_pointer_cast<GHighLowMapNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}break;
	case NODE_G_DXF_TYPE:
	{
		std::shared_ptr<GDxfNode> node1 = std::dynamic_pointer_cast<GDxfNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_GEOTIFF_TYPE:
	{
		std::shared_ptr<GGeotiffNode> node1 = std::dynamic_pointer_cast<GGeotiffNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_G_PICTURE_TYPE://NODE_G_CLIPBOARD_TYPE:
	{
		std::shared_ptr<GPictureNode> node1 = std::dynamic_pointer_cast<GPictureNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_C_TYPE:
		break;
	case NODE_C_CONVERSION_TYPE:
	{
		std::shared_ptr<CConversionNode> node1 = std::dynamic_pointer_cast<CConversionNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_C_MERGE_TYPE:
		break;
	case NODE_C_SUBSET_TYPE:
	{
		std::shared_ptr<CSubsetNode> node1 = std::dynamic_pointer_cast<CSubsetNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_C_ELIMINATIONFILTER_TYPE:
	{
		std::shared_ptr<CExclusionFilterNode> node1 = std::dynamic_pointer_cast<CExclusionFilterNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_C_MESH_TYPE:
	{
		std::shared_ptr<CMeshNode> node1 = std::dynamic_pointer_cast<CMeshNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
		}
						  break;
	case NODE_C_FILTER:
		break;
	case NODE_C_REPEATFILER:
	{
		std::shared_ptr<CRepeatFilterNode> node1 = std::dynamic_pointer_cast<CRepeatFilterNode>(node);
		if (!node1)
			return std::make_shared<SuperNodeStruct>();
		return node1->getParameter();
	}
		break;
	case NODE_O_TYPE:
		break;
	case NODE_O_COLORBAR_TYPE:
		break;
	case NODE_O_CUTTINGPLANE_TYPE:
		break;
	default:
		break;
	}
	return std::shared_ptr<SuperNodeStruct>();
}

int XmlProject::initProjectFromXml()
{
	if (m_ProjectXml.empty())
		return RETURN_ERROR;
	initNodeList(m_ProjectXml);
	m_isSaved = true;	// 0718 刚打开的项目
	initTaskList();
	return RETURN_NORMAL;
}
