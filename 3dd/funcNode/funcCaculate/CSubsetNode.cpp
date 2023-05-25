// CSubsetNode.cpp

#include "CSubsetNode.h"
#include "XmlProject.h"
#include "subset_property_widget.h"

CSubsetNode::CSubsetNode(std::shared_ptr< XmlProject> project) :CaculateNode(project)
{
	m_nodeType = NodeType::NODE_C_SUBSET_TYPE;
	m_nodeCanAppendNodeTypeList = {
		NodeType:: NODE_G_AXIS_TYPE,
		NodeType::NODE_G_BOUNDINGBOX_TYPE,
		NodeType::NODE_G_SLICEMAP_TYPE,
		NodeType::NODE_G_VOLUMERENDERING_TYPE,
		NodeType::NODE_G_SCATTERMAP_TYPE,
		NodeType::NODE_G_ORTHOGONALFACTOR_TYPE,
		NodeType::NODE_G_ISOLINE_TYPE,
		NodeType::NODE_G_ISOSURFACE_TYPE,
		NodeType::NODE_G_SURFACERENDER_TYPE,
	};
//	m_meshData = NULL;
}

CSubsetNode::~CSubsetNode()
{
}

bool CSubsetNode::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	//memcpy_s(&m_conversion, sizeof(C_Conversion), &conversion, sizeof(C_Conversion));
	std::shared_ptr<C_SubsetFilter> _st = std::dynamic_pointer_cast<C_SubsetFilter>(st);
	SurperNode::setParameter(st);
	m_filter = *_st;
	m_filter.setProjectId(m_project.lock()->getProjectId());
	//removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

bool CSubsetNode::getParameter(C_SubsetFilter& filter)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&conversion, sizeof(C_Conversion), &m_conversion, sizeof(C_Conversion));
	filter = m_filter;
	return true;
}

std::shared_ptr<SuperNodeStruct> CSubsetNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<C_SubsetFilter> ptr = std::make_shared<C_SubsetFilter>();
	*ptr = m_filter;
	SurperNode::getParameter(ptr);
	return ptr;
}

std::shared_ptr<TiXmlNode> CSubsetNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameter = std::make_shared<TiXmlElement>("Parameters");
	m_filter.toXml(pParameter);
	pElemNode->InsertEndChild(*pParameter);

	return pElemNode;
}

bool CSubsetNode::readParameterFromXmlNode(TiXmlNode* node)
{
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	TiXmlElement* pElement = (TiXmlElement*)(node);
	TiXmlElement* pParameter = (pElement->FirstChildElement("Parameters"));
	m_filter.fromXml(pParameter);
	return true;
}

#if 0
void CSubsetNode::setMeshData(std::vector<pointFinal>* meshData, int resolution[3]) 
{
	m_meshData = meshData;
	memcpy(m_oldResolution, resolution, sizeof(m_oldResolution));
}
#endif

void CSubsetNode::getDefault(SuperNodeStruct* st) {
	C_SubsetFilter* pst = (C_SubsetFilter*)st;
	pst->m_xStart = 0;
	pst->m_xEnd = 50;
	pst->m_xStep = 1;

	pst->m_yStart = 0;
	pst->m_yEnd = 50;
	pst->m_yStep = 1;

	pst->m_zStart = 0;
	pst->m_zEnd = 50;
	pst->m_zStep = 1;
}

bool CSubsetNode::run()
{
	std::vector<pointFinal>* m_meshData= &m_data;
	if (m_meshData == NULL || m_meshData->empty())
		return false;
	if (m_excuted) {
		makeNextNodeRun();
		return true;
	}
	if (widget) {
		subset_property_widget* pWidget = (subset_property_widget*)widget;
		//pWidget->data_count_to_widget(m_data.size(), 0);
		pWidget->showInputResolution(m_oldResolution);
	}

	if (m_oldResolution[0] < m_filter.m_xEnd)
		m_filter.m_xEnd = m_oldResolution[0];
	if (m_oldResolution[1] < m_filter.m_yEnd)
		m_filter.m_yEnd = m_oldResolution[1];
	if (m_oldResolution[2] < m_filter.m_xEnd)
		m_filter.m_zEnd = m_oldResolution[2];

	int nx = (m_filter.m_xEnd - m_filter.m_xStart) / m_filter.m_xStep;
	int ny = (m_filter.m_yEnd - m_filter.m_yStart) / m_filter.m_yStep;
	int nz = (m_filter.m_zEnd - m_filter.m_zStart) / m_filter.m_zStep;

	m_newResolution[0] = nx;
	m_newResolution[1] = ny;
	m_newResolution[2] = nz;

	m_filterData.clear();
	m_filterData.reserve(nx * ny * nz);

	for (int x = m_filter.m_xStart; x < m_filter.m_xEnd; x += m_filter.m_xStep)
	{
		for (int y = m_filter.m_yStart; y < m_filter.m_yEnd; y += m_filter.m_yStep) 
		{
			for (int z = m_filter.m_zStart; z < m_filter.m_zEnd; z += m_filter.m_zStep) 
			{
				int index = x * m_oldResolution[1]  * m_oldResolution[2]  +
					y * m_oldResolution[2]  + z;
				m_filterData.push_back(m_meshData->at(index));
			}
		}
	}


	m_dataRange[0] = m_dataRange[3] = m_filterData[0].x;
	m_dataRange[1] = m_dataRange[4] = m_filterData[0].y;
	m_dataRange[2] = m_dataRange[5] = m_filterData[0].z;
	
	for (auto var : m_filterData)
	{
		if (var.x > m_dataRange[3])
		{
			m_dataRange[3] = var.x;
		}
		if (var.x < m_dataRange[0])
		{
			m_dataRange[0] = var.x;
		}
		if (var.y > m_dataRange[4])
		{
			m_dataRange[4] = var.y;
		}
		if (var.y < m_dataRange[1])
		{
			m_dataRange[1] = var.y;
		}
		if (var.z > m_dataRange[5])
		{
			m_dataRange[5] = var.z;
		}
		if (var.z < m_dataRange[2])
		{
			m_dataRange[2] = var.z;
		}
	}

	m_limits[0] = *(m_dataRange); //网格化geometry属性互相影响 改变其中一个值会影响另外两个 优先度自定义
	m_limits[1] = -0.9387755102;
	m_limits[2] = *(m_dataRange + 2);//y值都是0  用了voxler的值 初始化逻辑自定义；
	m_limits[3] = *(m_dataRange + 3);
	m_limits[4] = 0.9387755102;
	m_limits[5] = *(m_dataRange + 5);

	makeNextNodeRun();

	m_excuted = true;

	return true;
}

bool CSubsetNode::getOnputData(std::vector<pointFinal>& data)
{
	if (m_filterData.empty())
		return false;
	data.resize(m_filterData.size());
	std::copy(m_filterData.begin(), m_filterData.end(), data.begin());
	return true;
}

void CSubsetNode::makeNextNodeRun1() {

	for (auto nId : m_nodeNextNodeList)
	{
		std::shared_ptr<GraphNode> node = std::dynamic_pointer_cast<GraphNode>(m_project.lock()->getSpecifiedNode(nId)); ;
		if (!node.get())
			continue;
		node->setInputData(m_filterData);
		node->setDataRange(getDataRange());
		node->setResolution(getResolution());
		node->setLimits(getLimits());
		node->run();
	}
}


