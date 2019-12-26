#include "Graphics.h"
#include "..\..\Core\Helpers\StringHelper.h"
#include "..\..\Core\Math\WGMath.h"

DEFINE_LOG_CATEGORY(GraphicsLog);
DEFINE_LOG_CATEGORY(LogD3D11_1RHI);
DEFINE_LOG_CATEGORY(LogD2D_RHI);

namespace wrl = Microsoft::WRL;

namespace dx = DirectX;

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "ImGui DX11 ShutDown");

	// only for log
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics Release");
}

void Graphics::Init(HWND hWnd, int width, int height)
{
	this->hWnd = hWnd;
	this->width = width;
	this->height = height;

	timer = std::make_shared<Timer>();

	desiredColourFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Create");

	InitDX11_1();
	InitDX2D();
	ImGui_ImplDX11_Init(pDevice3D.Get(), pDeviceContext3D.Get());
	WGE_LOG(GraphicsLog, LogVerbosity::Default, "ImGui DX11 Init");
}

void Graphics::InitDX11()
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

	dev11.As(&pDevice3D);
	devcon11.As(&pDeviceContext3D);
	devswap11.As(&pSwap);

	wrl::ComPtr<ID3D11Resource> pBackBuffer;

	pSwap->GetBuffer(0, __uuidof(ID3D11Resource),	&pBackBuffer);
	pDevice3D->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView3D);


	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice3D->CreateDepthStencilState(&dsDesc, &pDSState);

	// bind depth state
	pDeviceContext3D->OMSetDepthStencilState(pDSState.Get(), 1u);

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

	pDevice3D->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthV = {};
	descDepthV.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthV.Texture2D.MipSlice = 0u;

	pDevice3D->CreateDepthStencilView(pDepthStencil.Get(), &descDepthV, &pDepthStencilView3D);


	// bind depth stencil view to OM
	pDeviceContext3D->OMSetRenderTargets(1u, pRenderTargetView3D.GetAddressOf(), pDepthStencilView3D.Get());


	// configure viewport
	SetViewport(width, height);

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics DX11 Init");
}

