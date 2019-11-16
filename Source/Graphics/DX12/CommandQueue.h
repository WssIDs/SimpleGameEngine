#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include "Graphics/Helpers/DirectXHelper.h"

class CommandQueue
{

public:

	bool init();
	bool release();

	ID3D12CommandQueue* getCommandQueue();

private:
	ID3D12CommandQueue* m_commandQueue;
};

