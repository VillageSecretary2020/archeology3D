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

#include "TaskSever.h"
#include <ctime>


class SystermLog
{
private:
	SystermLog();
public:
	~SystermLog();
	static void init();
	static void info(const char* pStr);
	template<typename ...Args>
	static void info(const char* pformat, Args ...args);
	static void error();
	static void debug();
	static void warrn();
	void setFilePath(const char* logPath, const char* mode);
	static SystermLog* getInstance();
	FILE* getFile()
	{
		return m_file;
	}

protected:
	std::shared_ptr<TaskSever> m_taskSever;
	FILE* m_file;
};

