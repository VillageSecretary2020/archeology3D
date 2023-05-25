// OperationLogic.cpp

#include "OperationLogic.h"
#include "NodeHeader.h"

std::string getLogicErrorFromErrorCode(LogicErrorType errorCode)
{
	switch (errorCode)
	{
	case LogicErrorType::ERROR_INIT_PROJECT_FROMXML:
	{
		OperationLogic::updateLogicError();
		return std::string("xml file of project are unavailable!");
	}
	default:
		return std::string("no error.");
		break;
	}
}

LogicError OperationLogic::m_error = RETURN_NORMAL;

uint16_t OperationLogic::addNode(std::shared_ptr<SuperNodeStruct> parameter)
{
	return m_ProjectMap[parameter->getProjectId()]->addNode(parameter);
}

OperationLogic::OperationLogic()
{
	m_ProjectMap.clear();
	m_projectNum = 0;
}

OperationLogic::~OperationLogic()
{
	//test
	int num = 0;
}

std::shared_ptr<OperationLogic> OperationLogic::getOperationLogicInstance()
{
	static std::shared_ptr<OperationLogic> pLogic(new OperationLogic());
	return pLogic;
}

void OperationLogic::updateLogicError()
{
	m_error = RETURN_NORMAL;
}

int8_t OperationLogic::closeSystem()
{
	int ret = checkProjectSaved();
	if (RETURN_NORMAL != ret)
	{
		return ret;
	}
	return RETURN_NORMAL;
}

int8_t OperationLogic::saveAndCloseSysterm()
{
	std::map<int8_t, std::shared_ptr<XmlProject>>::iterator it = m_ProjectMap.begin();
	for (; it != m_ProjectMap.end(); )
	{
		if (RETURN_ERROR == it->second->saveProject())
			return it->first;
		it = m_ProjectMap.erase(it);
	}
	return RETURN_NORMAL;
}

void OperationLogic::updateErrorNum(std::shared_ptr<void> pOp, bool addNum)
{
	OperationLogic::getOperationLogicInstance()->addErrorNum(addNum);
}

std::list<std::shared_ptr<SuperNodeStruct>> OperationLogic::getProjectNodeList(int8_t projectId)
{
	auto itr = m_ProjectMap.find(projectId);
	if(itr == m_ProjectMap.end())
		return std::list<std::shared_ptr<SuperNodeStruct>>();

	return itr->second->getNodeListInfo();
}

std::shared_ptr<XmlProject> OperationLogic::getSpecifiedProject(int8_t projectId)
{
	auto itr = m_ProjectMap.find(projectId);
	if (m_ProjectMap.end() == itr)
		return std::shared_ptr<XmlProject>();
	return itr->second;
}

int8_t OperationLogic::openProject(std::string xmlPath)
{
	std::shared_ptr<XmlProject> project(std::make_shared<XmlProject>(xmlPath, getAddProjectId(), &OperationLogic::updateErrorNum));
	project->initProjectFromXml();
	if (!project->getProjectErrorList().empty())
	{
		m_error = LogicErrorType::ERROR_INIT_PROJECT_FROMXML;
		return RETURN_ERROR;
	}
	int id = project->getProjectId();
	m_ProjectMap.insert(std::pair<int8_t, std::shared_ptr<XmlProject>>(project->getProjectId(), project));
	return project->getProjectId();
}

bool OperationLogic::closeProject(int8_t projectId)
{
	if (RETURN_NORMAL == m_ProjectMap[projectId]->saveProject())
	{
		m_ProjectMap.erase(projectId);
		int size = m_ProjectMap.size();
		return true;
	}

	return false;
}

int8_t OperationLogic::newProject(std::string xmlPath)
{
	int8_t id = getAddProjectId();	// yzw 0525 优化
	std::shared_ptr<XmlProject> project(std::make_shared<XmlProject>( XmlProject(xmlPath, id, &OperationLogic::updateErrorNum)));
	
	int size = m_ProjectMap.size();
	m_ProjectMap.insert(std::pair<int8_t, std::shared_ptr<XmlProject>>(id, project));
	return id;
}

