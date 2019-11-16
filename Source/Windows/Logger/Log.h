#pragma once
#include <string>
#include <fstream>
#include <filesystem>

#define NULLTERMINATOR L"\n"

//// NEED FOR INI FILE
#define MAXCOUNTFILE_BACKUPS 3
#define LOGPATH L"Logs"

#ifdef _DEBUG
#  ifdef _MSC_VER
#    include <windows.h>
#    include <sstream>
#    define PRINT_OUTPUT(x) OutputDebugString(x); \
		OutputDebugString(NULLTERMINATOR)
#  else
#    include <iostream>
#    define PRINT_OUTPUT(x)  std::clog << (x)
#  endif        // or std::cerr << (x) << std::flush
#endif

class Log
{

public:

	Log();

	~Log();
	
	void print(std::wstring logText);

	void print(std::wstring logName, std::wstring logText);

	static Log* get();
	
private:

	bool is_opened;
	
	std::filesystem::path logPath;
	std::wstring m_logfile;

	std::wofstream m_logger_out;

	std::wstring getCurrentTime() const;

	std::wstring getCurrentTimeByFormat(const std::wstring& format = L"%d.%m.%Y %H.%M.%S") const;
		std::vector<std::filesystem::path> GetFilesByMask(std::filesystem::path& directory,const std::wstring& filemask);
};

