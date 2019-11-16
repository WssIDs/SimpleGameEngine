#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "Graphics/Helpers/DirectXHelper.h"
#include "SwapChain.h"
#include "Windows/Launch/Window/ApplicationWindow.h"

using namespace Microsoft::WRL;
using namespace DX;

class CommandQueue;

class GraphicsEngine
{

public:

	GraphicsEngine();

	bool init();
	bool release();


	static GraphicsEngine* get();

public:
	SwapChain* createSwapChain();

	CommandQueue* createCommandQueue();

	void setHwndInfo(HWND hwnd, UINT width, UINT height);

	void setApplicationWindow(class ApplicationWindow* wnd);

	void update();

	void render();

	bool getIsRunning();

	void waitForPreviousFrame();

	void setIsRunning(bool isRunning);

private:
	
	ApplicationWindow* window = nullptr;

	void updatePipeline();

	static const UINT FrameCount = 3;

	// we will exit the program when this becomes false
	bool m_running = true;

	bool m_useWarpDevice;
	IDXGIFactory4* m_dxgi_factory;

	void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

	HWND hwnd;
	UINT width;
	UINT height;

	// Pipeline objects.
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;
	IDXGISwapChain3* m_swapChain;
	ID3D12Device* m_device;
	ID3D12Resource* m_renderTargets[FrameCount];
	ID3D12CommandAllocator* m_commandAllocator[FrameCount];
	ID3D12CommandQueue* m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ID3D12DescriptorHeap* m_rtvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ID3D12GraphicsCommandList* m_commandList;
	UINT m_rtvDescriptorSize;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ID3D12Fence* m_fence[FrameCount];
	UINT64 m_fenceValue[FrameCount];





	SwapChain* swapchain;
	CommandQueue* commandqueue;

private:
	friend class SwapChain;
	friend class CommandQueue;
};