void Graphics::InitDX11_1()
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
	dev11.As(&pDevice3D);
	devcon11.As(&pDeviceContext3D);

	// First, convert our ID3D11Device1 into an IDXGIDevice1
	wrl::ComPtr<IDXGIDevice1> dxgiDevice;
	pDevice3D.As(&dxgiDevice);

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
	scd.BufferCount = 3;                                  // a front buffer and a back buffer
	scd.Format = desiredColourFormat;					  // the color palette to use								
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	  // allow mode switching
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		  // the recommended flip mode
	scd.SampleDesc.Count = 1;                             // disable anti-aliasing
	scd.SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {};
	scfd.RefreshRate.Denominator = 1;
	scfd.RefreshRate.Numerator = 0;
	scfd.Scaling =  DXGI_MODE_SCALING_UNSPECIFIED;
	scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scfd.Windowed = TRUE;

	dxgiFactory->CreateSwapChainForHwnd(pDevice3D.Get(), hWnd, &scd, &scfd,nullptr, &pSwap);
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	// enumerate all available display modes
	// get representation of the output adapter
	wrl::ComPtr<IDXGIOutput> output;
	pSwap->GetContainingOutput(&output);

	// get the amount of supported display modes for the desired format
	output->GetDisplayModeList(desiredColourFormat, 0, &numberOfSupportedModes, nullptr);

	// set up array for the supported modes
	supportedModes = new DXGI_MODE_DESC[numberOfSupportedModes];
	ZeroMemory(supportedModes, sizeof(DXGI_MODE_DESC) * numberOfSupportedModes);

	// fill the array with the available display modes
	output->GetDisplayModeList(desiredColourFormat, 0, &numberOfSupportedModes, supportedModes);

	// if the current resolution is not supported, switch to the lowest supported resolution
	bool supportedMode = false;
	for (unsigned int i = 0; i < numberOfSupportedModes; i++)
	{
		UINT sWidth = supportedModes[i].Width;
		UINT sHeight = supportedModes[i].Height;

		WGE_LOG(GraphicsLog, LogVerbosity::Warning, "SupportedModes: width = %d, height = %d", sWidth, sHeight);

		if ((unsigned int)width == sWidth && (unsigned int)height == sHeight)
		{
			supportedMode = true;
			currentModeDescription = supportedModes[i];
			currentModeIndex = i;
			break;
		}
	}

	if (!supportedMode)
	{
		// print a warning 
		WGE_LOG(LogD3D11_1RHI,LogVerbosity::Warning, "The desired screen resolution is not supported! Resizing...");

		// set the mode to the lowest supported resolution
		currentModeDescription = supportedModes[0];
		currentModeIndex = 0;
		if (FAILED(pSwap->ResizeTarget(&currentModeDescription)))
			throw std::runtime_error("Unable to resize target to a supported display mode!");

		// write the current mode to the configuration file
		//if (!writeCurrentModeDescriptionToConfigurationFile().wasSuccessful())
		//	return std::runtime_error("Unable to write to the configuration file!");
	}

	// set full screen mode?
	if (startInFullscreen)
	{
		// switch to full screen mode
		if (FAILED(pSwap->SetFullscreenState(true, nullptr)))
			throw std::runtime_error("Unable to switch to full screen mode!");
		currentlyInFullscreen = true;
	}
	else
		currentlyInFullscreen = false;

	// the remaining steps need to be done each time the window is resized
	if (!OnResize())
		throw std::runtime_error("Direct3D was unable to resize its resources!");

	/// Back buffer
	//wrl::ComPtr<ID3D11Texture2D> pBackBuffer;

	//pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
	//pDevice3D->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView3D);


	//// create depth stencil state
	//D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	//dsDesc.DepthEnable = true;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	//pDevice3D->CreateDepthStencilState(&dsDesc, &pDSState);

	//// bind depth state
	//pDeviceContext3D->OMSetDepthStencilState(pDSState.Get(), 1u);

	//// create depth stencil texture
	//wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	//D3D11_TEXTURE2D_DESC descDepth = {};
	//descDepth.Width = width;
	//descDepth.Height = height;
	//descDepth.MipLevels = 1u;
	//descDepth.ArraySize = 1u;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	//descDepth.SampleDesc.Count = 1u;
	//descDepth.SampleDesc.Quality = 0u;
	//descDepth.Usage = D3D11_USAGE_DEFAULT;
	//descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	//pDevice3D->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	//// create view of depth stencil texture
	//D3D11_DEPTH_STENCIL_VIEW_DESC descDepthV = {};
	//descDepthV.Format = DXGI_FORMAT_D32_FLOAT;
	//descDepthV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDepthV.Texture2D.MipSlice = 0u;

	//pDevice3D->CreateDepthStencilView(pDepthStencil.Get(), &descDepthV, &pDepthStencilView3D);

	//// bind depth stencil view to OM
	//pDeviceContext3D->OMSetRenderTargets(1u, pRenderTargetView3D.GetAddressOf(), pDepthStencilView3D.Get());

	//// configure viewport
	//SetViewport(width, height);

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "Graphics DX11_1 Init");
}

void Graphics::changeResolution(bool increase)
{
	if (increase)
	{
		// if increase is true, choose a higher resolution, if possible
		if (currentModeIndex < numberOfSupportedModes - 1)
		{
			currentModeIndex++;
			changeMode = true;
		}
		else
			changeMode = false;
	}
	else
	{
		// else choose a smaller resolution, but only if possible
		if (currentModeIndex > 0)
		{
			currentModeIndex--;
			changeMode = true;
		}
		else
			changeMode = false;
	}

	if (changeMode)
	{
		// change mode
		currentModeDescription = supportedModes[currentModeIndex];

		// resize everything
		OnResize();
	}
}

