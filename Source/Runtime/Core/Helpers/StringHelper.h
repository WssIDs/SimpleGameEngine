#pragma once

#include <Windows.h>
#include <string>



// Class Test for convert wide char to multi byte
struct String
{
	static FORCEINLINE std::string ConvertToMultiByte(const std::wstring& wtext)
	{
		// Convert a Unicode string to an ASCII string
		std::string strTo;
		char* szTo = new char[wtext.length() + 1];
		szTo[wtext.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wtext.c_str(), -1, szTo, (int)wtext.length(), nullptr, nullptr);
		strTo = szTo;
		delete[] szTo;
		return strTo;
	}

	static FORCEINLINE std::wstring ConvertToWideChar(const std::string& text)
	{
		// Convert an ASCII string to a Unicode String
		std::wstring wstrTo;
		wchar_t* wszTo = new wchar_t[text.length() + 1];
		wszTo[text.size()] = L'\0';
		MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, wszTo, (int)text.length());
		wstrTo = wszTo;
		delete[] wszTo;
		return wstrTo;
	}
};