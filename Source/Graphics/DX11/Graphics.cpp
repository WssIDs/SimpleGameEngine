#include "Graphics.h"
#include <iterator>
#include "Graphics/Helpers/DirectXHelper.h"
#include "Windows/Logger/LogDefinitions.h"
#include "Imgui/imgui_impl_dx11.h"
#include <ThirdParty\Imgui\imgui_impl_win32.h>
#include "../Helpers/StringHelper.h"
#include "Math/WGMath.h"
#include <d2d1helper.h>
#include <d2d1.h>
#include <dwrite.h>
#include "../Loader/DDSTextureLoader.h"
#include <string>

DEFINE_LOG_CATEGORY(GraphicsLog);
DEFINE_LOG_CATEGORY(LogD3D11_1RHI);
DEFINE_LOG_CATEGORY(LogD2D_RHI);

namespace wrl = Microsoft::WRL;

namespace dx = DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	width(width),
	height(height)
{
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Create");

	InitDX11_1(hWnd);
	InitDX2D(hWnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "ImGui DX11 Init");
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "ImGui DX11 ShutDown");

	// only for log
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics Release");
}

void Graphics::InitDX11(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
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




	auto  FeatureLevelsRequested = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
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
	descDepth.Width = width;
	descDepth.Height = height;
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
	SetViewport(width, height);

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics DX11 Init");
}

void Graphics::InitDX11_1(HWND hWnd)
{
	auto  FeatureLevelsRequested = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	// Define temporary pointers to a device and a device context
	wrl::ComPtr<ID3D11Device> dev11;
	wrl::ComPtr<ID3D11DeviceContext> devcon11;

	// Create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&dev11,
		&FeatureLevelsSupported,
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
	DXGI_ADAPTER_DESC dad = { 0 };
	dxgiAdapter->GetDesc(&dad);


	PrintListAdapters(DirectVersionName::DirectX11_1, dxgiFactory, dad.DeviceId);

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	SecureZeroMemory(&scd, sizeof(scd));
	scd.Width = width;
	scd.Height = height;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
	scd.BufferCount = 2;                                  // a front buffer and a back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
	scfd.RefreshRate.Denominator = 1;
	scfd.RefreshRate.Numerator = 0;
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
	descDepth.Width = width;
	descDepth.Height = height;
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
	SetViewport(width, height);

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics DX11_1 Init");
}

void Graphics::InitDX2D(HWND hWnd)
{

	wrl::ComPtr<IDXGISurface2> pBackBuffer;

	pSwap->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory),&options, &pFactory2D);

	D2D1_RENDER_TARGET_PROPERTIES props =
	D2D1::RenderTargetProperties(
	D2D1_RENDER_TARGET_TYPE_DEFAULT,
	D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	pFactory2D->CreateDxgiSurfaceRenderTarget(pBackBuffer.Get(),props,&pRenderTarget2D);

	WGE_LOG(LogD2D_RHI, LogVerbosity::Default, "Graphics 2D Init");
}

void Graphics::Begin2DFrame()
{
	pRenderTarget2D->BeginDraw();

	//wrl::ComPtr<ID2D1SolidColorBrush> pBrush;

	//D2D_SIZE_F size = pRenderTarget2D->GetSize();

	//pRenderTarget2D->CreateSolidColorBrush(
	//	D2D1::ColorF(D2D1::ColorF::White),
	//	&pBrush
	//);

	//D2D1_ELLIPSE elipse = {};
	//elipse.point.x = 40;
	//elipse.point.y = 40;
	//elipse.radiusX = 30;
	//elipse.radiusY = 30;

	//pRenderTarget2D->DrawEllipse(elipse, pBrush.Get(), 10.0f);
}

void Graphics::End2DFrame()
{
	pRenderTarget2D->EndDraw();
}

void Graphics::ClearScreen(LinearColor color)
{
	pRenderTarget2D->Clear(D2D1::ColorF(color.R, color.G, color.B, color.A));
}

