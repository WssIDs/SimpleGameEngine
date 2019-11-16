#include "SwapChain.h"
#include "GraphicsEngine.h"

bool SwapChain::init(ID3D12CommandQueue* commandqueue, HWND hwnd, UINT width, UINT height)
{

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;


	HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(commandqueue, &swapChainDesc, &m_swap_chain);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool SwapChain::release()
{
	if (m_swap_chain)
	{
		m_swap_chain->Release();
	}
	
	//delete this;
	return true;
}

IDXGISwapChain* SwapChain::getSwapChain()
{
	return m_swap_chain;
}
