/*----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。 
//
// 文件功能描述：
//
// 
// 创建者：tangao 
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once
#include "SurperNode.h"
#include <sstream>
#include <thread>

class DDataNode : public SurperNode
{
public:
	DDataNode(std::shared_ptr<XmlProject> project);
	~DDataNode();
public:
	bool setReadFilepath(std::string readInFile);
	bool setParameter(std::shared_ptr<SuperNodeStruct> dataInfo);
	std::shared_ptr<SuperNodeStruct> getParameter();
	std::shared_ptr<TiXmlNode> CreateXmlNode();
	virtual bool readParameterFromXmlNode(TiXmlNode* node);
	bool getOnputData(std::vector<pointOriginal> &data);
	bool getOnputData(std::vector<pointFinal> &data);
	int get_fields_num();
	//---------------------不做实现
	void setInputData(std::vector<pointFinal> &data);
	void setInputData(std::vector<pointOriginal> &data, int dataIndex = 0);
	//---------------------
	bool run();
	virtual void makeNextNodeRun();
	virtual bool nextNodeRun(int8_t nodeId);

	std::string m_readInFileName;
	int threadMode = 1;	// 默认使用线程读入数据，曲面切片不使用
private:
	bool readDatFile();
	bool readFinished(bool);
	bool set_progress(int range, int pos, const char*);
private:
	std::vector<pointOriginal> m_data;
	D_DataInfo m_dataInfo;
	std::thread *m_parseThread = nullptr;
	int fileReading = 0;
};
