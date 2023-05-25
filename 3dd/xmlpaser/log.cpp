#include "log.h"

SystermLog::SystermLog() : m_taskSever(std::make_shared<TaskSever>())
{
}

SystermLog::~SystermLog()
{
	m_taskSever->close();
}

void SystermLog::init()
{
	static SystermLog log;
}

void SystermLog::info(const char* pStr)
{
	SystermLog* log = getInstance();
	log->m_taskSever->addTask([=]()
		{
			if (log->getFile())
			{
				auto t = std::chrono::system_clock::now();
				auto tNow = std::chrono::system_clock::to_time_t(t);
				fprintf(log->getFile(), "%s", ctime(&tNow));
				std::tm* now = std::gmtime(&tNow);
				fprintf(log->getFile(), "%s", "Info ");
				fprintf(log->getFile(), "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
				fprintf(log->getFile(), "%s", pStr);
				fflush(log->getFile());
			}
			printf("%s", pStr);
		});
}

template<typename ...Args>
inline void SystermLog::info(const char * pformat, Args ...args)
{
	SystermLog* pLog = getInstance();
	pLog->m_taskSever->addTask([=]() {
		if (pLog->m_file)
		{
			auto t = std::chrono::system_clock::now();
			auto tNow = std::chrono::system_clock::to_time_t(t);
			//fprintf(pLog->_logFile, "%s", ctime(&tNow));
			std::tm* now = std::gmtime(&tNow);
			fprintf(pLog->m_file, "%s", "Info ");
			fprintf(pLog->m_file, "[%d-%d-%d %d:%d:%d]", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
			fprintf(pLog->m_file, pformat, args...);
			fflush(pLog->m_file);
		}
		printf(pformat, args...);
		});
}

void SystermLog::error()
{
}

void SystermLog::debug()
{
}

void SystermLog::warrn()
{
}

void SystermLog::setFilePath(const char * logPath, const char * mode)
{
	if (m_file)
	{
		info("CELLLog::setLogPath _logFile != nullptr\n");
		fclose(m_file);
		m_file = nullptr;
	}


	m_file = fopen(logPath, mode);
	if (m_file)
	{
		info("CELLLog::setLogPath success,<%s,%s>\n", logPath, mode);
	}
	else 
	{
		info("CELLLog::setLogPath failed,<%s,%s>\n", logPath, mode);
	}
}

SystermLog * SystermLog::getInstance()
{
	static SystermLog log;
	return &log;
}
