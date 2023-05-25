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
#ifndef __workthread_h_
#define __workthread_h_
#include <functional>
#include <memory>
#include <thread>


class WorkThread : public std::enable_shared_from_this<WorkThread>
{
	typedef std::function<void(std::shared_ptr<WorkThread>)> Func;
public:
	WorkThread();
	~WorkThread();
	void start(Func run);
	void close();
	bool isRun()
	{
		return m_isRun;
	}
private:
	void onRun();
private:
	Func onRunFunc = nullptr;
	bool m_isWait = true;
	bool m_isRun = true;
};
#endif // !__workthread_h_
