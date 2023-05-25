#include "TaskSever.h"

TaskSever::TaskSever() : m_workThread(std::make_shared<WorkThread>())
{
	m_taskList.clear();
	m_workThread->start([this](std::shared_ptr<WorkThread> pthread) {
		onRun(pthread);
		});
}

TaskSever::~TaskSever()
{
	close();
}

void TaskSever::addTask(Task task)
{
	std::lock_guard<std::mutex> lock(m_taskListMutex);
	m_taskList.push_back(task);
}

void TaskSever::onRun(std::shared_ptr<WorkThread> pthread)
{
	while (pthread->isRun())
	{
		if (m_taskList.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		std::lock_guard<std::mutex> lock(m_taskListMutex);
		Task task = m_taskList.front();
		m_taskList.pop_front();
		task();
	}
}

void TaskSever::close()
{
	m_workThread->close();
}
