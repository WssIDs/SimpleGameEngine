#pragma once
#include <windows.h>

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();

private:
	static ULONG_PTR m_token;
	static int m_refCount;
};

