#include "GraphicsEngine.h"
#include "CommandQueue.h"
#include "d3dx12.h"

GraphicsEngine::GraphicsEngine()
{
	m_useWarpDevice = false;
}

bool GraphicsEngine::init()
{
#if defined(_DEBUG)
	// Enable the D3D12 debug layer.
	{

		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory)));

	if (m_useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(m_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		));
	}
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(m_dxgi_factory, &hardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		));
	}


	//// Describe and create the command queue.
	//D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	//queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	//queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	//IDXGISwapChain* tempSwapChain;

	//DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
	//backBufferDesc.Width = width; // buffer width
	//backBufferDesc.Height = height; // buffer height
	//backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

	//													// describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
	//DXGI_SAMPLE_DESC sampleDesc = {};
	//sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)


	//// Describe and create the swap chain.
	//DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	//swapChainDesc.BufferCount = FrameCount;
	//swapChainDesc.BufferDesc.Width = width;
	//swapChainDesc.BufferDesc.Height = height;
	//swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//swapChainDesc.OutputWindow = hwnd;
	//swapChainDesc.SampleDesc.Count = 1;
	//swapChainDesc.Windowed = TRUE;


	//ThrowIfFailed(m_dxgi_factory->CreateSwapChain(m_commandQueue, &swapChainDesc, &tempSwapChain));

	//m_swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

	//m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();



	//// -- Create the Back Buffers (render target views) Descriptor Heap -- //

	//	// describe an rtv descriptor heap and create
	//D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	//rtvHeapDesc.NumDescriptors = FrameCount; // number of descriptors for this heap.
	//rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

	//// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
	//// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	//rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//
	//ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));


	//// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
	//// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
	//// device to give us the size. we will use this size to increment a descriptor handle offset
	//m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
	//// but we cannot literally use it like a c++ pointer.
	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

	//// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
	//for (int i = 0; i < FrameCount; i++)
	//{
	//	// first we get the n'th buffer in the swap chain and store it in the n'th
	//	// position of our ID3D12Resource array
	//	ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])));

	//	// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
	//	m_device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);

	//	// we increment the rtv handle by the rtv descriptor size we got above
	//	rtvHandle.Offset(1, m_rtvDescriptorSize);
	//}

	//// -- Create the Command Allocators -- //

	//for (int i = 0; i < FrameCount; i++)
	//{
	//	ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i])));
	//}

	//// -- Create a Command List -- //

	//   // create the command list with the first allocator
	//ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[0], NULL, IID_PPV_ARGS(&m_commandList)));

	//// command lists are created in the recording state. our main loop will set it up for recording again so close it now
	//m_commandList->Close();

	//// -- Create a Fence & Fence Event -- //

	//   // create the fences
	//for (int i = 0; i < FrameCount; i++)
	//{
	//	ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence[i])));
	//	m_fenceValue[i] = 0; // set the initial fence value to 0
	//}

	//// create a handle to a fence event
	//m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	//if (m_fenceEvent == nullptr)
	//{
	//	return false;
	//}

	return true;
}

bool GraphicsEngine::release()
{
	// wait for the gpu to finish all frames
	//for (int i = 0; i < FrameCount; ++i)
	//{
	//	m_frameIndex = i;
	//	waitForPreviousFrame();
	//}


	// close the fence event
	CloseHandle(m_fenceEvent);

	// get swapchain out of full screen before exiting
	//BOOL fs = false;
	//if (m_swapChain->GetFullscreenState(&fs, NULL))
	//	m_swapChain->SetFullscreenState(false, NULL);

	if (m_dxgi_factory)
	{
		m_dxgi_factory->Release();
	}

	if (m_device)
	{
		m_device->Release();
	}

	//swapchain->release();

	//commandqueue->release();

	SAFE_RELEASE(m_commandQueue);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_rtvHeap);
	SAFE_RELEASE(m_commandList);

	for (int i = 0; i < FrameCount; ++i)
	{
		SAFE_RELEASE(m_renderTargets[i]);
		SAFE_RELEASE(m_commandAllocator[i]);
		SAFE_RELEASE(m_fence[i]);
	};


	return true;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine graphicengine;

	return &graphicengine;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

