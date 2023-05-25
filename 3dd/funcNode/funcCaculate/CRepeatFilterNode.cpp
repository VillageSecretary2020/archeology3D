#include "CRepeatFilterNode.h"
#include "XmlProject.h"
#include "octtree.h"
#include <map>

CRepeatFilterNode::CRepeatFilterNode(std::shared_ptr< XmlProject> project) :CaculateNode(project)
{
	m_nodeType = NodeType::NODE_C_REPEATFILER;
	m_nodeCanAppendNodeTypeList = {};

}

CRepeatFilterNode::~CRepeatFilterNode()
{
}

bool CRepeatFilterNode::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	//memcpy_s(&m_conversion, sizeof(C_Conversion), &conversion, sizeof(C_Conversion));
	std::shared_ptr<C_RepeatFilter> _st = std::dynamic_pointer_cast<C_RepeatFilter>(st);
	SurperNode::setParameter(st);
	m_filter = *_st;
	m_filter.setProjectId(m_project.lock()->getProjectId());
	//removeFromRender();
	if (m_excuted) 
	{
		m_excuted = false;
		return run();
	}
	return true;
}

bool CRepeatFilterNode::getParameter(C_RepeatFilter& filter)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&conversion, sizeof(C_Conversion), &m_conversion, sizeof(C_Conversion));
	filter = m_filter;
	return true;
}

std::shared_ptr<SuperNodeStruct> CRepeatFilterNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<C_RepeatFilter> ptr = std::make_shared<C_RepeatFilter>();
	*ptr = m_filter;
	SurperNode::getParameter(ptr);
	return ptr;
}

std::shared_ptr<TiXmlNode> CRepeatFilterNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	//todo:

	return pElemNode;
}

bool CRepeatFilterNode::readParameterFromXmlNode(TiXmlNode* node)
{
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	return false;
}

void CRepeatFilterNode::getDefault(SuperNodeStruct* st)
{
	C_RepeatFilter* pst = (C_RepeatFilter*)st;
	pst->m_Xtolerance = 0.10;
	pst->m_Ytolerance = 0.10;
	pst->m_Ztolerance = 0.10;
}

bool CRepeatFilterNode::run()
{
	if (m_data.empty())
		return false;
	if (m_excuted) {
		makeNextNodeRun();
		return true;
	}

	std::set<int> skipIndices;
	
	unibn::Octree<pointFinal> octree;
	unibn::OctreeParams params;

	octree.initialize(m_data);

	float distance = (float)sqrt(m_filter.m_Xtolerance * m_filter.m_Xtolerance + m_filter.m_Ytolerance * m_filter.m_Ytolerance + m_filter.m_Ztolerance * m_filter.m_Ztolerance);
	for (int i = 0; i < m_data.size(); i++) {
		//Do not check duplicated points
		if (skipIndices.find(i) != skipIndices.end()) {
			continue;
		}

		std::vector<uint32_t> resultIndices;
		const pointFinal& point = m_data[i];
		octree.radiusNeighbors<unibn::L2Distance<pointFinal> >(point, distance, resultIndices);

		for (int j = 0; j < resultIndices.size(); j++) {
			if (resultIndices[j] == i) {
				continue;
			}

			if (skipIndices.find(resultIndices[j]) != skipIndices.end()) {
				continue;
			}

			if (isPointDuplicate(i, resultIndices[j])) {
				skipIndices.insert(resultIndices[j]);
			}
		}
	}

	m_filterData.clear();
	m_filterData.reserve(m_data.size() - skipIndices.size());
	for (int i = 0; i < m_data.size(); i++) {
		if (skipIndices.find(i) != skipIndices.end()) {
			continue;
		}
		m_filterData.push_back(m_data[i]);
	}
	m_excuted = true;
	makeNextNodeRun();
	return true;
}



bool CRepeatFilterNode::isPointDuplicate(int index1, int index2) {
	const pointFinal& point1 = m_data[index1];
	const pointFinal& point2 = m_data[index2];

	return (point1.x >= point2.x - m_filter.m_Xtolerance &&
		point1.x <= point2.x + m_filter.m_Xtolerance &&
		point1.y >= point2.y - m_filter.m_Ytolerance &&
		point1.y <= point2.y + m_filter.m_Ytolerance &&
		point1.z >= point2.z - m_filter.m_Ztolerance &&
		point1.z <= point2.z + m_filter.m_Ztolerance);
}

bool CRepeatFilterNode::getOnputData(std::vector<pointFinal>& data)
{
	if (m_filterData.empty())
		return false;
	data.resize(m_filterData.size());
	std::copy(m_filterData.begin(), m_filterData.end(), data.begin());
	return true;
}

