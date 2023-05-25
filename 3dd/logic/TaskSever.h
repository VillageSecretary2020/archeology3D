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
#ifndef __taskSever_h_
#define __taskSever_h_

#include "WorkThread.h"
#include <list>
#include <mutex>

class TaskSever
{
	typedef std::function<void()> Task;
public:
	TaskSever();
	~TaskSever();
	void addTask(Task task);

	void onRun(std::shared_ptr<WorkThread> pthread);
	void close();
private:
	std::mutex m_taskListMutex;
	std::list<Task> m_taskList;
	std::shared_ptr<WorkThread> m_workThread;
};

#endif // !__taskSever_h_

