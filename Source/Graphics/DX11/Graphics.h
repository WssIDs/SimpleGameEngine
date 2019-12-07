#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Graphics/Engine/Core.h"
#include <d2d1.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include "Render/Color.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

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


class Graphics
{
	friend Bind::Bindable;
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	// init DX11_0
	void InitDX11(HWND hWnd);
	// init DX11_1
	void InitDX11_1(HWND hWnd);

	// functions to change screen resolutions

	// changes the screen resolution, if increase is true, a higher resolution is chosen, else the resolution is lowered
	// returns true if the screen resolution should be changed
	void changeResolution(bool increase);					
	bool OnResize();						// resize the resources
	bool GetFullScreenState() const;			// get full screen state
	bool IsCurrentInFullScreen() const;
	// test function
	void InitDX2D(HWND hWnd);
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

private:

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GetDefaultBrush() const;
	// Set Color Default brush
	void SetColor(const LinearColor& color);

	HWND hWnd;

	void PrintListAdapters(DirectVersionName dVersionName, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory, UINT deviceId);

	int width;
	int height;

	// colour format
	DXGI_FORMAT desiredColourFormat;						// the desired colour format
	unsigned int numberOfSupportedModes;					// the number of supported screen modes for the desired colour format
	DXGI_MODE_DESC* supportedModes;							// list of all supported screen modes for the desired colour format
	DXGI_MODE_DESC  currentModeDescription;					// description of the currently active screen mode
	unsigned int currentModeIndex;							// the index of the current mode in the list of all supported screen modes
	bool startInFullscreen;									// true if the game should start in fullscreen mode
	bool currentlyInFullscreen;								// true if the game is currently in fullscreen mode
	bool changeMode;										// true if the screen resolution should be changed this frame

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	bool imguiEnabled = true;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwap = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device1> pDevice3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pDeviceContext3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView3D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView3D = nullptr;


	Microsoft::WRL::ComPtr<IDWriteFactory2> pDwriteFactory2D;	// pointer to the DirectWrite factory
	Microsoft::WRL::ComPtr<ID2D1Factory2> pFactory2D;			// pointer to the Direct2D factory
	Microsoft::WRL::ComPtr<ID2D1Device1> pDevice2D;				// pointer to the Direct2D device
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> pDeviceContext2D;		// pointer to the device context

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pDefaultBrush;
};


DECLARE_LOG_CATEGORY_EXTERN(GraphicsLog);
DECLARE_LOG_CATEGORY_EXTERN(LogD3D11_1RHI);
DECLARE_LOG_CATEGORY_EXTERN(LogD2D_RHI);