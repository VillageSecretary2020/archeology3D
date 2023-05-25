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

#include "XmlProject.h"

static std::string getLogicErrorFromErrorCode(LogicErrorType errorCode);
class OperationLogic : public std::enable_shared_from_this<OperationLogic>
{
public:
	virtual ~OperationLogic();
	static std::shared_ptr<OperationLogic> getOperationLogicInstance();
	static void updateLogicError();
	int8_t closeSystem();
	int8_t saveAndCloseSysterm();
	static void updateErrorNum(std::shared_ptr<void> pOp, bool addNum = true);
	std::list<std::shared_ptr<SuperNodeStruct>> getProjectNodeList(int8_t projectId);
	std::shared_ptr<XmlProject> getSpecifiedProject(int8_t projectId);
public:
	int8_t openProject(std::string xmlPath);
	bool closeProject(int8_t projectId);
	int8_t newProject(std::string xmlPath);
	int8_t saveProject(int8_t projectId, std::string xmlPath = "");
	int8_t saveAllProject();
	std::list<std::pair<std::string, std::string>> getProjectListError();
	stringlist getProjectError(int8_t projectId);

	//node
	uint16_t addNode(std::shared_ptr<SuperNodeStruct> parameter);
private:
	OperationLogic();
	int getErrorNum();
	int8_t getAddProjectId();
	int8_t checkProjectSaved();
	void addErrorNum(bool addNum);
	std::shared_ptr<SurperNode> createNode(NodeType type, std::shared_ptr<SuperNodeStruct> parameter);
private:
	std::map<int8_t, std::shared_ptr<XmlProject>> m_ProjectMap;
	int m_projectNum;
	static LogicError m_error;
	std::atomic_int m_errorNum;
};