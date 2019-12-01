#pragma once
#include "Log.h"


/**
 * @param CategoryName name of the logging category
 * @param LogVerbosity color and level logging
 * @param Format some logging text with args
 *  example "SomeText %d %s", int number, std::string name.c_str()
**/
#define WGE_LOG_MAIN(Format,...) Log::get()->InternalMsg(Format, ##__VA_ARGS__)

/**
 * @param CategoryName name of the logging category
 * @param LogVerbosity color and level logging
 * @param Format some logging text with args
 *  example "SomeText %d %s", int number, std::string name.c_str() 
**/
#define WGE_LOG(CategoryName, LogVerbosity, Format,...) Log::get()->InternalMsg(CategoryName.GetCategoryName(), LogVerbosity, Format, ## __VA_ARGS__)

/**
 * @param CategoryName name of the logging category
 * @param LogVerbosity color and level logging
 * @param Format some logging text with args
 *  example "SomeText %d %s", int number, std::string name.c_str()
**/
#define WGE_LOG_WITHOUT_CONSOLE(CategoryName, LogVerbosity, Format,...) Log::get()->InternalMsgNoConsole(CategoryName.GetCategoryName(), LogVerbosity, Format, ## __VA_ARGS__)

/** Declare log category. Used only header files
 * @param CategoryName name of the logging category
 **/
#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName) \
		extern struct LogCategory##CategoryName : public LogCategory \
		{ \
			FORCEINLINE LogCategory##CategoryName() : LogCategory(TEXT(#CategoryName)) {} } CategoryName;

 /** Define log category. Used only source files
  * @param CategoryName name of the logging category
 **/
#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;