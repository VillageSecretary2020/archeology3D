#include "NodeTask.h"

NodeTask::NodeTask()
{
	m_nodeList.clear();
}

NodeTask::~NodeTask()
{
}

int NodeTask::doTask()
{

	return 0;
}

int NodeTask::initTask(std::list<std::shared_ptr<SurperNode>> nodeList)
{
	return 0;
}

bool NodeTask::checkInTask(_int_4 id)
{
	std::list<std::shared_ptr<SurperNode>>::iterator it = std::find_if(m_nodeList.begin(), m_nodeList.end(), [this, &id](std::shared_ptr<SurperNode> node)->bool
		{
			return node->getNodeIndex() == id;
		});

	return it != m_nodeList.end();
}
