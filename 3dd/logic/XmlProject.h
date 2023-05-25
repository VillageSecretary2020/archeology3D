/* ----------------------------------------------------------------
// Copyright (C)  合肥鼎方
// 版权所有。
//
// 文件功能描述：
//
//
// 创建者：tangAo
//
// 修改人：
// 时间：
// 修改说明：
//
//----------------------------------------------------------------*/
#pragma once

#include "NodeHeader.h"
#include <map>
#include <thread>
#include <condition_variable>

class workRunThread;
typedef std::function<bool()> BOOL_FUNC;

class XmlProject : public std::enable_shared_from_this<XmlProject>
{
	typedef void (callBack)(std::shared_ptr<void>, bool addNum);

public:
	XmlProject(int8_t ProjectId, callBack* call);
	XmlProject(std::string xmlPath, int8_t ProjectId, callBack* call);
	XmlProject() = delete;
	virtual ~XmlProject();
	static std::shared_ptr<SuperNodeStruct> getNodeParameter(std::shared_ptr<SurperNode> node);
public:
	void setRender(vtkSmartPointer<vtkRenderer> render);
	void setRenderWinInteractor(vtkSmartPointer< vtkRenderWindowInteractor> winInteractor);
		
	vtkSmartPointer<vtkRenderer> getRender();
	vtkSmartPointer<vtkRenderWindowInteractor> getWinInteractor();
	int8_t getProjectId();
	int8_t addNode(std::shared_ptr<SuperNodeStruct> pNodeInfo);

	int removeNode(int8_t nodeId);
//	int addConnectRelation(int8_t firstId, int8_t secondId);
	int removeConnectRelation(int8_t firstId, int8_t secondId);
	bool checkProjectSaved();
	void setProjectSavedStatus(bool isSave = false);
	std::shared_ptr<SurperNode> createNodeFromElement(TiXmlElement* element);
	int initProjectFromXml();
	int8_t saveProject(std::string xmlPath = "");
	stringlist getProjectErrorList();
	bool setProjectError(ProjectError error);
	void setProjectName(std::string projectName);
	std::string getProjectName();
	bool run(std::shared_ptr<SurperNode> runNode);
	int8_t getNextAloneNodeId(int8_t nodeId);
	std::shared_ptr<SurperNode> getSpecifiedNode(int8_t nodeId);
	std::list<std::shared_ptr<SuperNodeStruct>> getNodeListInfo();
	std::multimap<int8_t, int8_t> getNodeListConnection();

	//project
	void run();

	void caculateMesh(int meshId);
	void addFuncTask(BOOL_FUNC);
private:
	void initTaskList();
	//void runTaskFunc();
private:
	int addNode(std::shared_ptr<SurperNode> pNode);
	void updateProjectError();
	void removeNodeAndConnectRelation(int8_t nodeId);
	int saveNodeList(std::string xmlPath = "");
	int initNodeList(std::string xmlPath = "");
	int initProjectInfo(TiXmlDocument &doc);
	void updateNodePreNextIdList();
	int8_t transformId(int8_t oldId);
	std::shared_ptr<SurperNode> getNodePtr(int8_t nodeId)
	{
		return m_nodeMap[nodeId];
	}

	inline bool checkProjectHasError()
	{
		return RETURN_NORMAL == m_error;
	}
	std::shared_ptr<SurperNode> createSurperNode(std::shared_ptr<SuperNodeStruct> pNodeInfo);
	std::list<vtkSmartPointer<vtkProp>> getAddPropList()
	{
		std::list<vtkSmartPointer<vtkProp>> list;
		list.resize(m_addPropList.size());
		std::copy(m_addPropList.begin(), m_addPropList.end(), list.begin());
		m_addPropList.clear();
		return list;
	}
	std::list<vtkSmartPointer<vtkProp>> getRemovePropList()
	{
		std::list<vtkSmartPointer<vtkProp>> list;
		list.resize(m_removePropList.size());
		std::copy(m_removePropList.begin(), m_removePropList.end(), list.begin());
		m_removePropList.clear();
		return list;
	}

private:
	vtkSmartPointer<vtkRenderer> m_render;
	vtkSmartPointer< vtkRenderWindowInteractor> m_WinInteractor;
	std::map<int8_t, std::shared_ptr<SurperNode>> m_nodeMap;
	uint16_t m_ListNodeNum;
	int8_t m_projectId;
	bool m_isSaved;
	std::string m_projectName;
	std::string m_ProjectXml;
	ProjectError m_error;
	bool m_hasError = false;
	stringlist m_errorList;
	std::map<int8_t, int8_t> m_readXmlMapTable;
	callBack * m_callBack;
	std::shared_ptr<void> m_pLogic;
	std::list<vtkSmartPointer<vtkProp>> m_addPropList;
	std::list<vtkSmartPointer<vtkProp>> m_removePropList;
	std::list<vtkSmartPointer<vtkProp>> m_propList;
	std::list<std::list<int8_t>> m_taskList;

	workRunThread* m_workThreadObj;
};

class workRunThread
{
public:
	workRunThread()
	{
		m_isrun = true;
		m_workThread = new std::thread(&workRunThread::run, this);
	}
	void setProjectPtr(XmlProject* p)
	{
		m_project = p;
	}
	bool addFuncTask(std::function<bool()> _func)
	{
		if (!m_isrun)return false;
		std::lock_guard<std::mutex> _lock(m_listMutex);
		m_funcList.push_back(_func);
		m_var.notify_one();
		return true;
	}
private:
	void run()
	{
		while (m_isrun)
		{
			std::function<bool()> _func = getFuncTask();
			_func();
		}
	}

	std::function<bool()> getFuncTask()
	{
		std::unique_lock<std::mutex> _lock(m_listMutex);
		if (m_funcList.empty())
		{
			m_var.wait(_lock);
		}
		std::function<bool()> _func = m_funcList.front();
		m_funcList.pop_front();
		return _func;
	}
private:
	bool m_isrun = false;
	std::thread * m_workThread = nullptr;
	std::list < std::function<bool()>> m_funcList;
	std::mutex m_listMutex;
	std::condition_variable m_var;
	XmlProject* m_project;
};