int8_t OperationLogic::saveProject(int8_t projectId, std::string xmlPath)
{
	auto itr = m_ProjectMap.find(projectId);
	if (m_ProjectMap.end() == itr)
		return RETURN_ERROR;
	if (itr->second->saveProject(xmlPath) == RETURN_NORMAL)
		return itr->first;
	return RETURN_ERROR;
}
int8_t OperationLogic::saveAllProject()
{
	for (auto var : m_ProjectMap)
	{
		if (RETURN_NORMAL != var.second->saveProject())
		{
			return var.first;
		}
	}
	return RETURN_NORMAL;
}
int OperationLogic::getErrorNum()
{
	int num = m_errorNum;
	m_errorNum = RETURN_NORMAL;
	return num;
}

void OperationLogic::addErrorNum(bool addNum)
{
	if (addNum)
		m_errorNum++;
	else
		m_errorNum--;
}

std::shared_ptr<SurperNode> OperationLogic::createNode(NodeType type, std::shared_ptr<SuperNodeStruct> parameter)
{
	switch (type)
	{
	case NODE_NODEFINE_TYPE:
	{
		
	}
		break;
	case NODE_D_DATASOURCE_TYPE:
		break;
	case NODE_G_TYPE:
		break;
	case NODE_G_SLICEMAP_TYPE:
		break;
	case NODE_G_VOLUMERENDERING_TYPE:
		break;
	case NODE_G_SCATTERMAP_TYPE:
	{
		std::shared_ptr<GScatterMapNode> node(std::make_shared<GScatterMapNode>(m_ProjectMap[parameter->getProjectId()]));
		std::shared_ptr<G_ScatterMap> smap = std::dynamic_pointer_cast<G_ScatterMap>(parameter);
		if (node->setParameter(smap))
		{
			return node;
		}
		return node;
	}
		break;
	case NODE_G_TEXT_TYPE:
		break;
	case NODE_G_ORTHOGONALFACTOR_TYPE:
		break;
	case NODE_G_COMMENT_TYPE:
		break;
	case NODE_G_ISOLINE_TYPE:
		break;
	case NODE_G_ISOSURFACE_TYPE:
		break;
	case NODE_G_SURFACERENDER_TYPE:
		break;
	case NODE_G_AXIS_TYPE:
		break;
	case NODE_G_BOUNDINGBOX_TYPE:
		break;
	case NODE_G_HIGHANDLOWDIAGRAM_TYPE:
		break;
	//case NODE_G_CLIPBOARD_TYPE:
	//	break;
	case NODE_C_TYPE:
		break;
	case NODE_C_CONVERSION_TYPE:
		break;
	case NODE_C_MERGE_TYPE:
		break;
	case NODE_C_SUBSET_TYPE:
		break;
	case NODE_C_ELIMINATIONFILTER_TYPE:
		break;
	case NODE_C_MESH_TYPE:
		break;
	case NODE_C_FILTER:
		break;
	case NODE_C_REPEATFILER:
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
	return std::shared_ptr<SurperNode>();
}

std::list<std::pair<std::string, std::string>> OperationLogic::getProjectListError()
{
	int errorNum = getErrorNum();
	std::list<std::pair<std::string, std::string>> errorStrList;
	for (auto var : m_ProjectMap)
	{
		if (errorNum <= RETURN_ERROR)
			break;
		if (!var.second->getProjectErrorList().empty())
			errorStrList.push_back(std::make_pair(var.second->getProjectName(), (":has error happened")));
		errorNum--;
	}
	return errorStrList;
}

stringlist OperationLogic::getProjectError(int8_t projectId)
{
	return m_ProjectMap[projectId]->getProjectErrorList();
}

int8_t OperationLogic::getAddProjectId()
{
	return ++m_projectNum;
}

int8_t OperationLogic::checkProjectSaved()
{
	for (auto var : m_ProjectMap)
	{
		if (!var.second->checkProjectSaved())
			return var.first;
	}
	return RETURN_NORMAL;
}
