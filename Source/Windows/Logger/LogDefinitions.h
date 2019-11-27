#pragma once
#include "Log.h"


/**
 * used only for open adn close logfile
 * @param LogText text
**/
#define S_LOG_MAIN(Format,...) Log::get()->print(Format, ##__VA_ARGS__)

/**
 * @param LogName name of the logging category
 * @param LogText text
**/
//#define S_LOG(LogName,LogText) Log::get()->print(LogName,LogText)



#define S_LOG(LogName, LogVerbosity, Format,...) Log::get()->print(LogName, LogVerbosity, Format, ##__VA_ARGS__)