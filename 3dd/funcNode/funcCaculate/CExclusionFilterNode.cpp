#include "CExclusionFilterNode.h"
#include "XmlProject.h"
#include "shunting-yard.h"
#include "exprtk/exprtk.hpp"

CExclusionFilterNode::CExclusionFilterNode(std::shared_ptr< XmlProject> project) :CaculateNode(project)
{
	m_nodeType = NodeType::NODE_C_ELIMINATIONFILTER_TYPE;
	m_nodeCanAppendNodeTypeList = {};

}

CExclusionFilterNode::~CExclusionFilterNode()
{
}

bool CExclusionFilterNode::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	//memcpy_s(&m_conversion, sizeof(C_Conversion), &conversion, sizeof(C_Conversion));
	std::shared_ptr<C_EliminationFiler> _st = std::dynamic_pointer_cast<C_EliminationFiler>(st);
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

bool CExclusionFilterNode::getParameter(C_EliminationFiler& filter)
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&conversion, sizeof(C_Conversion), &m_conversion, sizeof(C_Conversion));
	filter = m_filter;
	return true;
}

std::shared_ptr<SuperNodeStruct> CExclusionFilterNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<C_EliminationFiler> ptr = std::make_shared<C_EliminationFiler>();
	*ptr = m_filter;
	SurperNode::getParameter(ptr);
	return ptr;
}

std::shared_ptr<TiXmlNode> CExclusionFilterNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	//todo:

	return pElemNode;
}

bool CExclusionFilterNode::readParameterFromXmlNode(TiXmlNode* node)
{
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	return false;
}

void CExclusionFilterNode::getDefault(SuperNodeStruct* st)
{// 默认参数
	C_EliminationFiler* pst = (C_EliminationFiler*)st;
	pst->m_eliminationStr = " x > 30 and z > -10";
}

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>   expression_t;
typedef exprtk::parser<double>       parser_t;

bool CExclusionFilterNode::run()
{
	if (m_data.empty())
		return false;
	if (m_excuted) {
		makeNextNodeRun();
		return true;
	}

	m_filterData.clear();

	if (m_filter.m_eliminationStr.empty()) {
		for (int i = 0; i < m_data.size(); i++) {
			m_filterData.push_back(m_data[i]);
		}
	} else {
		const std::string expression_string = m_filter.m_eliminationStr;
		symbol_table_t symbol_table;
		pointFinal point;

		symbol_table.add_variable("x", point.x);
		symbol_table.add_variable("y", point.y);
		symbol_table.add_variable("z", point.z);
		symbol_table.add_variable("s", point.scalar);

		expression_t expression;
		expression.register_symbol_table(symbol_table);

		parser_t parser;
		parser.compile(expression_string, expression);

		for (int i = 0; i < m_data.size(); i++) {
			point = m_data[i];
			// like  x > 30 and z > -10
			double value = expression.value();
			if (value == 0 || isnan(value)) {
				m_filterData.push_back(point);
			}
		}
	}
	m_excuted = true;
	
	makeNextNodeRun();

	return true;
}

bool CExclusionFilterNode::getOnputData(std::vector<pointFinal>& data)
{
	if (m_filterData.empty())
		return false;
	data.resize(m_filterData.size());
	std::copy(m_filterData.begin(), m_filterData.end(), data.begin());
	return true;
}

bool CExclusionFilterNode::run1()
{
	std::shared_ptr<SurperNode> node = m_project.lock()->getSpecifiedNode(*(m_nodePreNodelist.begin()));
	if (NODE_D_DATASOURCE_TYPE == node->getNodeType()){
		std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
		bool ok = node1->run();
		std::vector<pointOriginal> _data;
		bool ok1 = node1->getOnputData(_data);
		setInputData(_data, 0);//m_mesh.m_dataIndex % (node1->get_fields_num())
	}
	if (m_data.empty())
		return false;
	if (m_excuted) {
		makeNextNodeRun();
		return true;
	}

	for (int i = 0; i < m_data.size(); i++) {
		cparse::TokenMap vars;
		const pointFinal& point = m_data[i];
		vars["x"] = point.x;
		vars["y"] = point.y;
		vars["z"] = point.z;
		vars["s"] = point.scalar;

		// like x <= 3 && y <= 3
		if (cparse::calculator::calculate(m_filter.m_eliminationStr.c_str()).asBool()) {
			m_filterData.push_back(point);
		}
	}
	
	return true;
}


