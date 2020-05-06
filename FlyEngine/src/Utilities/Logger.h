/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla 60001718
- End Header --------------------------------------------------------*/
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "../Managers/GameStateManager.h"
class Logger
{
public:
	static void Init();
	static void AddLogger(const char*);
	static void RemoveLogger(const char*);
	inline static std::shared_ptr<spdlog::logger> GetLogger(const char * _pLoggerName)
	{
		assert(mLogger.find(_pLoggerName) != mLogger.end());
		return mLogger[_pLoggerName];
	}
	//inline static std::shared_ptr<spdlog::logger>& GetLogger(const char* ) { return mLogger; }		
private:
	static std::unordered_map < std::string, std::shared_ptr<spdlog::logger> > mLogger;
};

#if _DEBUG						
#define SAFE_DELETE()								::GameStateManager::SafeDelete();
#define LOG_ADD(LogName)							::Logger::AddLogger(LogName);
#define LOG_REMOVE(LogName)							::Logger::RemoveLogger(LogName);
#define LOG_DEBUG(LogName,...)						::Logger::GetLogger(LogName)->debug(__VA_ARGS__)
#define LOG_INFO(LogName,...)						::Logger::GetLogger(LogName)->info(__VA_ARGS__)
#define LOG_WARN(LogName,...)						::Logger::GetLogger(LogName)->warn(__VA_ARGS__)
#define LOG_ERROR(LogName,...)						::Logger::GetLogger(LogName)->error(__VA_ARGS__)
#define LOG_CRITICAL(LogName,...)					::Logger::GetLogger(LogName)->critical(__VA_ARGS__)
#define ASSERT(LogName,Pointer,...)					if (Pointer == nullptr) { LOG_CRITICAL(LogName,__VA_ARGS__); SAFE_DELETE(); assert(Pointer); }
#else
#define SAFE_DELETE(Pointer) 
#define LOG_ADD(LogName)
#define LOG_REMOVE(LogName)
#define LOG_DEBUG(LogName,...)
#define LOG_INFO(LogName,...)
#define LOG_WARN(LogName,...)
#define LOG_ERROR(LogName,...)
#define LOG_CRITICAL(LogName,...)
#define ASSERT(LogName,Pointer,...)		
#endif