void Graphics::DrawCircle()
{
	//wrl::ComPtr<ID2D1RadialGradientBrush> radialBrush;
	//D2D1_GRADIENT_STOP stops[] =
	//{
	//	{0.0f, D2D1::ColorF(D2D1::ColorF::White,1) },
	//	{1.0f, D2D1::ColorF(D2D1::ColorF::Black,1) }
	//};

	//wrl::ComPtr<ID2D1GradientStopCollection> collection;
	//pRenderTarget2D->CreateGradientStopCollection(stops, 2, D2D1_GAMMA_2_2,
	//	D2D1_EXTEND_MODE_CLAMP, &collection);

	//D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES newprops = {};
	//pRenderTarget2D->CreateRadialGradientBrush(newprops, D2D1::BrushProperties(), collection.Get(), radialBrush.GetAddressOf());


	//D2D1_SIZE_F targetSize = pRenderTarget2D->GetSize();



	//radialBrush->SetTransform(
	//	D2D1::Matrix3x2F::Scale(targetSize)
	//);

	//D2D1_RECT_F rect = D2D1::RectF(
	//	0.0f,
	//	0.0f,
	//	targetSize.width,
	//	targetSize.height
	//);

	//pRenderTarget2D->FillRectangle(&rect, radialBrush.Get());


}

void Graphics::DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY,const std::wstring& fontName)
{
	// Create a DirectWrite factory.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(pDwriteFactory),
		&pDwriteFactory
	);

	wrl::ComPtr<IDWriteTextFormat> pTextFormat;

	pDwriteFactory->CreateTextFormat(
		fontName.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"", //locale
		&pTextFormat
	);

	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	wrl::ComPtr<ID2D1SolidColorBrush> pBrush;
	pRenderTarget2D->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(textColor.R,textColor.G, textColor.B, textColor.A)),
		&pBrush
	);
	
	D2D1_SIZE_F renderTargetSize = pRenderTarget2D->GetSize();

	wrl::ComPtr<IDWriteTextLayout> pTextLayout;
	pDwriteFactory->CreateTextLayout(text.c_str(), (UINT32)text.length(), pTextFormat.Get(), renderTargetSize.width, renderTargetSize.height, &pTextLayout);

	DWRITE_TEXT_METRICS tm = {};
	pTextLayout->GetMetrics(&tm);

	pRenderTarget2D->DrawTextLayout(D2D1::Point2F(screenX, screenY), pTextLayout.Get(), pBrush.Get());
}

void Graphics::SetViewport(int width, int height)
{
	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp); // one viewport, not split screen

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "SetViewport %dx%d", width, height );
}

void Graphics::EndFrame()
{
	if(imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	End2DFrame();

	pSwap->Present(0, 0u);
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	if(imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	Begin2DFrame();

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

void Graphics::PrintListAdapters(DirectVersionName dVersionName, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory, UINT deviceId)
{
	std::string versionName;

	switch (dVersionName)
	{
	case DirectVersionName::DirectX11:
		versionName = "D3D11";
		break;
	case DirectVersionName::DirectX11_1:
		versionName = "D3D11_1";
		break;
	case DirectVersionName::DirectX12:
		versionName = "D3D12";
		break;
	default:
		break;
	}

	WGE_LOG(LogD3D11_1RHI, LogVerbosity::Default, "%s Adapters", versionName.c_str());
	
	wrl::ComPtr<IDXGIAdapter> dxgiAdapter;
	//dxgiAdapter->CheckInterfaceSupport()
	int chosenAdapter = 0;
	for (UINT i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC dad = { 0 };
		dxgiAdapter->GetDesc(&dad);
		std::wstring wAdapterDesc = dad.Description;
		std::string adapterDesc = String::ConvertToMultiByte(wAdapterDesc);

		WGE_LOG(LogD3D11_1RHI, LogVerbosity::Default, "%d. '%s' (%s)", i, adapterDesc.c_str(), "Feature Level 11_0");

		int output = 0;
		if (dad.DeviceId == deviceId)
		{
			output = 1;
			chosenAdapter = i;
		}

		auto MiB = (long)WGMath::Pow(2, 20);

		auto delicatedVideoMemory = dad.DedicatedVideoMemory / MiB;
		auto delicatedSystemMemory = dad.DedicatedSystemMemory / MiB;
		auto sharedSystemMemory = dad.SharedSystemMemory / MiB;

		WGE_LOG(LogD3D11_1RHI, LogVerbosity::Default, "    %lu/%lu/%lu MB DedicatedVideo/DedicatedSystem/SharedSystem, Outputs:%d, VendorId:%04x(%d)",
			delicatedVideoMemory, delicatedSystemMemory, sharedSystemMemory, output, dad.VendorId, dad.VendorId);
	}

	WGE_LOG(LogD3D11_1RHI, LogVerbosity::Success, "Chosen %s Adapter: %d", versionName.c_str(), chosenAdapter);
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
