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

#include "OperationLogic.h"
#include <algorithm>

#define getOperationalLayerInstance OperationalLayer::getOperationInstance() 
class OperationalLayer
{
	typedef void (ProjectWidget_callback)(std::shared_ptr<LogicMsgHeader> msg);
	typedef void (OtherWidget_callback)(std::shared_ptr<LogicMsgHeader> msg);

	typedef void (GUICallBack)(void* ptr, int8_t projectId);
	typedef void (XYZLimits_cb)(int* limits, void* widget);//回调函数类型， limits 为 表面渲染几何属性的轴节点范围，[0]:X 轴，【1】：Y轴，【2】：Z轴, widget:控件所在类指针
	typedef void (updata_info_cb)(int num_blocks, float average_blocks, float volume, void* widget);//块数量，平均块数量，体积,控件所在类指针

	//for test mesh cb
	//typedef void (close_mesh_widget)(void* widget, vtkSmartPointer<vtkRenderer> render);
public:
	~OperationalLayer();
	static OperationalLayer& getOperationInstance();
	void setGUICallBack(GUICallBack* fun, void *ptr);
	void* getMainWin();
	//project
	int8_t newProject(std::string xmlPath="d:\\projectFold");
	int8_t openProject(std::string xmlPath);
	int8_t setVtkRender(vtkSmartPointer<vtkRenderer> render, int8_t projectId);
	int8_t setRenderWinInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor, int8_t projectId);
	int8_t saveProject(int8_t projectId, std::string xmlPath = "");
	int8_t saveAllProject();
	int8_t closeProject(int8_t projectId);
	bool closeSysterm();
	bool saveAndCloseSysterm();

	static std::shared_ptr<TiXmlNode> OperationalLayer::getCopyNodeXml();
	static void setCopyNodeXml(std::shared_ptr<TiXmlNode>);
	uint16_t addNodeIntoProject(int8_t projectId, std::shared_ptr<SuperNodeStruct> NodeInfo);
	//创建表面渲染模块时，将更新控件参数回调函数传入
	uint16_t setSurfaceRender_cb(uint16_t key, XYZLimits_cb* limits_func, updata_info_cb* info_cb, void *ptr);
	uint16_t modifyNodeIntoProject(uint16_t key, std::shared_ptr<SuperNodeStruct> NodeInfo);
	uint16_t modifyNodeBaseInfo(uint16_t key, std::shared_ptr<SuperNodeStruct> baseInfo);
	uint16_t modifyNodeAvailable(uint16_t key, bool available);
	bool deleteNodeFromProject(uint16_t key);
	 
	std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> getProjectNodeList(int8_t projectId);
	std::multimap<uint16_t, uint16_t> getProjectNodeListConnection(int8_t projectId);
	bool setNodeConnection(int16_t firstKey, int16_t secondKey);
	bool removeNodeConnection(int16_t firstKey, int16_t secondKey);
	std::shared_ptr<SuperNodeStruct> getSpecifiedNodeInfo(int16_t key);
	int16_t getExcutedCaculateNode(int16_t firstKey);// yzw 0526 查找已计算的上级计算节点
	int16_t nextNodeRun(int16_t firstKey);// yzw 0525
	int16_t nextGraphNodeRun(int16_t firstKey);// lizi 0706
	int16_t aloneNodesRun(int16_t firstKey);// yzw 0727
	std::list<vtkSmartPointer<vtkProp>> getProjectProp(int8_t projectid);

	void func_excut(uint16_t id_key, BOOL_FUNC func = nullptr);
	//vtkSmartPointer<vtkProp> getActorFromProject(int8_t projectId, close_mesh_widget func = nullptr, void *widget = nullptr);
	void addFuncTask(uint8_t id_key, BOOL_FUNC);
	
	void testDoMeshNode(int keyId);
public:
	void addLogicMsg(std::shared_ptr<LogicMsgHeader> msg);
	stringlist getLogicMsgErrorResult(std::weak_ptr<LogicMsgHeader> msg);
	void notifyCallBack(int8_t projectId);
	
private:
	OperationalLayer();

#if 0
	void doNext(std::shared_ptr<SurperNode> nodefist, std::shared_ptr<SurperNode> nodeNext, std::shared_ptr<XmlProject> project);
	void doBehind(std::shared_ptr<SurperNode> node, std::shared_ptr<XmlProject> project);
	void linkDataNode(std::shared_ptr<DDataNode> dataNode, std::shared_ptr<SurperNode> nodeSecond);
	void linkMeshNoe(std::shared_ptr<CMeshNode> meshNode, std::shared_ptr<SurperNode> nodeSeconde);
#endif

private:
	std::list<std::shared_ptr<LogicMsgHeader>> m_logicMsgList;
	std::mutex m_logicMsgListMutex;
	//ProjectWidget_callback* m_projectRunCallBack;
	//OtherWidget_callback* m_otherRunCallBack;
	//std::weak_ptr<void> m_projectWidget;
	//std::weak_ptr<void> m_otherWidget;
	GUICallBack* m_callBack;
	void *m_ptr;

	std::thread* m_mesh_thread;
};