void Graphics::OnBordlessMaximize()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	DXGI_MODE_DESC zeroRefreshRate = currentModeDescription;
	zeroRefreshRate.RefreshRate.Numerator = 0;
	zeroRefreshRate.RefreshRate.Denominator = 0;
	zeroRefreshRate.Width = desktop.right;
	zeroRefreshRate.Height = desktop.bottom;

	pSwap->ResizeTarget(&zeroRefreshRate);

	// release and reset all resources
	if (pDeviceContext2D != nullptr)
	{
		pDeviceContext2D->SetTarget(nullptr);
	}

	pDeviceContext3D->ClearState();
	pRenderTargetView3D = nullptr;
	pDepthStencilView3D = nullptr;

	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	pSwap->GetDesc1(&scd);
	// resize the swap chain
	if (FAILED(pSwap->ResizeBuffers(scd.BufferCount, 0, 0, scd.Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to resize the swap chain!");
	}

	// (re)-create the render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	if (FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to acquire the back buffer!");
	}

	if (FAILED(pDevice3D->CreateRenderTargetView(backBuffer.Get(), nullptr, &pRenderTargetView3D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create the render target view!");
	}


	// create the depth and stencil buffer
	D3D11_TEXTURE2D_DESC dsd;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dsBuffer;
	backBuffer->GetDesc(&dsd);
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(pDevice3D->CreateTexture2D(&dsd, nullptr, dsBuffer.GetAddressOf())))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create a 2D-texture!");
	}

	if (FAILED(pDevice3D->CreateDepthStencilView(dsBuffer.Get(), nullptr, pDepthStencilView3D.GetAddressOf())))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create the depth and stencil buffer!");
	}

	// activate the depth and stencil buffer
	pDeviceContext3D->OMSetRenderTargets(1, pRenderTargetView3D.GetAddressOf(), pDepthStencilView3D.Get());

	SetViewport(dsd.Width, dsd.Height);

	// specify the desired bitmap properties
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	wrl::ComPtr<IDXGISurface> dxgiBuffer;
	if (FAILED(pSwap->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to retrieve the back buffer!");
		throw "Critical error: Unable to retrieve the back buffer!";
	}

	// create the bitmap
	if (pDeviceContext2D != nullptr)
	{
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
		if (FAILED(pDeviceContext2D->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, &targetBitmap)))
		{
			WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the Direct2D bitmap from the DXGI surface!");
			throw "Critical error: Unable to create the Direct2D bitmap from the DXGI surface!";
		}

		pDeviceContext2D->SetTarget(targetBitmap.Get());
	}

	ImGui::GetIO().DisplaySize.x = (float)currentModeDescription.Width;
	ImGui::GetIO().DisplaySize.y = (float)currentModeDescription.Height;
}

