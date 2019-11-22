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
	bool running = true;

	bool useWarpDevice;
	IDXGIFactory4* dxgi_factory;

	void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);

	HWND hwnd;
	UINT width;
	UINT height;

	// Pipeline objects.
	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;
	IDXGISwapChain3* swapChain;
	ID3D12Device* device;
	ID3D12Resource* renderTargets[FrameCount];
	ID3D12CommandAllocator* commandAllocator[FrameCount];
	ID3D12CommandQueue* commandQueue;
	ComPtr<ID3D12RootSignature> rootSignature;
	ID3D12DescriptorHeap* rtvHeap;
	ComPtr<ID3D12PipelineState> pipelineState;
	ID3D12GraphicsCommandList* commandList;
	UINT rtvDescriptorSize;

	// Synchronization objects.
	UINT frameIndex;
	HANDLE fenceEvent;
	ID3D12Fence* fence[FrameCount];
	UINT64 fenceValue[FrameCount];





	SwapChain* swapchain;
	CommandQueue* commandqueue;

private:
	friend class SwapChain;
	friend class CommandQueue;
};

