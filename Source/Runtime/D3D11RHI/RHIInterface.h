#pragma once
#include <Runtime/D3D11RHI/DX11/DirectXIncludes.h>
#include <Runtime/Core/Timer/Timer.h>

class RHIInterface
{
public:
	
	virtual void Init(HWND hWnd, int width, int height) = 0;
};