bool Graphics::OnResize()
{
	// Microsoft recommends zeroing out the refresh rate of the description before resizing the targets
	DXGI_MODE_DESC zeroRefreshRate = currentModeDescription;
	zeroRefreshRate.RefreshRate.Numerator = 0;
	zeroRefreshRate.RefreshRate.Denominator = 0;

	// check for full screen switch
	BOOL inFullscreen = false;
	pSwap->GetFullscreenState(&inFullscreen, nullptr);

	if (currentlyInFullscreen != (bool)inFullscreen)
	{
		// full screen switch
		if (inFullscreen)
		{
			// switched to full screen -> Microsoft recommends resizing the target before going into fullscreen
			if (FAILED(pSwap->ResizeTarget(&zeroRefreshRate)))
			{
				WGE_LOG(GraphicsLog, LogVerbosity::Error, "Unable to resize target!");
				return false;
			}
			// set full screen state
			if (FAILED(pSwap->SetFullscreenState(true, nullptr)))
			{
				WGE_LOG(GraphicsLog, LogVerbosity::Error, "Unable to switch to full screen mode!");
				return false;
			}
		}
		else
		{
			// switched to windowed -> simply set fullscreen mode to false
			if (FAILED(pSwap->SetFullscreenState(false, nullptr)))
			{
				WGE_LOG(GraphicsLog, LogVerbosity::Error, "Unable to switch to windowed mode mode!");
				return false;
			}

			 //recompute client area and set new window size
			RECT rect = { 0, 0, (long)currentModeDescription.Width,  (long)currentModeDescription.Height };
			if (AdjustWindowRectEx(&rect, WS_EX_OVERLAPPEDWINDOW, false, WS_OVERLAPPEDWINDOW))
			{
				WGE_LOG(GraphicsLog, LogVerbosity::Error, "Failed to adjust window rectangle!");
				return false;
			}

			SetWindowPos(hWnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
		}

		// change full screen mode
		currentlyInFullscreen = !currentlyInFullscreen;
	}

	// resize target to the desired resolution
	if (FAILED(pSwap->ResizeTarget(&zeroRefreshRate)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Unable to resize target!");
		return false;
	}

	// release and reset all resources
	if (pDeviceContext2D != nullptr)
	{
		pDeviceContext2D->SetTarget(nullptr);
	}

	pDeviceContext3D->ClearState();
	pRenderTargetView3D = nullptr;
	pDepthStencilView3D = nullptr;

	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	pSwap->GetDesc1(&scd);
	// resize the swap chain
	if (FAILED(pSwap->ResizeBuffers(scd.BufferCount, 0, 0, scd.Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to resize the swap chain!");
		return false;
	}

	// (re)-create the render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	if (FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to acquire the back buffer!");
		return false;
	}

	if (FAILED(pDevice3D->CreateRenderTargetView(backBuffer.Get(), nullptr, &pRenderTargetView3D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create the render target view!");
		return false;
	}


	// create the depth and stencil buffer
	D3D11_TEXTURE2D_DESC dsd;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> dsBuffer;
	backBuffer->GetDesc(&dsd);
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(pDevice3D->CreateTexture2D(&dsd, nullptr, dsBuffer.GetAddressOf())))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create a 2D-texture!");
		return false;
	}

	if (FAILED(pDevice3D->CreateDepthStencilView(dsBuffer.Get(), nullptr, pDepthStencilView3D.GetAddressOf())))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Error, "Direct3D was unable to create the depth and stencil buffer!");
		return false;
	}

	// activate the depth and stencil buffer
	pDeviceContext3D->OMSetRenderTargets(1, pRenderTargetView3D.GetAddressOf(), pDepthStencilView3D.Get());

	SetViewport(dsd.Width, dsd.Height);

	// specify the desired bitmap properties
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	wrl::ComPtr<IDXGISurface> dxgiBuffer;
	if (FAILED(pSwap->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to retrieve the back buffer!");
		throw "Critical error: Unable to retrieve the back buffer!";
	}

	// create the bitmap
	if (pDeviceContext2D != nullptr)
	{
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
		if (FAILED(pDeviceContext2D->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, &targetBitmap)))
		{
			WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the Direct2D bitmap from the DXGI surface!");
			throw "Critical error: Unable to create the Direct2D bitmap from the DXGI surface!";
		}

		pDeviceContext2D->SetTarget(targetBitmap.Get());
	}

	ImGui::GetIO().DisplaySize.x = (float)currentModeDescription.Width;
	ImGui::GetIO().DisplaySize.y = (float)currentModeDescription.Height;

	return true;
}

bool Graphics::GetFullScreenState() const
{
	BOOL fullscreen;

	pSwap->GetFullscreenState(&fullscreen, nullptr);

	return (bool)fullscreen;
}

bool Graphics::IsCurrentInFullScreen() const
{
	return currentlyInFullscreen;
}

void Graphics::InitDX2D()
{
	if(FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &pDwriteFactory2D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the DirectWrite factory!");
		throw "Critical error: Unable to create the DirectWrite factory!";
	}

	D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory3), &options, &pFactory2D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create Direct2D Factory!");
		throw "Critical error: Unable to create Direct2D Factory!";
	}

	// get the dxgi device
	wrl::ComPtr<IDXGIDevice> dxgiDevice;
	if (FAILED(pDevice3D.Get()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to get the DXGI device!");
		throw "Critical error: Unable to get the DXGI device!";
	}

	// create the Direct2D device
	if (FAILED(pFactory2D->CreateDevice(dxgiDevice.Get(), &pDevice2D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the Direct2D device!");
		throw "Critical error: Unable to create the Direct2D device!";
	}

	// create its context
	if (FAILED(pDevice2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &pDeviceContext2D)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the Direct2D device context!");
		throw "Critical error: Unable to create the Direct2D device context!";
	}

	// specify the desired bitmap properties
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	wrl::ComPtr<IDXGISurface> dxgiBuffer;
	if (FAILED(pSwap->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to retrieve the back buffer!");
		throw "Critical error: Unable to retrieve the back buffer!";
	}

	// create the bitmap
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
	if (FAILED(pDeviceContext2D->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, &targetBitmap)))
	{
		WGE_LOG(GraphicsLog, LogVerbosity::Fatal, "Unable to create the Direct2D bitmap from the DXGI surface!");
		throw "Critical error: Unable to create the Direct2D bitmap from the DXGI surface!";
	}

	pDeviceContext2D->SetTarget(targetBitmap.Get());

	WGE_LOG(LogD2D_RHI, LogVerbosity::Default, "Graphics 2D Init");
}

void Graphics::Begin2DFrame()
{
	pDeviceContext2D->BeginDraw();

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

	if (pDeviceContext2D != nullptr)
	{
		pDefaultBrush = CreateSolidColorBrush(LinearColor(0.0f,0.0f,0.0f,1.0f));
	}
}

void Graphics::End2DFrame()
{
	pDeviceContext2D->EndDraw();
}

void Graphics::ClearScreen(LinearColor color)
{
	pDeviceContext2D->Clear(D2D1::ColorF(color.R, color.G, color.B, color.A));
}

void Graphics::DrawCircle()
{
}

void Graphics::DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY,const std::wstring& fontName)
{

	wrl::ComPtr<IDWriteTextFormat> pTextFormat;

	pDwriteFactory2D->CreateTextFormat(
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

	SetColor(textColor);
	
	D2D1_SIZE_F renderTargetSize = pDeviceContext2D->GetSize();

	wrl::ComPtr<IDWriteTextLayout> pTextLayout;
	pDwriteFactory2D->CreateTextLayout(text.c_str(), (UINT32)text.length(), pTextFormat.Get(), renderTargetSize.width, renderTargetSize.height, &pTextLayout);

	DWRITE_TEXT_METRICS tm = {};
	pTextLayout->GetMetrics(&tm);

	pDeviceContext2D->DrawTextLayout(D2D1::Point2F(screenX, screenY), pTextLayout.Get(), GetDefaultBrush().Get());
}

void Graphics::DrawRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color, const float strokeWidth /*= 1.0f*/)
{
	D2D1_RECT_F rect = D2D1::RectF(
		screenX,
		screenY,
		screenW,
		screenH
	);

	SetColor(color);
	pDeviceContext2D->DrawRectangle(rect, GetDefaultBrush().Get(), strokeWidth);
}

void Graphics::DrawFillRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color)
{
	D2D1_RECT_F rect = D2D1::RectF(
		screenX,
		screenY,
		screenW,
		screenH
	);

	SetColor(color);
	pDeviceContext2D->FillRectangle(rect, GetDefaultBrush().Get());
}

