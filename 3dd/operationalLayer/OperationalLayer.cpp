// OperationalLayer.cpp

#include <QApplication>
#include <QCursor>

#include "NodeHeader.h"
#include "OperationalLayer.h"

OperationalLayer::OperationalLayer()
{
}

OperationalLayer::~OperationalLayer()
{
}

OperationalLayer & OperationalLayer::getOperationInstance()
{
	static OperationalLayer layer;
	return layer;
}

void OperationalLayer::setGUICallBack(GUICallBack* fun, void *ptr)
{
	m_callBack = fun;
	m_ptr = ptr;
}

void* OperationalLayer::getMainWin()
{
	return m_ptr;
}

int8_t OperationalLayer::newProject(std::string xmlPath)
{
	return OperationLogic::getOperationLogicInstance()->newProject(xmlPath);
}

int8_t OperationalLayer::openProject(std::string xmlPath)
{
	return OperationLogic::getOperationLogicInstance()->openProject(xmlPath);
}

int8_t OperationalLayer::setVtkRender(vtkSmartPointer<vtkRenderer> render, int8_t projectId)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	project->setRender(render);
	return projectId;
}

int8_t OperationalLayer::setRenderWinInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor, int8_t projectId)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return RETURN_ERROR;
	project->setRenderWinInteractor(interactor);
	return projectId;
}

int8_t OperationalLayer::saveProject(int8_t projectId, std::string xmlPath)
{
	return OperationLogic::getOperationLogicInstance()->saveProject(projectId, xmlPath);
}

int8_t OperationalLayer::saveAllProject()
{
	return OperationLogic::getOperationLogicInstance()->saveAllProject();
}

int8_t OperationalLayer::closeProject(int8_t projectId)
{
	if (OperationLogic::getOperationLogicInstance()->closeProject(projectId))
		return projectId;
	return RETURN_ERROR;
}

bool OperationalLayer::closeSysterm()
{
	return RETURN_NORMAL == OperationLogic::getOperationLogicInstance()->closeSystem();
}

bool OperationalLayer::saveAndCloseSysterm()
{
	return RETURN_NORMAL == OperationLogic::getOperationLogicInstance()->saveAndCloseSysterm();
}

uint16_t OperationalLayer::addNodeIntoProject(int8_t projectId, std::shared_ptr<SuperNodeStruct> NodeInfo)
{
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	int8_t nodeId = project->addNode(NodeInfo);
	return getKeyById(projectId, nodeId);
}

static std::shared_ptr<TiXmlNode> copied_node_xml;

std::shared_ptr<TiXmlNode> OperationalLayer::getCopyNodeXml( )
{
	return copied_node_xml ;
}

void OperationalLayer::setCopyNodeXml(std::shared_ptr<TiXmlNode> pn)
{
	copied_node_xml = pn;
}

uint16_t OperationalLayer::setSurfaceRender_cb(uint16_t key, XYZLimits_cb* limits_func, updata_info_cb* info_cb, void *ptr)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if (!project)
		return 0;
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	if (!node)
		return 0;
	std::shared_ptr<GSurfaceNode> surfaceRender = std::dynamic_pointer_cast<GSurfaceNode>(node);
	if (!surfaceRender)
		return 0;
	surfaceRender->setCallbackFun(limits_func, info_cb, ptr);
	return key;
}

uint16_t OperationalLayer::modifyNodeIntoProject(uint16_t key, std::shared_ptr<SuperNodeStruct> NodeInfo)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if (!project)
		return RETURN_ERROR;
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	node->setParameter(NodeInfo);
	project->setProjectSavedStatus();
	QApplication::restoreOverrideCursor();
	return key;
}

uint16_t OperationalLayer::modifyNodeBaseInfo(uint16_t key, std::shared_ptr<SuperNodeStruct> baseInfo)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if (!project)
		return RETURN_ERROR;
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	node->setBaseInfo(baseInfo);
	project->setProjectSavedStatus();
	return key;
}

uint16_t OperationalLayer::modifyNodeAvailable(uint16_t key, bool available)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if (!project)
		return RETURN_ERROR;

	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	if (!node)
		return RETURN_ERROR;
	node->setAvailable(available);
	return key;
}

bool OperationalLayer::deleteNodeFromProject(uint16_t key)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if (!project)
		return false;
	project->removeNode(getNodeId(key));
	return true;
}

