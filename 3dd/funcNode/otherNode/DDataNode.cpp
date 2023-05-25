// DDataNode.cpp

#include "base_property_widget.h"
#include "source_property_widget.h"
#include "DDataNode.h"
#include "XmlProject.h"
#include "QThread.h"
#include <QCoreApplication>
#include <QElapsedTimer>


DDataNode::DDataNode(std::shared_ptr<XmlProject> project) : SurperNode(project)
{
	m_nodeType = NodeType::NODE_D_DATASOURCE_TYPE;
	m_nodeBelongType = NodeType::NODE_NODEFINE_TYPE;

	m_nodeCanAppendNodeTypeList = { NodeType::NODE_G_SCATTERMAP_TYPE, NodeType::NODE_G_AXIS_TYPE,
	NodeType::NODE_G_BOUNDINGBOX_TYPE, NodeType::NODE_C_MESH_TYPE, NodeType::NODE_C_REPEATFILER,
	NodeType::NODE_C_ELIMINATIONFILTER_TYPE, NodeType::NODE_C_CONVERSION_TYPE};
}

DDataNode::~DDataNode()
{
	QElapsedTimer t;
	while (0!= fileReading){
		fileReading = -5;
		t.start();
		while (t.elapsed() < 500);
				QCoreApplication::processEvents();
	}
	if (m_parseThread)
		m_parseThread->join();
}

bool DDataNode::setReadFilepath(std::string readInFile)
{
	char ext[10];
	_splitpath(readInFile.data(), nullptr, nullptr, nullptr, ext);
	auto compareStr = [&](std::string str1, std::string str2) {
		if (str1.length() != str2.length())
			return false;
		return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
			return tolower(a) == tolower(b); });
	};
	if (compareStr(ext, ".dat"))
	{
		m_dataInfo.m_filetype = fileType::DAT;
	}
	else if(compareStr(ext, ".dem"))
	{
		m_dataInfo.m_filetype = fileType::DEM;
	}
	else

	{
		return false;
	}
	m_readInFileName = readInFile;
	return true;
}

bool DDataNode::setParameter(std::shared_ptr<SuperNodeStruct> dataInfo)
{
	std::shared_ptr<D_DataInfo> _info = std::dynamic_pointer_cast<D_DataInfo>(dataInfo);
	if (_info->m_filePath.empty())
		return false;
	SurperNode::setParameter(dataInfo);
	// memcpy(&m_dataInfo, dataInfo.get(), sizeof(dataInfo));
	m_dataInfo = *_info;	// yzw 0702
	run();
	return true;
}

std::shared_ptr<SuperNodeStruct> DDataNode::getParameter()
{
	if (!m_project.lock() || RETURN_ERROR == m_nodeIndex)
		return std::shared_ptr<SuperNodeStruct>();
	std::shared_ptr<D_DataInfo> dSource(std::make_shared<D_DataInfo>());
	//memcpy_s(dSource.get(), sizeof(D_DataInfo), &m_dataInfo, sizeof(D_DataInfo));
	*(dSource.get()) = m_dataInfo;
	SurperNode::getParameter(dSource);
	return dSource;
}

// yzw 0701
std::shared_ptr<TiXmlNode> DDataNode::CreateXmlNode()
{
	std::shared_ptr<TiXmlNode> pNode = SurperNode::CreateXmlNode();
	std::shared_ptr<TiXmlElement> pElemNode = std::dynamic_pointer_cast<TiXmlElement>(pNode);
	std::shared_ptr<TiXmlElement> pParameters = std::make_shared<TiXmlElement>("Parameters");
	std::shared_ptr<TiXmlElement> pDataInfo = std::make_shared<TiXmlElement>("DataInfo");
	m_dataInfo.toXml(pDataInfo);
	pParameters->InsertEndChild(*pDataInfo);
	pElemNode->InsertEndChild(*pParameters);
	return pNode;
}

