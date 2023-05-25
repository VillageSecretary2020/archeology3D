#include "WorkThread.h"

WorkThread::WorkThread()
{
}

WorkThread::~WorkThread()
{
	while (m_isWait)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void WorkThread::start(Func run)
{
	std::thread t(&WorkThread::onRun, this);
	t.detach();
}

void WorkThread::close()
{
	m_isRun = false;
	while (m_isWait)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void WorkThread::onRun()
{
	if (onRunFunc)
		onRunFunc(shared_from_this());
	m_isWait = false;
}
