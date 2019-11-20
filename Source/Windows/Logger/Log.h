#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "Windows\Launch\Window\STypes.h"

#define NULLTERMINATOR TEXT("\n")

//// NEED FOR INI FILE
#define MAXCOUNTFILE_BACKUPS 3
#define LOGPATH TEXT("Logs")

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
	
	void print(TSTRING logText);

	void print(TSTRING logName, TSTRING logText);

	static Log* get();
	
private:

	bool is_opened;
	
	std::filesystem::path logPath;
	TSTRING m_logfile;

	std::ofstream m_logger_out;

	TSTRING getCurrentTime() const;

	TSTRING getCurrentTimeByFormat(const TSTRING& format = TEXT("%d.%m.%Y %H.%M.%S")) const;
		std::vector<std::filesystem::path> GetFilesByMask(std::filesystem::path& directory,const TSTRING& filemask);
};