bool DDataNode::readParameterFromXmlNode(TiXmlNode* node)
{
#define SetProjectError m_project.lock()->setProjectError(ProjectErrorType::ERROR_XML_PARSE_NODE);
	if (!node)
	{
		SetProjectError;
		return false;
	}
	if (!SurperNode::readParameterFromXmlNode(node))
	{
		SetProjectError;
		return false;
	}
	// yzw 0702
	TiXmlElement* pParameters = ((TiXmlElement*)node)->FirstChildElement("Parameters");
	if(pParameters){
		TiXmlElement* pDataInfo = pParameters->FirstChildElement("DataInfo");
		if(pDataInfo){
			m_dataInfo.fromXml(pDataInfo);
			run();
		}
	}return true;
}

bool DDataNode::getOnputData(std::vector<pointOriginal>& data)
{
	if (m_data.empty())
		return false;
	data.resize(m_data.size());
	std::copy(m_data.begin(), m_data.end(), data.begin());
	return true;
}

bool DDataNode::getOnputData(std::vector<pointFinal> &data)
{
	if (m_data.empty())
		return false;
	data.clear();
	for (auto item : m_data) {
		pointFinal pf(item.x, item.y, item.z, item.scalar[0]);
		data.push_back(pf);
	}
	return true;
}

int DDataNode::get_fields_num()
{
	return m_dataInfo.fields_num;
}

void DDataNode::setInputData(std::vector<pointFinal>& data)
{
}

void DDataNode::setInputData(std::vector<pointOriginal>& data, int dataIndex)
{
}

bool DDataNode::run()
{
	if (m_excuted)
	{
			if(widget){
				base_property_widget* pWidget = (base_property_widget*)widget;
				pWidget->data_count_to_widget(m_dataInfo.data_count, m_dataInfo.fields_num);
			}
		return true;
	}
	bool ret = false;
	m_dataInfo.fields_num = 0;
	m_readInFileName = m_dataInfo.m_filePath;
	char ext[10];
	char filename[100];
	_splitpath(m_readInFileName.data(), nullptr, nullptr, filename, ext);
	//m_dataInfo.m_filePath = filename;
	//m_dataInfo.m_filePath += ext;
	//test
	m_dataInfo.m_filetype = fileType::DAT;
	if (strstr(ext, "dem"))
		m_dataInfo.m_filetype = fileType::DEM;
	m_data.clear();
	switch (m_dataInfo.m_filetype)
	{
	case fileType::DAT:
		fileReading = 1;
		if(!threadMode){
			ret = readDatFile();
			if (ret) {
				m_dataInfo.data_count = m_data.size();
			}
			return ret;
		}
		else{
			OperationalLayer::getOperationInstance().addFuncTask(m_project.lock()->getProjectId(), std::bind(&DDataNode::readDatFile, this));
		}
		break;
	default:
		break;
	}
	return true;
}

bool DDataNode::readDatFile()
{
	std::string space = ", \t";		// 可能的分隔符是 逗号","，空格" "，制表符"\t"
	std::ifstream ifs(m_dataInfo.m_filePath);	// yzw 0702// m_readInFileName);
	if (!ifs.is_open())
		return readFinished(false);

	std::vector<std::string> nameVector;
	std::string tempStr;
	std::getline(ifs, tempStr);
	int num = 0;
	auto checkName = [&]() {
		std::istringstream iss(tempStr);
		num = stringSplit(tempStr, space).size();
		if (3 > num)
			return false;
		std::vector<std::string> tempVec;
		for (int i = 0; i < num; i++)
		{
			std::string str1;
			iss >> str1;
			tempVec.push_back(str1);
		}
		auto isNum = [&]() {
			for (auto var : tempVec)
			{
				std::stringstream sin(var);
				double d;
				char c;
				if (!(sin >> d))
					return false;
				if (sin >> c)
					return false;
			}
			return true;
		};
		if (!isNum())
		{
			nameVector = tempVec;
			return true;
		}
		else
		{
			pointOriginal point;
			point.x = atof(tempVec[0].c_str());
			point.y = atof(tempVec[1].c_str());
			point.z = atof(tempVec[2].c_str());
			for (int i = 3; i < tempVec.size(); i++)
			{
				point.scalar.push_back(atof(tempVec[i].c_str()));
			}
			m_data.push_back(point);
			return true;
		}
	};
		
	while (1)
	{
		bool ret = checkName();
		if (!ret)
		{
			std::getline(ifs, tempStr);
		}
		else
			break;
	}
	bool xe, ye, ze;	// xyz各轴数据是否存在
	xe = xyz_exist(nameVector, "x") || xyz_exist(nameVector, "X");
	ye = xyz_exist(nameVector, "y") || xyz_exist(nameVector, "Y");
	ze = xyz_exist(nameVector, "z") || xyz_exist(nameVector, "Z");

	int idn = 0, iline = 0;
	while (std::getline(ifs, tempStr) && 0< fileReading)
	{
		stringlist readTemp =stringSplit(tempStr, space);
		std::string str;
		//if (readTemp.size() == 0)
		//	break;
		if (readTemp.size() != num)
			continue;// return false;
		pointOriginal point;
		idn = 0; iline++;
		if(1 == iline%999)
		set_progress(iline, 0, "");
		point.x = 0;	point.y = 0;	point.z = 0;	
		if(xe){
			str = *readTemp.begin(); 
			readTemp.pop_front();
			point.x = atof(str.c_str()) ;
			idn++;
		}
		if (ye) {
			str = *readTemp.begin(); 
			readTemp.pop_front();
			point.y = atof(str.c_str());
			idn++;
		}
		if (ze) {
			str = *readTemp.begin(); 
			readTemp.pop_front();
			point.z = atof(str.c_str());
			idn++;
		}
		for (int i = idn; i < num; i++)
		{
			str = *readTemp.begin(); readTemp.pop_front();
			point.scalar.push_back(atof(str.c_str()));
		}
		point.scalarNameVec = nameVector;
		m_data.push_back(point);
	}
	m_dataInfo.fields_num = num - idn;	// 标量数据域个数
	set_progress(iline, 0, "");
	bool reading = 0 < fileReading;
	fileReading = 0;
	return readFinished(reading);
}

