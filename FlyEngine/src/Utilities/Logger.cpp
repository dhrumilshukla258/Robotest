/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Dhrumil Shukla, dhrumil.shukla 60001718
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "Logger.h"
#include "../Managers/GameStateManager.h"
#include <time.h>

extern GameStateManager* gpGameStateManager;

std::unordered_map< std::string, std::shared_ptr<spdlog::logger> > Logger::mLogger;
	
void Logger::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	// If want to add log in file use logger FlyEngine
	// Else name any logger you want and create it by AddLogger
	// And use it in the terminal output as a way to keep track of 
	// Error warnings and what not.
	/*
	time_t t = time(NULL);
	struct tm buf;
	localtime_s(&buf, &t);

	std::string mon = std::to_string(buf.tm_mon + 1);
	std::string day = std::to_string(buf.tm_mday);
	std::string hour = std::to_string(buf.tm_hour);
	std::string min = std::to_string(buf.tm_min);
	std::string sec = std::to_string(buf.tm_sec);
	std::string totalTime = mon + "-" + day + "__" + hour + " " + min + " " + sec;

	std::string fileName = "Logs/FLog__" + totalTime;
	fileName += ".txt";
	mLogger["FlyEngine"] = spdlog::basic_logger_mt("FlyEngine", fileName);
	*/

	mLogger["FlyEngine"] = spdlog::daily_logger_mt("FlyEngine", "Flog", 14, 55);
	mLogger["FlyEngine"]->set_level(spdlog::level::trace);
}
	
void Logger::AddLogger(const char * _pLoggerName)
{
	assert(mLogger.find(_pLoggerName) == mLogger.end());
	mLogger[_pLoggerName] = spdlog::stdout_color_mt(_pLoggerName);
}

void Logger::RemoveLogger(const char * _pLoggerName)
{
	assert(mLogger.find(_pLoggerName) != mLogger.end());
	mLogger.erase(_pLoggerName);
}