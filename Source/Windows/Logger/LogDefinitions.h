#pragma once
#include "Log.h"


/**
 * used only for open adn close logfile
 * @param LogText text
**/
#define S_LOG_MAIN(LogText) Log::get()->print(LogText)

/**
 * @param LogName name of the logging category
 * @param LogText text
**/
#define S_LOG(LogName,LogText) Log::get()->print(LogName,LogText)