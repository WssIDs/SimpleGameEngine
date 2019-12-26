#include "Log.h"
#include <iostream>
//#include <ctime>
#include <locale>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdarg.h>
#include <sstream>
#include <memory>

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

	InternalMsg("Log file open, ");
}

Log::~Log()
{
	if (is_opened)
	{
		InternalMsg("Log file closed, ");
		logger_out.close();
		is_opened = false;
	}	
}

void Log::InternalMsg(std::string format, ...)
{
	int final_n, n = ((int)format.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (true) {
		formatted = std::make_unique<char[]>(n); /* Wrap the plain char array into the unique_ptr */
		strcpy_s(&formatted[0], n, format.c_str());
		va_start(ap, format);
		final_n = vsnprintf(&formatted[0], n, format.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	std::stringstream output;
	output << formatted.get() << getCurrentTimeByFormat("%d.%m.%Y %H.%M.%S");

	if (is_opened)
	{
		logger_out << output.str() << std::endl;
	}

	std::cout << output.str() << std::endl;

#ifdef _DEBUG
	PRINT_OUTPUT(output.str().c_str());
#endif // _DEBUG
}

void Log::InternalMsg(std::string logCategory, LogVerbosity logVerbosity,const std::string format, ...)
{
	int final_n, n = ((int)format.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (true) {
		formatted = std::make_unique<char[]>(n); /* Wrap the plain char array into the unique_ptr */
		strcpy_s(&formatted[0], n, format.c_str());
		va_start(ap, format);
		final_n = vsnprintf(&formatted[0], n, format.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	std::stringstream output;
	output << getCurrentTime() << " " << logCategory << ": ";

	switch (logVerbosity)
	{
	case LogVerbosity::Default:
		break;
	case LogVerbosity::Warning:
		output << "Warning: ";
		break;
	case LogVerbosity::Success:
		output << "Success: ";
		break;
	case LogVerbosity::Error:
		output << "Error: ";
		break;
	case LogVerbosity::Fatal:
		output << "Fatal: ";
		break;
	}

	output << formatted.get();

	if (is_opened)
	{
		logger_out << output.str() << std::endl;
	}

	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole)
	{
		SetConsoleTextAttribute(hConsole, (int)logVerbosity);
		std::cout << output.str() << std::endl;
	}

#ifdef _DEBUG
	PRINT_OUTPUT(output.str().c_str());
#endif // _DEBUG
}

void Log::InternalMsgNoConsole(std::string logCategory, LogVerbosity logVerbosity, const std::string format, ...)
{
	int final_n, n = ((int)format.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (true) {
		formatted = std::make_unique<char[]>(n); /* Wrap the plain char array into the unique_ptr */
		strcpy_s(&formatted[0], n, format.c_str());
		va_start(ap, format);
		final_n = vsnprintf(&formatted[0], n, format.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	std::stringstream output;
	output << getCurrentTime() << " " << logCategory << ": ";

	switch (logVerbosity)
	{
	case LogVerbosity::Default:
		break;
	case LogVerbosity::Warning:
		output << "Warning: ";
		break;
	case LogVerbosity::Success:
		output << "Success: ";
		break;
	case LogVerbosity::Error:
		output << "Error: ";
		break;
	case LogVerbosity::Fatal:
		output << "Fatal: ";
		break;
	}

	output << formatted.get();

	if (is_opened)
	{
		logger_out << output.str() << std::endl;
	}

//#ifdef _DEBUG
//	PRINT_OUTPUT(output.str().c_str());
//#endif // _DEBUG
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