void Graphics::DrawEllipse(const float screenX, const float screenY, const float radiusX, const float radiusY, const LinearColor& color, const float strokeWidth)
{
	D2D1_POINT_2F centre = {screenX, screenY};
	D2D1_ELLIPSE ellipse = {centre, radiusX, radiusY};

	SetColor(color);
	pDeviceContext2D->DrawEllipse(ellipse, GetDefaultBrush().Get(), strokeWidth);
}

wrl::ComPtr<ID2D1SolidColorBrush> Graphics::CreateSolidColorBrush(LinearColor color)
{
	wrl::ComPtr<ID2D1SolidColorBrush> pBrush;
	pDeviceContext2D->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(color.R, color.G, color.B, color.A)),
			&pBrush
		);

	return pBrush;
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

	pDeviceContext3D->RSSetViewports(1u, &vp); // one viewport, not split screen

	WGE_LOG(GraphicsLog, LogVerbosity::Default, "SetViewport %dx%d", width, height );
}

void Graphics::CalculateFrameStats()
{
	nFrames++;
	// compute average statistics over one second
	if ((timer->GetTotalTime() - elapsedTime) >= 1.0)
	{
		// set fps and mspf
		fps = nFrames;
		mspf = 1000.0 / (double)fps;

		// reset
		nFrames = 0;
		elapsedTime += 1.0;
	}
}

Timer* Graphics::GetTimer() const
{
	return timer.get();
}

int Graphics::GetFPS() const
{
	return fps;
}

double Graphics::GetFrameTime() const
{
	return mspf;
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
	pDeviceContext3D->OMSetRenderTargets(1u, pRenderTargetView3D.GetAddressOf(), pDepthStencilView3D.Get());

	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext3D->ClearRenderTargetView(pRenderTargetView3D.Get(), color);
	pDeviceContext3D->ClearDepthStencilView(pDepthStencilView3D.Get(), D3D11_CLEAR_DEPTH| D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	pDeviceContext3D->DrawIndexed(count,0u, 0u);
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

Graphics& Graphics::Get()
{
	static Graphics graphics;
	return graphics;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext1> Graphics::GetDeviceContext3D() const
{
	return pDeviceContext3D;
}

Microsoft::WRL::ComPtr<ID3D11Device1> Graphics::GetDevice3D() const
{
	return pDevice3D;
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Graphics::GetDefaultBrush() const
{
	return pDefaultBrush;
}

void Graphics::SetColor(const LinearColor& color)
{
	D2D_COLOR_F colorF = D2D1::ColorF(color.R, color.G, color.B, color.A);

	if (pDefaultBrush != nullptr)
	{
		pDefaultBrush->SetColor(colorF);
	}
	else
	{
		WGE_LOG(LogD2D_RHI, LogVerbosity::Warning, "Default brush not created");
	}
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