std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> OperationalLayer::getProjectNodeList(int8_t projectId)
{
	std::list<std::shared_ptr<SuperNodeStruct>> list = OperationLogic::getOperationLogicInstance()->getProjectNodeList(projectId);
	std::map<uint16_t, std::shared_ptr<SuperNodeStruct>> map_list;
	map_list.clear();
	for (auto var :  list)
	{
		uint16_t key = getKeyById(projectId, var->m_nodeIndex);
		var->m_nodeIndex = RETURN_ERROR;
		map_list.insert(std::pair < uint16_t, std::shared_ptr<SuperNodeStruct>>(key, var));
	}
	return map_list;
}

std::multimap<uint16_t, uint16_t> OperationalLayer::getProjectNodeListConnection(int8_t projectId)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return std::multimap<uint16_t, uint16_t>();
	std::multimap<int8_t, int8_t> idMap = project->getNodeListConnection();
	std::multimap<uint16_t, uint16_t> _map;
	_map.clear();
	for (auto var : idMap)
	{
		_map.insert(std::pair<uint16_t, uint16_t>(getKeyById(projectId, var.first), getKeyById(projectId, var.second)));
	}
	return _map;
}

bool OperationalLayer::setNodeConnection(int16_t firstKey, int16_t secondKey)
{
	if (getProjectId(firstKey) != getProjectId(secondKey))
		return false;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(firstKey));
	if (!project)
		return false;
	std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getNodeId(firstKey));
	std::shared_ptr<SurperNode> secondNode = project->getSpecifiedNode(getNodeId(secondKey));
	if (!firstNode || !secondNode)
		return false;
	secondNode->addConnectRelation(getNodeId(firstKey));
	firstNode->addConnectRelation(getNodeId(secondKey), false);
	return true;
}

bool OperationalLayer::removeNodeConnection(int16_t firstKey, int16_t secondKey)
{
	if (getProjectId(firstKey) != getProjectId(secondKey))
		return false;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(firstKey));
	if (!project)
		return false;
	std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getNodeId(firstKey));
	std::shared_ptr<SurperNode> secondNode = project->getSpecifiedNode(getNodeId(secondKey));
	if (!firstNode || !secondNode)
		return false;
	firstNode->removeConnectRelation(getNodeId(secondKey), false);
	secondNode->removeConnectRelation(getNodeId(firstKey));
	return true;
}

std::shared_ptr<SuperNodeStruct> OperationalLayer::getSpecifiedNodeInfo(int16_t key)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(key));
	if(!project)
		return std::shared_ptr<SuperNodeStruct>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(getNodeId(key));
	if(!node)
		return std::shared_ptr<SuperNodeStruct>();
	return XmlProject::getNodeParameter(node);
}

std::list<vtkSmartPointer<vtkProp>> OperationalLayer::getProjectProp(int8_t projectid)
{
	return std::list<vtkSmartPointer<vtkProp>>();
}

void OperationalLayer::func_excut(uint16_t id_key, BOOL_FUNC func)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(getProjectId(id_key));
	if (!project)
		return;

	project->caculateMesh(getNodeId(id_key));
	project->addFuncTask(func);
}

void OperationalLayer::addFuncTask(uint8_t id_key, BOOL_FUNC pf)
{
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(id_key);
	if (!project)
		return;

	project->addFuncTask(pf);
}

