#pragma once
#include <string>
#include <fstream>
#include <filesystem>

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

enum class LogVerbosity : int
{
	Default = 7,
	Warning = 6,
	Success = 2,
	Error = 12,
	Fatal = 4
};


class Log
{

public:

	Log();

	~Log();
	
	void print(std::string logText, ...);

	void print(std::string logName, LogVerbosity logVerbosity, std::string logText, ...);

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

struct LogCategoryBase
{
	LogCategoryBase(const std::string& CategoryName)
		:
		CategoryName(CategoryName)
	{
	}

	FORCEINLINE std::string GetCategoryName() const
	{
		return CategoryName;
	}

protected:
	std::string CategoryName;
};


struct LogCategory : public LogCategoryBase
{
	FORCEINLINE LogCategory(const std::string& CategoryName)
		: LogCategoryBase(CategoryName)
	{
	}
};