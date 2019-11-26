#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "Windows\Launch\Window\STypes.h"

#define NULLTERMINATOR TEXT("\n")

//// NEED FOR INI FILE
#define MAXCOUNTFILE_BACKUPS 3
#define LOGPATH "..\\..\\..\\Saved\\Logs"

#ifdef _DEBUG
#	ifdef _MSC_VER
#    include <windows.h>
#    include <sstream>
#    define PRINT_OUTPUT(x) OutputDebugString(x); \
		OutputDebugString(NULLTERMINATOR)
#	endif
#else
#    include <iostream>
#    define PRINT_OUTPUT(x)  std::clog << (x)
#endif

class Log
{

public:

	Log();

	~Log();
	
	void print(std::string logText, ...);

	void print(std::string logName, std::string logText, ...);

	static Log* get();
	
private:

	bool is_opened;
	
	std::filesystem::path logPath;
	std::string logfile;

	std::ofstream logger_out;

	std::string getCurrentTime() const;

	std::string getCurrentTimeByFormat(const std::string& format = "%d.%m.%Y %H.%M.%S") const;
		std::vector<std::filesystem::path> GetFilesByMask(std::filesystem::path& directory,const std::string& filemask);
};