bool DDataNode::readFinished(bool ret)
{
	if (widget && ret) {
		base_property_widget* pWidget = (base_property_widget*)widget;
		pWidget->close_progress();
	}
	m_excuted = ret;
	m_dataInfo.data_count = ret ? m_data.size() : 0;
	return ret;
}

#include <setjmp.h>
#include <signal.h>
#ifndef sigsetjmp
#define sigsetjmp(p1, p2) setjmp(p1)
#define siglongjmp longjmp
typedef void(*sighandler_t)(int);
#define _crt_signal_t sighandler_t 
#endif
void DDataNode::makeNextNodeRun()
{
	int nodeId = 0;
	static jmp_buf env_mesh;		// yzw 0622 避免内存访问违例时程序崩溃
	_crt_signal_t hpre = signal(SIGSEGV, [](int sig) {	siglongjmp(env_mesh, 1); });
	int r = sigsetjmp(env_mesh, 1);
	if (r)	// 从longjmp跳回来了，返回值为1
	{
		//std::cout << __FUNCTION__ << ":L" << __LINE__ << " There's a problem in the node id= "<< nodeId << " ." <<std::endl;
	}
	else // 开始运行可能内存访问违例的部分
		do {
			// int* pi = (int*)nullptr;
			// test  *pi = 555;
			for (auto nId : m_nodeNextNodeList)
			{
				if (fileReading < 0)
					break;
				nodeId = nId;
				nextNodeRun(nId);
			}	// end for
			fileReading = 0;
		} while (0);
		hpre = signal(SIGSEGV, hpre);
}

bool DDataNode::nextNodeRun(int8_t nodeId)
{
	if (!m_excuted)
		return false;
	std::shared_ptr<SurperNode> node = m_project.lock()->getSpecifiedNode(nodeId);
	NodeType t = node->getNodeType();
	if (NODE_G_BOUNDINGBOX_TYPE == t ||
		NODE_G_HIGHANDLOWDIAGRAM_TYPE ==t ||
		NODE_G_PICTURE_TYPE ==t ||
		NODE_G_SCATTERMAP_TYPE ==t ||
		NODE_C_REPEATFILER== t ||
		NODE_C_CONVERSION_TYPE == t ||
		NODE_C_ELIMINATIONFILTER_TYPE == t) {
		node->setInputData(m_data, 0);
		node->run();
	}
	return true;
}

bool DDataNode::set_progress(int r, int p, const char* m)
{
	source_property_widget* pWidget = (source_property_widget*)widget;
		if(pWidget){
			pWidget->set_progress( r,  p,  m); 
		}
		return true;
}
