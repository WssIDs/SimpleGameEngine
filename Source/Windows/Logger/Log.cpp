#include "Log.h"
#include <iostream>
//#include <ctime>
#include <locale>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace fs = std::filesystem;

Log::Log()
{

	auto t = std::time(nullptr);

	// Using localtime() 
	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	logPath = fs::path(TEXT("..\\..\\..\\Saved")) / LOGPATH ;

	if(!fs::exists(logPath))
	{
		fs::create_directories(logPath);
	}

	logfile = TEXT("WGEngine.log");

	if(fs::exists(logPath/logfile))
	{
		TSTRINGSTREAM wss;
		wss << TEXT("WGEngine-backup-") << std::put_time(&t_, TEXT("%Y.%m.%d-%H.%M.%S")) << TEXT(".log");
		TSTRING backupfile = wss.str();

		fs::rename(logPath/logfile, logPath/backupfile);

		auto paths = GetFilesByMask(logPath, TEXT("WGEngine-backup"));
		
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

	print(TEXT("Log file open, "));
}

Log::~Log()
{
	if (is_opened)
	{
		print(TEXT("Log file closed, "));
		logger_out.close();
		is_opened = false;
	}	
}

void Log::print(TSTRING logText)
{
	if (is_opened)
	{
		TSTRINGSTREAM output;
		output << logText << getCurrentTimeByFormat();

		logger_out << output.str() << std::endl;

		PRINT_OUTPUT(output.str().c_str());
	}
	else
	{
		PRINT_OUTPUT(TEXT("Cannot Print Log"));
	}

}

void Log::print(TSTRING logName, TSTRING logText)
{
	if (is_opened)
	{
		TSTRINGSTREAM output;
		output << getCurrentTime() << logName << TEXT(": ") << logText;

		logger_out << output.str() << std::endl;


		PRINT_OUTPUT(output.str().c_str());
	}
	else
	{
		PRINT_OUTPUT(TEXT("Cannot Print Log"));
	}
}

Log* Log::get()
{
	static Log log;

	return &log;
}

TSTRING Log::getCurrentTime() const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	TSTRINGSTREAM dateformat;
	dateformat << TEXT("[")<< std::put_time(&t_, TEXT("%d.%m.%Y-%H.%M.%S")) << TEXT("]");
	
	return dateformat.str();
}

// L"%d.%m.%Y %H.%M.%S")
TSTRING Log::getCurrentTimeByFormat(const TSTRING& format) const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	TSTRINGSTREAM dateformat;
	dateformat << std::put_time(&t_, format.c_str());

	return dateformat.str();
}

std::vector<std::filesystem::path> Log::GetFilesByMask(std::filesystem::path& directory,const TSTRING& filemask)
{
	std::vector<fs::path> paths;

	for (const auto& entry : fs::directory_iterator(directory))
	{

#ifdef UNICODE
		std::wstring path = entry.path().wstring();
#else
		std::string path = entry.path().string();
#endif

		if (path.find(filemask) != TSTRING::npos)
		{
			paths.push_back(entry.path());
		}
	}

	return paths;
}
