#include "Graphics.h"
#include <iterator>
#include "Graphics/Helpers/DirectXHelper.h"
#include "Windows/Logger/LogDefinitions.h"
#include "Imgui/imgui_impl_dx11.h"
#include <ThirdParty\Imgui\imgui_impl_win32.h>

namespace wrl = Microsoft::WRL;

namespace dx = DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	S_LOG(TEXT("Graphics"), TEXT("Create"));

	InitDX11_1(hWnd);

	S_LOG(TEXT("Graphics"), TEXT("Init"));
	
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	S_LOG(TEXT("ImguiDX11"), TEXT("Init"));
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	S_LOG(TEXT("ImguiDX11"), TEXT("ShutDown"));

	// only for log
	S_LOG(TEXT("Graphics"), TEXT("Release"));
}

void Graphics::InitDX11(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 1024;
	sd.BufferDesc.Height = 768;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2; // 2 with DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // Windows 8+
	sd.Flags = 0;




	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;


	wrl::ComPtr<ID3D11Device> dev11;
	wrl::ComPtr<ID3D11DeviceContext> devcon11;
	wrl::ComPtr<IDXGISwapChain> devswap11;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		&FeatureLevelsRequested, 
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&devswap11,
		&dev11,
		&FeatureLevelsSupported,
		&devcon11
	);

	dev11.As(&pDevice);
	devcon11.As(&pContext);
	devswap11.As(&pSwap);

	wrl::ComPtr<ID3D11Resource> pBackBuffer;

	pSwap->GetBuffer(0, __uuidof(ID3D11Resource),	&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView);


	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1024u;
	descDepth.Height = 768u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthV = {};
	descDepthV.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthV.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDepthV, &pDepthStencilView);


	// bind depth stencil view to OM
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());


	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1024;
	vp.Height = 768;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp); // one viewport, not split screen
}

void Graphics::InitDX11_1(HWND hWnd)
{
	// Define temporary pointers to a device and a device context
	wrl::ComPtr<ID3D11Device> dev11;
	wrl::ComPtr<ID3D11DeviceContext> devcon11;

	// Create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11);

	// Convert the pointers from the DirectX 11 versions to the DirectX 11.1 versions
	dev11.As(&pDevice);
	devcon11.As(&pContext);

	// First, convert our ID3D11Device1 into an IDXGIDevice1
	wrl::ComPtr<IDXGIDevice1> dxgiDevice;
	pDevice.As(&dxgiDevice);

	// Second, use the IDXGIDevice1 interface to get access to the adapter
	wrl::ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	// Third, use the IDXGIAdapter interface to get access to the factory
	wrl::ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);


	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	SecureZeroMemory(&scd, sizeof(scd));
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
	scd.BufferCount = 2;                                  // a front buffer and a back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
	scfd.Scaling =  DXGI_MODE_SCALING_UNSPECIFIED;
	scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scfd.Windowed = TRUE;

	dxgiFactory->CreateSwapChainForHwnd(pDevice.Get(), hWnd, &scd, &scfd,nullptr, &pSwap);



	/// Back buffer
	wrl::ComPtr<ID3D11Texture2D> pBackBuffer;

	pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView);


	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1024u;
	descDepth.Height = 768u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthV = {};
	descDepthV.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthV.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDepthV, &pDepthStencilView);

	// bind depth stencil view to OM
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1024;
	vp.Height = 768;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp); // one viewport, not split screen
}

void Graphics::EndFrame()
{
	if(imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	pSwap->Present(1u, 0u);
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	if(imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}


	// bind depth stencil view to OM
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDepthStencilView.Get());

	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTargetView.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	pContext->DrawIndexed(count,0u, 0u);
}

void Graphics::EnableImgui()
{
	imguiEnabled = true;
}

void Graphics::DisableImgui()
{
	imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const
{
	return imguiEnabled;
}

void Graphics::SetProjection(DirectX::XMMATRIX projection)
{
	this->projection = projection;
}

DirectX::XMMATRIX Graphics::GetProjection() const
{
	return projection;
}

void Graphics::SetCamera(DirectX::XMMATRIX camera)
{
	this->camera = camera;
}

DirectX::XMMATRIX Graphics::GetCamera() const
{
	return camera;
}
