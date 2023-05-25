// CMeshNode.cpp

#include "base_property_widget.h"
#include "CMeshNode.h"
#include "XmlProject.h"
#include "gridding_property_widget.h"
#include "Interpolation.h"

CMeshNode::CMeshNode(std::shared_ptr< XmlProject> project) :CaculateNode(project)
{
	m_nodeType = NodeType::NODE_C_MESH_TYPE;
	m_nodeCanAppendNodeTypeList = {
	};
}

CMeshNode::~CMeshNode()
{
}

bool CMeshNode::setParameter(std::shared_ptr<SuperNodeStruct> data)
{
	std::shared_ptr<C_Mesh> _data = std::dynamic_pointer_cast<C_Mesh>(data);
	if (!_data || !m_project.lock())	// yzw 0704 RETURN_ERROR == m_nodeIndex ||
		return false;
	//memcpy_s(&m_mesh, sizeof(C_Mesh), mesh.get(), sizeof(C_Mesh));
	SurperNode::setParameter(data);
	m_mesh = *_data;
	m_mesh.setProjectId(m_project.lock()->getProjectId());
	m_excuted = false;	// 设置已修改，需要重新run
	return true;
}

std::shared_ptr<SuperNodeStruct> CMeshNode::getParameter()
{
	if (RETURN_ERROR == m_nodeIndex || !m_project.lock())
		return nullptr;
	
	std::shared_ptr<C_Mesh> data = std::make_shared<C_Mesh>();
	//memcpy_s(mesh.get(), sizeof(C_Mesh), &m_mesh, sizeof(C_Mesh));
	*data = m_mesh;
	SurperNode::getParameter(data);
	return data;
}

std::shared_ptr<TiXmlNode> CMeshNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	m_mesh.toXml(pParameters);

	pElemNode->InsertEndChild(*pParameters);

	return pElemNode;
}

bool CMeshNode::readParameterFromXmlNode(TiXmlNode* node)
{

#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
		return false;
	TiXmlElement* pParaters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if (!pParaters)
		return SetProjectError
		m_mesh.fromXml(pParaters);

	return true;
}

bool CMeshNode::getOnputData(std::vector<pointFinal>& data)
{
	if (m_caculateData.empty())
		return false;
	data.resize(m_caculateData.size());
	std::copy(m_caculateData.begin(), m_caculateData.end(), data.begin());
	return true;
}

void CMeshNode::getDefault(SuperNodeStruct* st)		// 取默认参数
{
	C_Mesh* pst = (C_Mesh*)st;	//	默认参数
	pst->m_putInPointNum = -1;
	Search_TYpe search_type;
	//search_type.m_simple.Radius = sqrt(findr);
	search_type.m_simple.MinCount = 1;
	search_type.m_simple.MaxCount = 50;
	//search_type.m_anisotropic.SearchEllipse_x = sqrt(findr) - 5;
	//search_type.m_anisotropic.SearchEllipse_y = sqrt(findr);
	//search_type.m_anisotropic.SearchEllipse_z = sqrt(findr) + 5;
	search_type.m_anisotropic.MinCount = 1;
	search_type.m_anisotropic.MinCount = 50;

	Child_Method_Type method_type;
	method_type.m_idw.order = 2;
	method_type.m_idw.smooth = 0;
	method_type.m_kriging.mode = 2;
	method_type.m_kriging.c0 = 1.0;
	method_type.m_kriging.c1 = 1.0;
	method_type.m_kriging.a = 1.0;

	C_Mesh m_mesh;
	m_mesh.m_putInPointNum = -1;
	m_mesh.m_SearchType = SearchType::SEARCH_SIMPLE;
	m_mesh.m_search_type = search_type;
	m_mesh.m_MethosType = Child_insert_Method_Mesh::INVERSEDISTANCE;
	m_mesh.m_method_type = method_type;
#if  1
	m_mesh.m_geometry.limits[0] = 0; 
	m_mesh.m_geometry.limits[1] = 0;
	m_mesh.m_geometry.limits[2] = 0;
	m_mesh.m_geometry.limits[3] = 1;
	m_mesh.m_geometry.limits[4] = 1;
	m_mesh.m_geometry.limits[5] = 1;
	m_mesh.m_geometry.resolution[0] = 30;
	m_mesh.m_geometry.resolution[1] = 30;
	m_mesh.m_geometry.resolution[2] = 30;
	m_mesh.m_geometry.space[0] = 0.02;
	m_mesh.m_geometry.space[1] = 0.02;
	m_mesh.m_geometry.space[2] = 0.02;
#endif
	*pst = m_mesh;
}

