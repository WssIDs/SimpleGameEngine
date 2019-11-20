#pragma once
#include <string>


//
// Neutral ANSI/UNICODE types and macros
//
#ifdef  UNICODE
// std::wstring 
typedef std::wstring TSTRING;
typedef std::wstringstream TSTRINGSTREAM;
#else
// std::string
typedef std::string TSTRING;
typedef std::stringstream TSTRINGSTREAM;
#endif