#if 0
vtkSmartPointer<vtkProp> OperationalLayer::getActorFromProject(int8_t projectId, close_mesh_widget func, void *widget)
{
	int m = 100;
	std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok = node1->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_1 = std::dynamic_pointer_cast<CMeshNode>(node2);
	std::vector<pointOriginal> _data;
	bool ok1 = node1->getOnputData(_data);
	node2_1->setInputData(_data, 0);
	project->setCloseMeshFunc(func, widget);
	project->caculateMesh(2);
	//m_mesh_thread = new std::thread([&, projectId,func, widget]() -> vtkSmartPointer<vtkProp> {
		
		//func_excut(projectId);
		//测试导入数据+散点
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if(!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok = node1->run();

	std::shared_ptr<SurperNode> node_ = project->getSpecifiedNode(2);
	std::shared_ptr<GScatterMapNode> node2 = std::dynamic_pointer_cast<GScatterMapNode>(node_);
	std::vector<pointOriginal> _data;
	bool ok1 =node1->getOnputData(_data);
	node2->setInputData(_data, 0);
	bool ok3 = node2->run();
	return node2->getActor();*/

	//测试注解
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<GCommentNode> node1 = std::dynamic_pointer_cast<GCommentNode>(node);
	bool ok = node1->run();
	return vtkSmartPointer<vtkProp>();*/

	//表面渲染
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return vtkSmartPointer<vtkActor>();
	if (!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok = node1->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_1 = std::dynamic_pointer_cast<CMeshNode>(node2);
	std::vector<pointOriginal> _data;
	bool ok1 = node1->getOnputData(_data);
	node2_1->setInputData(_data, 0);
	bool ok2 = node2_1->run();
	std::vector<pointFinal> pointData_;
	bool ok_1 = node2_1->getOnputData(pointData_);
	std::shared_ptr<SurperNode> node_temp = project->getSpecifiedNode(3);
	std::shared_ptr<GSurfaceNode> node_surface = std::dynamic_pointer_cast<GSurfaceNode>(node_temp);
	node_surface->setInputData(pointData_);
	node_surface->setDataRange(node2_1->getDataRange());
	node_surface->setResolution(node2_1->getResolution());
	bool surface_ok = node_surface->run();*/

	//文本+散点
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok1 = node1->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(3);
	std::shared_ptr<GScatterMapNode> node2_ = std::dynamic_pointer_cast<GScatterMapNode>(node2);
	std::vector<pointOriginal> _data;
	node1->getOnputData(_data);
	node2_->setInputData(_data, 0);
	bool ok2 = node2_->run();

	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(2);
	node3->run();*/

	//正交
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if(!node1_)
		return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();

	std::vector<pointOriginal> dataVec;
	ok1 = node1_->getOnputData(dataVec);

	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if(!node2_)
		return vtkSmartPointer<vtkProp>();
	node2_->setInputData(dataVec, 0);
	bool ok2 = node2_->run();

	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GOrthonalFactorNode> node3_ = std::dynamic_pointer_cast<GOrthonalFactorNode>(node3);
	if (!node3_)
		return vtkSmartPointer<vtkProp>();
	std::vector<pointFinal> tempDataVec;
	ok2 = node2_->getOnputData(tempDataVec);
	node3_->setInputData(tempDataVec);

	node3_->setResolution(node2_->getResolution());
	node3_->setSpacing(node2_->getspacing());
	node3_->setLimits(node2_->getLimits());
	bool ok3 = node3_->run();*/

	//形体渲染
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if(!node1_)
		return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if(!node2_)
		return vtkSmartPointer<vtkProp>();
	std::vector<pointOriginal> DataVec;
	node1_->getOnputData(DataVec);
	node2_->setInputData(DataVec, 0);
	bool ok2 = node2_->run();
	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GVolumeRenderNode> node3_ = std::dynamic_pointer_cast<GVolumeRenderNode>(node3);
	if(!node3_)
		return vtkSmartPointer<vtkProp>();;
	std::vector<pointFinal> dataTemp;
	node2_->getOnputData(dataTemp);
	node3_->setInputData(dataTemp);
	node3_->setDataRange(node2_->getDataRange());
	node3_->setResolution(node2_->getResolution());
	bool ok3 = node3_->run();*/

	//切片
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if (!project)
	return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if (!node1_)
	return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if (!node2_)
	return vtkSmartPointer<vtkProp>();
	std::vector<pointOriginal> DataVec;
	node1_->getOnputData(DataVec);
	node2_->setInputData(DataVec, 0);
	bool ok2 = node2_->run();
	std::vector<pointFinal> _tempdata;
	node2_->getOnputData(_tempdata);
	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GSliceMapNode> node3_ = std::dynamic_pointer_cast<GSliceMapNode>(node3);
	if(!node3)
		return vtkSmartPointer<vtkProp>();
	node3->setInputData(_tempdata);
	node3_->setDataRange(node2_->getDataRange());
	node3_->setResolution(node2_->getResolution());
	node3_->run();*/

	/*等值面*/
		/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
		if (!project)
			return vtkSmartPointer<vtkProp>();
		std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
		std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
		if (!node1_)
			return vtkSmartPointer<vtkProp>();
		bool ok1 = node1_->run();
		std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
		std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
		if (!node2_)
			return vtkSmartPointer<vtkProp>();
		std::vector<pointOriginal> DataVec;
		node1_->getOnputData(DataVec);
		node2_->setInputData(DataVec, 0);
		bool ok2 = node2_->run();
		std::vector<pointFinal> _tempdata;
		node2_->getOnputData(_tempdata);

		std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
		std::shared_ptr<GIsoSurfaceNode> node3_ = std::dynamic_pointer_cast<GIsoSurfaceNode>(node3);
		if (!node3_)
			return vtkSmartPointer<vtkProp>();

		node3_->setDataRange(node2_->getDataRange());
		node3_->setResolution(node2_->getResolution());
		node3_->setInputData(_tempdata);
		node3_->run();*/


		//cb
		/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
		if (!project)
			return vtkSmartPointer<vtkActor>();*/
		/*if(func)
			func(widget, project->getRender());

		return vtkSmartPointer<vtkActor>();
		});*/
	//测试导入数据+散点
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if(!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok = node1->run();
	
	std::shared_ptr<SurperNode> node_ = project->getSpecifiedNode(2);
	std::shared_ptr<GScatterMapNode> node2 = std::dynamic_pointer_cast<GScatterMapNode>(node_);
	std::vector<pointOriginal> _data;
	bool ok1 =node1->getOnputData(_data);
	node2->setInputData(_data, 0);
	bool ok3 = node2->run();
	return node2->getActor();*/

	//测试注解
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<GCommentNode> node1 = std::dynamic_pointer_cast<GCommentNode>(node);
	bool ok = node1->run();
	return vtkSmartPointer<vtkProp>();*/

	//表面渲染
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	if (!project)
		return vtkSmartPointer<vtkActor>();
	if (!project)
		return vtkSmartPointer<vtkActor>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok = node1->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_1 = std::dynamic_pointer_cast<CMeshNode>(node2);
	std::vector<pointOriginal> _data;
	bool ok1 = node1->getOnputData(_data);
	node2_1->setInputData(_data, 0);
	bool ok2 = node2_1->run();
	std::vector<pointFinal> pointData_;
	bool ok_1 = node2_1->getOnputData(pointData_);
	std::shared_ptr<SurperNode> node_temp = project->getSpecifiedNode(3);
	std::shared_ptr<GSurfaceNode> node_surface = std::dynamic_pointer_cast<GSurfaceNode>(node_temp);
	node_surface->setInputData(pointData_);
	node_surface->setDataRange(node2_1->getDataRange());
	node_surface->setResolution(node2_1->getResolution());
	bool surface_ok = node_surface->run();*/

	//文本+散点
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1 = std::dynamic_pointer_cast<DDataNode>(node);
	bool ok1 = node1->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(3);
	std::shared_ptr<GScatterMapNode> node2_ = std::dynamic_pointer_cast<GScatterMapNode>(node2);
	std::vector<pointOriginal> _data;
	node1->getOnputData(_data);
	node2_->setInputData(_data, 0);
	bool ok2 = node2_->run();

	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(2);
	node3->run();*/

	//正交
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if(!node1_)
		return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();

	std::vector<pointOriginal> dataVec;
	ok1 = node1_->getOnputData(dataVec);

	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if(!node2_)
		return vtkSmartPointer<vtkProp>();
	node2_->setInputData(dataVec, 0);
	bool ok2 = node2_->run();

	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GOrthonalFactorNode> node3_ = std::dynamic_pointer_cast<GOrthonalFactorNode>(node3);
	if (!node3_)
		return vtkSmartPointer<vtkProp>();
	std::vector<pointFinal> tempDataVec;
	ok2 = node2_->getOnputData(tempDataVec);
	node3_->setInputData(tempDataVec);
	
	node3_->setResolution(node2_->getResolution());
	node3_->setSpacing(node2_->getspacing());
	node3_->setLimits(node2_->getLimits());
	bool ok3 = node3_->run();*/

	//形体渲染
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if(!project)
		return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if(!node1_)
		return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if(!node2_)
		return vtkSmartPointer<vtkProp>();
	std::vector<pointOriginal> DataVec;
	node1_->getOnputData(DataVec);
	node2_->setInputData(DataVec, 0);
	bool ok2 = node2_->run();
	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GVolumeRenderNode> node3_ = std::dynamic_pointer_cast<GVolumeRenderNode>(node3);
	if(!node3_)
		return vtkSmartPointer<vtkProp>();;
	std::vector<pointFinal> dataTemp;
	node2_->getOnputData(dataTemp);
	node3_->setInputData(dataTemp);
	node3_->setDataRange(node2_->getDataRange());
	node3_->setResolution(node2_->getResolution());
	bool ok3 = node3_->run();*/

	//切片
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if (!project)
	return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if (!node1_)
	return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if (!node2_)
	return vtkSmartPointer<vtkProp>();
	std::vector<pointOriginal> DataVec;
	node1_->getOnputData(DataVec);
	node2_->setInputData(DataVec, 0);
	bool ok2 = node2_->run();
	std::vector<pointFinal> _tempdata;
	node2_->getOnputData(_tempdata);
	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GSliceMapNode> node3_ = std::dynamic_pointer_cast<GSliceMapNode>(node3);
	if(!node3)
		return vtkSmartPointer<vtkProp>();
	node3->setInputData(_tempdata);
	node3_->setDataRange(node2_->getDataRange());
	node3_->setResolution(node2_->getResolution());
	node3_->run();*/

	/*等值面*/
	/*std::shared_ptr<XmlProject> project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(1);
	if (!project)
	return vtkSmartPointer<vtkProp>();
	std::shared_ptr<SurperNode> node1 = project->getSpecifiedNode(1);
	std::shared_ptr<DDataNode> node1_ = std::dynamic_pointer_cast<DDataNode>(node1);
	if (!node1_)
	return vtkSmartPointer<vtkProp>();
	bool ok1 = node1_->run();
	std::shared_ptr<SurperNode> node2 = project->getSpecifiedNode(2);
	std::shared_ptr<CMeshNode> node2_ = std::dynamic_pointer_cast<CMeshNode>(node2);
	if (!node2_)
	return vtkSmartPointer<vtkProp>();
	std::vector<pointOriginal> DataVec;
	node1_->getOnputData(DataVec);
	node2_->setInputData(DataVec, 0);
	bool ok2 = node2_->run();
	std::vector<pointFinal> _tempdata;
	node2_->getOnputData(_tempdata);

	std::shared_ptr<SurperNode> node3 = project->getSpecifiedNode(3);
	std::shared_ptr<GIsoSurfaceNode> node3_ = std::dynamic_pointer_cast<GIsoSurfaceNode>(node3);
	if(!node3_)
		return vtkSmartPointer<vtkProp>();

	node3_->setDataRange(node2_->getDataRange());
	node3_->setResolution(node2_->getResolution());
	node3_->setInputData(_tempdata);
	node3_->run();*/
	return vtkSmartPointer<vtkProp>();
}
#endif

int16_t OperationalLayer::getExcutedCaculateNode(int16_t firstKey)
{
	int16_t cnKey = 0;
	int8_t projectId = getProjectId(firstKey);
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getNodeId(firstKey));
	while (firstNode->m_nodePreNodelist.size()){
		firstNode = project->getSpecifiedNode(getKeyById(projectId, *(firstNode->m_nodePreNodelist.begin())));
		if (NODE_C_MESH_TYPE == firstNode->getNodeType()) {
			std::shared_ptr<CMeshNode> meshNode = std::dynamic_pointer_cast<CMeshNode>(firstNode);
			if(meshNode->getExcuted())	// yzw 0526
				cnKey = getKeyById(projectId, firstNode->getNodeIndex());
			break;
		}
	}
	return cnKey;
}

int16_t OperationalLayer::nextNodeRun(int16_t firstKey)
{
	int8_t projectId = getProjectId(firstKey);
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getNodeId(firstKey));
	if (NODE_C_MESH_TYPE == firstNode->getNodeType()) {
		std::shared_ptr<CMeshNode> node = std::dynamic_pointer_cast<CMeshNode>(firstNode);
		if (node) {
			node->makeNextNodeRun();
		}
	}else if (NODE_D_DATASOURCE_TYPE == firstNode->getNodeType()) {
		std::shared_ptr<DDataNode> node = std::dynamic_pointer_cast<DDataNode>(firstNode);
		if (node) {
			node->makeNextNodeRun();
		}
	}
	return 0;
}

int16_t OperationalLayer::nextGraphNodeRun(int16_t firstKey)
{
	int8_t projectId = getProjectId(firstKey);
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getNodeId(firstKey));
	std::shared_ptr<GraphNode> graphNode = std::dynamic_pointer_cast<GraphNode>(firstNode);
	if (graphNode) {
		graphNode->run();
	}
	return 0;
}

int16_t OperationalLayer::aloneNodesRun(int16_t firstKey)
{
	int16_t ic = 0;
	int8_t projectId = getProjectId(firstKey);
	auto project = OperationLogic::getOperationLogicInstance()->getSpecifiedProject(projectId);
	int8_t nodeId = 0;
	do{
		nodeId = project->getNextAloneNodeId(nodeId);
		if(0< nodeId){
			std::shared_ptr<SurperNode> firstNode = project->getSpecifiedNode(getKeyById(projectId, nodeId));
			if (NODE_D_DATASOURCE_TYPE == firstNode->getNodeType())
				continue;
			if(NODE_G_TEXT_TYPE ==firstNode->getNodeType()	//getBelongType()
				|| NODE_G_COMMENT_TYPE ==firstNode->getNodeType()	// getBelongType()
				)
			 {
				std::shared_ptr<GraphNode> gNode = std::dynamic_pointer_cast<GraphNode>(firstNode);
				gNode->run();
				ic++;
			}
		}
	} while (nodeId);
	return ic;
}