bool CMeshNode::run()
{
	std::shared_ptr<SurperNode> node = m_project.lock()->getSpecifiedNode(*(m_nodePreNodelist.begin()));
	if (NODE_D_DATASOURCE_TYPE == node->getNodeType()) {
		std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
		bool ok = node1->run();
		std::vector<pointOriginal> _data;
		bool ok1 = node1->getOnputData(_data);
		setInputData(_data, m_mesh.m_dataIndex % (node1->get_fields_num()));
	}
	if (NODE_C_TYPE == node->getBelongType()){
		std::shared_ptr<CaculateNode> subsetNode = std::dynamic_pointer_cast<CaculateNode>(node);
		std::vector<pointFinal> _data;
		bool ok1 = subsetNode->getOnputData(_data);
		setInputData(_data);
	}
	if (m_data.empty())
		return false;
	if (m_excuted){	// 运行过，未改变设置
		makeNextNodeRun();
		return true;
	}
	if (widget) {
		base_property_widget* pWidget = (base_property_widget*)widget;
		pWidget->data_count_to_widget(m_data.size(), 0);
	}
	//double m_dataRange[6];
	m_dataRange[0] = m_dataRange[3] = m_data[0].x;
	m_dataRange[1] = m_dataRange[4] = m_data[0].y;
	m_dataRange[2] = m_dataRange[5] = m_data[0].z;
	std::vector<std::vector<double>> _points;

	for (auto var : m_data)
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
		std::vector<double> _point;
		_point.push_back(var.x);
		_point.push_back(var.y);
		_point.push_back(var.z);
		_point.push_back(var.scalar);
		_points.push_back(_point);
	}
	double xdif = m_dataRange[3] - m_dataRange[0];
	double ydif = m_dataRange[4] - m_dataRange[1];
	double zdif = m_dataRange[5] - m_dataRange[2];
	double findr = xdif * xdif + ydif * ydif + zdif * zdif;
	findr = sqrt(findr);
	////
	m_mesh.m_search_type.m_simple.Radius = findr;
	m_mesh.m_search_type.m_anisotropic.SearchEllipse_x = (findr) - 5;
	m_mesh.m_search_type.m_anisotropic.SearchEllipse_y = (findr);
	m_mesh.m_search_type.m_anisotropic.SearchEllipse_z = (findr) + 5;

	//C_Mesh mesh;
	m_mesh.m_geometry.limits[0] = m_dataRange[0]; //网格化geometry属性互相影响 改变其中一个值会影响另外两个 优先度自定义
	m_mesh.m_geometry.limits[1] = m_dataRange[1]; //
	m_mesh.m_geometry.limits[2] = m_dataRange[2]; // *(m_dataRange + 2);//y值都是0  用了voxler的值 初始化逻辑自定义；
	m_mesh.m_geometry.limits[3] = m_dataRange[3]; // *(m_dataRange + 3);
	m_mesh.m_geometry.limits[4] = m_dataRange[4]; // 0.9387755102;
	m_mesh.m_geometry.limits[5] = m_dataRange[5]; // *(m_dataRange + 5);
	if (xdif < 2.0)
	{
		m_mesh.m_geometry.limits[0] = m_dataRange[0] -0.9387755102;
		m_mesh.m_geometry.limits[3] = m_dataRange[3] +0.9387755102; 
	}
	if (ydif < 2.0)
	{
		m_mesh.m_geometry.limits[1] = m_dataRange[1] -0.9387755102;
		m_mesh.m_geometry.limits[4] = m_dataRange[4] +0.9387755102; 
	}
	if (zdif < 2.0)
	{
		m_mesh.m_geometry.limits[2] = m_dataRange[2] -0.9387755102;
		m_mesh.m_geometry.limits[5] = m_dataRange[5] +0.9387755102; 
	}
	m_mesh.m_geometry.space[0] = (m_mesh.m_geometry.limits[3] - m_mesh.m_geometry.limits[0]) / (m_mesh.m_geometry.resolution[0]-1);
	m_mesh.m_geometry.space[1] = (m_mesh.m_geometry.limits[4] - m_mesh.m_geometry.limits[1]) / (m_mesh.m_geometry.resolution[1]-1);
	m_mesh.m_geometry.space[2] = (m_mesh.m_geometry.limits[5] - m_mesh.m_geometry.limits[2]) / (m_mesh.m_geometry.resolution[2]-1);
	////
	bool ret= false;
	KDTree tree(_points);

	double* result = new double[(m_mesh.m_geometry.resolution[0] ) * (m_mesh.m_geometry.resolution[1] ) * (m_mesh.m_geometry.resolution[2] )];

	Interpolation ipn;
	ipn.set_callback(std::bind(&CMeshNode::set_progress, this, std::placeholders::_1, std::placeholders::_2, placeholders::_3));
	//
	ipn.setKDtree(&tree);
	m_caculateData.clear();
	m_caculateData.reserve((m_mesh.m_geometry.resolution[0] ) * (m_mesh.m_geometry.resolution[1] ) * (m_mesh.m_geometry.resolution[2] ));
	if (m_mesh.m_SearchType == SearchType::SEARCH_AllDATA)
	{
		ipn.setSearchParameter();
	}
	if (m_mesh.m_SearchType == SearchType::SEARCH_SIMPLE)
	{
		ipn.setSearchParameter(m_mesh.m_search_type.m_simple.Radius, m_mesh.m_search_type.m_simple.MinCount, m_mesh.m_search_type.m_simple.MaxCount);
	}
	if (m_mesh.m_SearchType == SearchType::SEARCH_ANISOTROPIC)
	{
		std::vector<double> Radius;
		Radius.push_back(m_mesh.m_search_type.m_anisotropic.SearchEllipse_x);
		Radius.push_back(m_mesh.m_search_type.m_anisotropic.SearchEllipse_y);
		Radius.push_back(m_mesh.m_search_type.m_anisotropic.SearchEllipse_z);
		ipn.setSearchParameter(Radius, m_mesh.m_search_type.m_anisotropic.MinCount, m_mesh.m_search_type.m_anisotropic.MinCount);
	}

	if (m_mesh.m_MethosType == Child_insert_Method_Mesh::INVERSEDISTANCE)
	{
		ret= ipn.idw(m_mesh.m_geometry.limits, m_mesh.m_method_type.m_idw.order, m_mesh.m_method_type.m_idw.smooth, m_mesh.m_geometry.resolution, result, m_caculateData);
	}
	else //if (m_mesh.m_MethosType == Child_insert_Method_Mesh::KRIGING)
	{
		ret= ipn.kriging(m_mesh.m_geometry.limits, m_mesh.m_method_type.m_kriging.mode, m_mesh.m_geometry.resolution, m_mesh.m_method_type.m_kriging.c0, m_mesh.m_method_type.m_kriging.c1, m_mesh.m_method_type.m_kriging.a, result, m_caculateData);
	}
	//else
	//{
	//	return false;
	//}
	delete result;
	if(ret){	// 已完成
		for (auto var : m_caculateData)
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
		m_excuted = true;
		//makeNextNodeRun();//移至gridding_property_widget::end_progress()内nextNodeRun，在主线程内运行
	}
	return ret;
}

bool CMeshNode::set_progress(int r, int p, const char* m)
{
	bool stop= false;
	if(widget){
		gridding_property_widget*pw = (gridding_property_widget*)widget;
		stop= !pw->set_progress(r, p, m);
	}
	return stop;
}
