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

	logPath = fs::path(L"..\\..\\..\\Saved") / LOGPATH ;

	if(!fs::exists(logPath))
	{
		fs::create_directories(logPath);
	}

	m_logfile = L"WGEngine.log";

	if(fs::exists(logPath/m_logfile))
	{
		std::wstringstream wss;
		wss << L"WGEngine-backup-" << std::put_time(&t_, L"%Y.%m.%d-%H.%M.%S") << L".log";
		std::wstring backupfile = wss.str();

		fs::rename(logPath/m_logfile, logPath/backupfile);

		auto paths = GetFilesByMask(logPath, L"WGEngine-backup");
		
		if (paths.size() > MAXCOUNTFILE_BACKUPS)
		{
			for (auto it = paths.begin(); it != paths.end() - MAXCOUNTFILE_BACKUPS; ++it)
			{
				fs::remove(*it);
			}
		}

	}

	m_logger_out.open(logPath/m_logfile, std::ios::out);

	//m_logger_out.imbue(locale(), new utf8cvt<false>);

	is_opened = m_logger_out.is_open();

	print(L"Log file open, ");
}

Log::~Log()
{
	if (is_opened)
	{
		print(L"Log file closed, ");
		m_logger_out.close();
		is_opened = false;
	}	
}

void Log::print(std::wstring logText)
{
	if (is_opened)
	{
		std::wstringstream output;
		output << logText << getCurrentTimeByFormat();

		m_logger_out << output.str() << std::endl;

		PRINT_OUTPUT(output.str().c_str());
	}
	else
	{
		PRINT_OUTPUT(L"Cannot Print Log");
	}

}

void Log::print(std::wstring logName, std::wstring logText)
{
	if (is_opened)
	{
		std::wstringstream output;
		output << getCurrentTime() << logName << L": " << logText;

		m_logger_out << output.str() << std::endl;


		PRINT_OUTPUT(output.str().c_str());
	}
	else
	{
		PRINT_OUTPUT(L"Cannot Print Log");
	}
}

Log* Log::get()
{
	static Log log;

	return &log;
}

std::wstring Log::getCurrentTime() const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	std::wstringstream dateformat;
	dateformat << L"["<< std::put_time(&t_, L"%d.%m.%Y-%H.%M.%S") << L"]";
	
	return dateformat.str();
}

// L"%d.%m.%Y %H.%M.%S")
std::wstring Log::getCurrentTimeByFormat(const std::wstring& format) const
{
	auto t = std::time(nullptr);

	std::tm t_;
	localtime_s(std::addressof(t_), std::addressof(t));

	std::wstringstream dateformat;
	dateformat << std::put_time(&t_, format.c_str());

	return dateformat.str();
}

std::vector<std::filesystem::path> Log::GetFilesByMask(std::filesystem::path& directory,const std::wstring& filemask)
{
	std::vector<fs::path> paths;

	for (const auto& entry : fs::directory_iterator(directory))
	{
		std::wstring path = entry.path().c_str();

		if (path.find(filemask) != std::wstring::npos)
		{
			paths.push_back(entry.path());
		}
	}

	return paths;
}
