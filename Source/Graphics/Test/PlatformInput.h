#pragma once
#include <string>
#include <Windows.h>
#include <assert.h>

struct FGenericPlatformInput
{
public:
	FORCEINLINE static unsigned int GetKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings)
	{
		return 0;
	}

	FORCEINLINE static unsigned int GetCharKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings)
	{
		return 0;
	}

protected:

	static unsigned int GetStandardPrintableKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings, bool bMapUppercaseKeys, bool bMapLowercaseKeys);
};


struct FWindowsPlatformInput : FGenericPlatformInput
{
	static unsigned int GetKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings);

	static unsigned int GetCharKeyMap(unsigned int* KeyCodes, std::string* KeyNames, unsigned int MaxMappings)
	{
		return FGenericPlatformInput::GetStandardPrintableKeyMap(KeyCodes, KeyNames, MaxMappings, true, false);
	}
};
