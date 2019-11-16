#include "GDIPlusManager.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>


ULONG_PTR GDIPlusManager::m_token;
int GDIPlusManager::m_refCount;

GDIPlusManager::GDIPlusManager()
{
	if (m_refCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&m_token, &input, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if (--m_refCount == 0)
	{
		Gdiplus::GdiplusShutdown(m_token);
	}
}
