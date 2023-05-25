#include "CaculateNode.h"
#include "XmlProject.h"

CaculateNode::CaculateNode(std::shared_ptr< XmlProject> project):SurperNode(project)
{
	m_nodeBelongType = NodeType::NODE_C_TYPE;
}

CaculateNode::~CaculateNode()
{
}

std::shared_ptr<SuperNodeStruct> CaculateNode::getParameter()
{
	return nullptr;
}

void CaculateNode::setInputData(std::vector<pointFinal>& data)
{
	m_data = data;
}

bool CaculateNode::getOnputData(std::vector<pointFinal>& data)
{
	if(!m_project.lock() || m_nodeIndex == RETURN_ERROR)
		return false;
	data = m_data;
	return true;
}

void CaculateNode::setInputData(std::vector<pointOriginal>& data, int dataIndex)
{
	m_data.clear();
	for  (auto var : data)//for each (auto var in data)
	{
		m_data.push_back(pointFinal(var.x, var.y, var.z, var.scalar[dataIndex]));
	}
}

bool CaculateNode::nextNodeRun(int8_t nId)
{
	std::shared_ptr<SurperNode> sn = m_project.lock()->getSpecifiedNode(nId);
	NodeType nt = sn->getNodeType();
	if (NODE_G_TYPE == sn->getBelongType()) { //Test if GraphNode
		std::shared_ptr<GraphNode> node = std::dynamic_pointer_cast<GraphNode>(sn);
		std::vector<pointFinal> _data;
		bool ok1 = getOnputData(_data);
		node->setDataRange(getDataRange());
		node->setResolution(getResolution());
		node->setLimits(getLimits());
		node->setOriginData(&m_data);
		node->setInputData(_data);
		node->run();
	}
	else if (NODE_C_SUBSET_TYPE == nt
		|| NODE_C_CONVERSION_TYPE == nt
		|| NODE_C_ELIMINATIONFILTER_TYPE == nt
		) {
		//std::shared_ptr<CSubsetNode> subsetNode = std::dynamic_pointer_cast<CSubsetNode>(m_project.lock()->getSpecifiedNode(nId));
		std::shared_ptr<CaculateNode> subsetNode = std::dynamic_pointer_cast<CaculateNode>(m_project.lock()->getSpecifiedNode(nId));
		if (subsetNode.get()) {
			std::vector<pointFinal> _data;
			bool ok1 = getOnputData(_data);
			subsetNode->setInputData(_data);
			subsetNode->setDataRange(getDataRange());
			subsetNode->setResolution(getResolution());
			subsetNode->setLimits(getLimits());
			subsetNode->run();
		}
	}
	return true;
}

bool CaculateNode::saveData(std::string path)
{
	if (m_excuted) {
		//cpl 7.29
		std::ofstream ofs(path);
		if (!ofs.is_open())
		{
			return false;
		}
		else
		{
			ofs << "x\ty\tz\tscalar\n";
			for (int i = 0; i < m_data.size(); i++)
			{
				ofs << (double)m_data.at(i).x << "\t" 
					<< (double)m_data.at(i).y << "\t" 
					<< (double)m_data.at(i).z << "\t" 
					<< (double)m_data.at(i).scalar << "\n";
			}
			ofs.flush();
			ofs.close();
		}
	}
	return true;
}

#include <setjmp.h>
#include <signal.h>
#ifndef sigsetjmp
#define sigsetjmp(p1, p2) setjmp(p1)
#define siglongjmp longjmp
typedef void(*sighandler_t)(int);
#define _crt_signal_t sighandler_t 
#endif
void CaculateNode::makeNextNodeRun()
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
		if(m_excuted)		do {
			// int* pi = (int*)nullptr;
			// test  *pi = 555;
			for (auto nId : m_nodeNextNodeList)
			{
				nodeId = nId;
				nextNodeRun(nId);
			}	// end for
		} while (0);
		hpre = signal(SIGSEGV, hpre);
}