CommandQueue* GraphicsEngine::createCommandQueue()
{
	return new CommandQueue();
}

void GraphicsEngine::setHwndInfo(HWND hwnd, UINT width, UINT height)
{
	this->hwnd = hwnd;
	this->height = height;
	this->width = width;
}

void GraphicsEngine::setApplicationWindow(class ApplicationWindow* wnd)
{
	window = wnd;
}

void GraphicsEngine::update()
{

}

void GraphicsEngine::updatePipeline()
{
	HRESULT hr;

	// We have to wait for the gpu to finish with the command allocator before we reset it
	waitForPreviousFrame();

	// we can only reset an allocator once the gpu is done with it
	// resetting an allocator frees the memory that the command list was stored in
	hr = m_commandAllocator[m_frameIndex]->Reset();
	if (FAILED(hr))
	{
	}

	// reset the command list. by resetting the command list we are putting it into
	// a recording state so we can start recording commands into the command allocator.
	// the command allocator that we reference here may have multiple command lists
	// associated with it, but only one can be recording at any time. Make sure
	// that any other command lists associated to this command allocator are in
	// the closed state (not recording).
	// Here you will pass an initial pipeline state object as the second parameter,
	// but in this tutorial we are only clearing the rtv, and do not actually need
	// anything but an initial default pipeline, which is what we get by setting
	// the second parameter to NULL
	hr = m_commandList->Reset(m_commandAllocator[m_frameIndex], NULL);
	if (FAILED(hr))
	{
	}

	// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

	// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);

	// set the render target for the output merger stage (the output of the pipeline)
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Clear the render target by using the ClearRenderTargetView command
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
	// warning if present is called on the render target when it's not in the present state
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	hr = m_commandList->Close();
	if (FAILED(hr))
	{
	}
}

void GraphicsEngine::waitForPreviousFrame()
{
	HRESULT hr;

	// swap the current rtv buffer index so we draw on the correct buffer
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
	// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
	if (m_fence[m_frameIndex]->GetCompletedValue() < m_fenceValue[m_frameIndex])
	{
		// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
		hr = m_fence[m_frameIndex]->SetEventOnCompletion(m_fenceValue[m_frameIndex], m_fenceEvent);
		if (FAILED(hr))
		{
		}

		// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
		// has reached "fenceValue", we know the command queue has finished executing
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	// increment fenceValue for next frame
	m_fenceValue[m_frameIndex]++;


	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. More advanced samples 
	// illustrate how to use fences for efficient resource usage.

	// Signal and increment the fence value.
	//const UINT64 fence = m_fenceValue;
	//ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
	//m_fenceValue++;

	//// Wait until the previous frame is finished.
	//if (m_fence->GetCompletedValue() < fence)
	//{
	//	ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
	//	WaitForSingleObject(m_fenceEvent, INFINITE);
	//}

	//m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void GraphicsEngine::render()
{
	HRESULT hr;

	updatePipeline(); // update the pipeline by sending commands to the commandqueue

	// create an array of command lists (only one command list here)
	ID3D12CommandList* ppCommandLists[] = { m_commandList };

	// execute the array of command lists
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// this command goes in at the end of our command queue. we will know when our command queue 
	// has finished because the fence value will be set to "fenceValue" from the GPU since the command
	// queue is being executed on the GPU
	hr = m_commandQueue->Signal(m_fence[m_frameIndex], m_fenceValue[m_frameIndex]);
	if (FAILED(hr))
	{
	}

	// present the current backbuffer
	hr = m_swapChain->Present(0, 0);
	if (FAILED(hr))
	{
	}
}

bool GraphicsEngine::getIsRunning()
{
	return m_running;
}

void GraphicsEngine::setIsRunning(bool isRunning)
{
	m_running = isRunning;
}

void GraphicsEngine::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;
		if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
		{
			// No more adapters to enumerate.
			break;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}
		pAdapter->Release();
	}
}


