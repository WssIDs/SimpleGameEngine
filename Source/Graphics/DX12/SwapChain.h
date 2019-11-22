#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include "CommandQueue.h"

class SwapChain
{

public:

	bool init(ID3D12CommandQueue* commandqueue, HWND hwnd, UINT width, UINT height);
	bool release();

	IDXGISwapChain* getSwapChain();

private:
	IDXGISwapChain* swap_chain;
};

