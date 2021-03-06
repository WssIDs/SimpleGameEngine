#pragma once
#include <Runtime/D3D11RHI/RHIInterface.h>
#include "../../Core/Math/Color.h"
#include "../../Logger/LogDefinitions.h"


namespace Bind
{
	class Bindable;
}

enum class DirectVersionName
{
	DirectX11,
	DirectX11_1,
	DirectX12
};


class DX11RHI : public RHIInterface
{
	friend Bind::Bindable;

public:
	DX11RHI(const DX11RHI&) = delete;
	DX11RHI& operator=(const DX11RHI&) = delete;
	~DX11RHI();

	virtual void Init(HWND hWnd, int width, int height) override;

	// init DX11_0
	void InitDX11();
	// init DX11_1
	void InitDX11_1();

	// functions to change screen resolutions

	// changes the screen resolution, if increase is true, a higher resolution is chosen, else the resolution is lowered
	// returns true if the screen resolution should be changed
	void changeResolution(bool increase);	
	void OnBordlessMaximize();						// resize the resources
	bool OnResize();						// resize the resources
	bool GetFullScreenState() const;			// get full screen state
	bool IsCurrentInFullScreen() const;
	// test function
	void InitDX2D();
	void Begin2DFrame();
	void End2DFrame();
	void ClearScreen(LinearColor color);
	void DrawCircle();

	void DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY, const std::wstring& fontName = L"Verdana");
	
	void DrawRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color, const float strokeWidth = 1.0f);
	void DrawFillRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color);

	void DrawEllipse(const float screenX, const float screenY, const float radiusX, const float radiusY, const LinearColor& color, const float strokeWidth);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> CreateSolidColorBrush(LinearColor color);

	void SetViewport(int width, int height);

	// Clear buffers and set a new TragetView 
	void BeginFrame(float red, float green, float blue);
	// Show scene
	void EndFrame();
	// Draw Objects
	void DrawIndexed(UINT count);
	void SetProjection(DirectX::XMMATRIX projection);
	DirectX::XMMATRIX GetProjection() const;
	void SetCamera(DirectX::XMMATRIX camera);
	DirectX::XMMATRIX GetCamera() const;

	// ImGui
	void EnableImgui();
	void DisableImgui();
	bool IsImguiEnabled() const;

protected:

	DX11RHI() {};

public:

	static DX11RHI& Get();

	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> GetDeviceContext3D() const;
	Microsoft::WRL::ComPtr<ID3D11Device1> GetDevice3D() const;

private:

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GetDefaultBrush() const;
	// Set Color Default brush
	void SetColor(const LinearColor& color);

	HWND hWnd = nullptr;

	void PrintListAdapters(DirectVersionName dVersionName, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory, UINT deviceId);

	int width = 0;
	int height = 0;

	// color format
	DXGI_FORMAT desiredColourFormat = {};						// the desired color format
	unsigned int numberOfSupportedModes = 0;					// the number of supported screen modes for the desired color format
	DXGI_MODE_DESC* supportedModes = nullptr;					// list of all supported screen modes for the desired color format
	DXGI_MODE_DESC  currentModeDescription = {};				// description of the currently active screen mode
	unsigned int currentModeIndex = 0;							// the index of the current mode in the list of all supported screen modes
	bool startInFullscreen = false;								// true if the game should start in full screen mode
	bool currentlyInFullscreen = false;							// true if the game is currently in full screen mode
	bool changeMode = false;									// true if the screen resolution should be changed this frame

	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX camera = DirectX::XMMatrixIdentity();

	bool imguiEnabled = true;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwap = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device1> pDevice3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pDeviceContext3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView3D = nullptr;


	Microsoft::WRL::ComPtr<IDWriteFactory2> pDwriteFactory2D = nullptr;			// pointer to the DirectWrite factory
	Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D = nullptr;					// pointer to the Direct2D factory
	Microsoft::WRL::ComPtr<ID2D1Device1> pDevice2D = nullptr;					// pointer to the Direct2D device
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> pDeviceContext2D = nullptr;		// pointer to the device context

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pDefaultBrush = nullptr;
};


DECLARE_LOG_CATEGORY_EXTERN(GraphicsLog);
DECLARE_LOG_CATEGORY_EXTERN(LogD3D11_1RHI);
DECLARE_LOG_CATEGORY_EXTERN(LogD2D_RHI);