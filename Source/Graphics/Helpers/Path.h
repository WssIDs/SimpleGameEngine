#pragma once

#ifndef WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif
#include <string>
#include <filesystem>

struct Path
{
	static FORCEINLINE std::string GetFileNameWithExtension(const char* filePath)
	{
		std::filesystem::path calculatedPath(filePath);	 
		return calculatedPath.filename().string();
	}

	static FORCEINLINE std::string GetFileNameWithExtension(const std::string& filePath)
	{
		std::filesystem::path calculatedPath(filePath);
		return calculatedPath.filename().string();
	}

	static FORCEINLINE std::string GetFileNameWithoutExtension(const std::string& filePath)
	{
		std::filesystem::path calculatedPath(filePath);
		return calculatedPath.stem().string();
	}
};