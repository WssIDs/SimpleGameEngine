﻿#include "Log.h"
#include <iostream>
//#include <ctime>
#include <locale>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdarg.h>
#include <sstream>

namespace fs = std::filesystem;

Log::Log()
{

	auto t = std::time(nullptr);

	// Using localtime() 
	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	logPath = fs::path( LOGPATH );

	if(!fs::exists(logPath))
	{
		fs::create_directories(logPath);
	}

	logfile = "WGEngine.log";

	if(fs::exists(logPath/logfile))
	{
		std::stringstream wss;
		wss << "WGEngine-backup-" << std::put_time(&t_, "%Y.%m.%d-%H.%M.%S") << ".log";
		std::string backupfile = wss.str();

		fs::rename(logPath/logfile, logPath/backupfile);

		auto paths = GetFilesByMask(logPath, "WGEngine-backup");
		
		if (paths.size() > MAXCOUNTFILE_BACKUPS)
		{
			for (auto it = paths.begin(); it != paths.end() - MAXCOUNTFILE_BACKUPS; ++it)
			{
				fs::remove(*it);
			}
		}

	}

	logger_out.open(logPath/logfile, std::ios::out);

	//m_logger_out.imbue(locale(), new utf8cvt<false>);

	is_opened = logger_out.is_open();

	print("Log file open, ");
}

Log::~Log()
{
	if (is_opened)
	{
		print("Log file closed, ");
		logger_out.close();
		is_opened = false;
	}	
}

void Log::print(std::string logText, ...)
{
	if (is_opened)
	{
		va_list Args;
		va_start(Args, logText);
		std::string buffer;
		std::vsnprintf(&buffer[0], 1024, logText.c_str(), Args);

		std::stringstream output;
		output << buffer << getCurrentTimeByFormat();

		logger_out << output.str() << std::endl;

#ifdef _DEBUG
		PRINT_OUTPUT(output.str().c_str());
#endif
	}
	else
	{
#ifdef _DEBUG
		PRINT_OUTPUT(TEXT("Cannot Print Log"));
#endif
	}
}

void Log::print(std::string logName, std::string logText, ...)
{
	if (is_opened)
	{
		va_list Args;
		va_start(Args, logText);
		std::string buffer;
		std::vsnprintf(&buffer[0], 1024, logText.c_str(), Args);

		std::stringstream output;
		output << getCurrentTime() << logName << ": " << buffer.c_str();

		logger_out << output.str() << std::endl;

#ifdef _DEBUG
		PRINT_OUTPUT(output.str().c_str());
#endif
	}
	else
	{
#ifdef _DEBUG
		PRINT_OUTPUT(TEXT("Cannot Print Log"));
#endif
	}
}

Log* Log::get()
{
	static Log log;

	return &log;
}

std::string Log::getCurrentTime() const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	std::stringstream dateformat;
	dateformat << "["<< std::put_time(&t_, "%d.%m.%Y-%H.%M.%S") << "]";
	
	return dateformat.str();
}

// L"%d.%m.%Y %H.%M.%S")
std::string Log::getCurrentTimeByFormat(const std::string& format) const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	std::stringstream dateformat;
	dateformat << std::put_time(&t_, format.c_str());

	return dateformat.str();
}

std::vector<std::filesystem::path> Log::GetFilesByMask(std::filesystem::path& directory,const std::string& filemask)
{
	std::vector<fs::path> paths;

	for (const auto& entry : fs::directory_iterator(directory))
	{

#ifdef UNICODE
		std::wstring path = entry.path().wstring();
#else
		std::string path = entry.path().string();
#endif

		if (path.find(filemask) != std::string::npos)
		{
			paths.push_back(entry.path());
		}
	}

	return paths;
}
