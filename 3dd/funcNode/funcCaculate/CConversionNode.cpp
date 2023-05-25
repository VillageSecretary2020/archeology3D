#include "CConversionNode.h"
#include "XmlProject.h"
#include "math.h"

static void matrix_multiply(const double m1[4][4], const double m2[4][4], double m_out[4][4]);
static void get_rotate_matrix(double x, double y, double z, double angle, double m_out[4][4]);

CConversionNode::CConversionNode(std::shared_ptr< XmlProject> project) :CaculateNode(project)
{
	m_nodeType = NodeType::NODE_C_CONVERSION_TYPE;
	m_nodeCanAppendNodeTypeList = {};

}

CConversionNode::~CConversionNode()
{
}

bool CConversionNode::setParameter(std::shared_ptr<SuperNodeStruct> st)
{
	//if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
	if (!m_project.lock())
		return false;
	//memcpy_s(&m_conversion, sizeof(C_Conversion), &conversion, sizeof(C_Conversion));
	std::shared_ptr<C_Conversion> _st = std::dynamic_pointer_cast<C_Conversion>(st);
	SurperNode::setParameter(st);
	m_conversion= *_st;
	m_conversion.setProjectId(m_project.lock()->getProjectId());
	//removeFromRender();
	if (m_excuted) {
		m_excuted = false;
		return run();
	}
	return true;
}

bool CConversionNode::getParameter(C_Conversion & conversion)
{
	if(RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return false;
	//memcpy_s(&conversion, sizeof(C_Conversion), &m_conversion, sizeof(C_Conversion));
	conversion= m_conversion;
	return true;
}

std::shared_ptr<SuperNodeStruct> CConversionNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;

	std::shared_ptr<C_Conversion> ptr = std::make_shared<C_Conversion>();
	*ptr = m_conversion;
	SurperNode::getParameter(ptr);
	return ptr;
}

std::shared_ptr<TiXmlNode> CConversionNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	//todo:

	return pElemNode;
}

bool CConversionNode::readParameterFromXmlNode(TiXmlNode* node)
{
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	return false;
}

void CConversionNode::getDefault(SuperNodeStruct* st) {
	C_Conversion* pst = (C_Conversion*)st;
	pst->m_pos = {0, 0, 0};
	pst->m_conversionAttribute.m_scaleFactor.m_xScaleFactor= 1.0;
	pst->m_conversionAttribute.m_scaleFactor.m_yScaleFactor= 1.0;
	pst->m_conversionAttribute.m_scaleFactor.m_zScaleFactor= 1.0;
	pst->m_conversionAttribute.m_rotate.m_xRotate = 1;
	pst->m_conversionAttribute.m_rotate.m_yRotate = 0;
	pst->m_conversionAttribute.m_rotate.m_zRotate = 0;
	pst->m_conversionAttribute.m_rotate.m_angle = 0;
	pst->m_conversionAttribute.m_exchange.m_xExchange = 0;
	pst->m_conversionAttribute.m_exchange.m_yExchange = 0;
	pst->m_conversionAttribute.m_exchange.m_zExchange = 0;
}

bool CConversionNode::run()
{ // ? m_origin
	if (m_data.empty())
		return false;
	if (m_excuted) {
		makeNextNodeRun();
		return true;
	}
	m_transferData.clear();
	m_transferData.reserve(m_data.size());
	double matrix[4][4] = { 0 };
	getTransformMatrix(matrix);

	for (int i = 0; i < m_data.size(); i++)
	{
		const pointFinal& var = m_data[i];
		pointFinal newPoint;
		newPoint.x = var.x * matrix[0][0] + var.y * matrix[1][0] + var.z * matrix[2][0] + 1 * matrix[3][0] + m_conversion.m_pos.m_x;
		newPoint.y = var.x * matrix[0][1] + var.y * matrix[1][1] + var.z * matrix[2][1] + 1 * matrix[3][1] + m_conversion.m_pos.m_y;
		newPoint.z = var.x * matrix[0][2] + var.y * matrix[1][2] + var.z * matrix[2][2] + 1 * matrix[3][2] + m_conversion.m_pos.m_z;
		newPoint.scalar = var.scalar;
		m_transferData.push_back(newPoint);
	}
	m_excuted = true;
	makeNextNodeRun();
	return true;
}

void CConversionNode::getTransformMatrix(double matrix[4][4]) {

	double scale_matrix[4][4] = { 0 };
	double rotate_matrix[4][4] = { 0 };
	double translation_matrix[4][4] = { 0 };
	double matrix_temp[4][4] = { 0 };

	scale_matrix[0][0] = m_conversion.m_conversionAttribute.m_scaleFactor.m_xScaleFactor;
	scale_matrix[1][1] = m_conversion.m_conversionAttribute.m_scaleFactor.m_yScaleFactor;
	scale_matrix[2][2] = m_conversion.m_conversionAttribute.m_scaleFactor.m_zScaleFactor;
	scale_matrix[3][3] = 1;

	translation_matrix[0][0] = 1;
	translation_matrix[1][1] = 1;
	translation_matrix[2][2] = 1;

	get_rotate_matrix(
		m_conversion.m_conversionAttribute.m_rotate.m_xRotate,
		m_conversion.m_conversionAttribute.m_rotate.m_yRotate,
		m_conversion.m_conversionAttribute.m_rotate.m_zRotate,
		m_conversion.m_conversionAttribute.m_rotate.m_angle,
		rotate_matrix);

	translation_matrix[3][0] = m_conversion.m_conversionAttribute.m_exchange.m_xExchange;
	translation_matrix[3][1] = m_conversion.m_conversionAttribute.m_exchange.m_yExchange;
	translation_matrix[3][2] = m_conversion.m_conversionAttribute.m_exchange.m_zExchange;
	translation_matrix[3][3] = 1;

	matrix_multiply(scale_matrix, rotate_matrix, matrix_temp);
	matrix_multiply(matrix_temp, translation_matrix, matrix);
}


void get_rotate_matrix(double x, double y, double z, double angle, double m_out[4][4]) {
	double sinV = sin(angle / 180.0);
	double cosV = cos(angle / 180.0);

	m_out[0][0] = cosV + x * x * (1 - cosV);
	m_out[0][1] = x * y * (1 - cosV) - z * sinV;
	m_out[0][2] = x * z * (1 - cosV) + y * sinV;
	m_out[1][0] = x * y * (1 - cosV) + z * sinV;;
	m_out[1][1] = cosV + y * y * (1 - cosV);;
	m_out[1][2] = y * z * (1 - cosV) - x * sinV;
	m_out[2][0] = x * z * (1 - cosV) - y * sinV;
	m_out[2][1] = y * z * (1 - cosV) + x * sinV;
	m_out[2][2] = cosV + z * z * (1 - cosV);
	m_out[3][3] = 1;
}

void matrix_multiply(const double m1[4][4], const double m2[4][4], double m_out[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				m_out[i][j] += (m1[i][k] * m2[k][j]);
			}
		}
	}
}

bool CConversionNode::getOnputData(std::vector<pointFinal>& data)
{
	if (m_transferData.empty())
		return false;
	data.resize(m_transferData.size());
	std::copy(m_transferData.begin(), m_transferData.end(), data.begin());
	return true;
}
