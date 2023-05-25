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
#ifndef __nodetask_h_
#define __nodetask_h_

#include "SurperNode.h"
#include <algorithm>

class NodeTask : public std::enable_shared_from_this<NodeTask>
{
public:
	NodeTask();
	~NodeTask();

	int doTask();
	int initTask(std::list<std::shared_ptr<SurperNode>> nodeList);
	bool checkInTask(_int_4 id);
private:
	std::list<std::shared_ptr<SurperNode>> m_nodeList;
};

#endif // !__nodetask_h_

