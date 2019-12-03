#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Graphics/Engine/Core.h"
#include <d2d1.h>
#include <d2d1_3.h>
#include "Render/Color.h"

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

	// test function
	void InitDX2D(HWND hWnd);
	void Begin2DFrame();
	void End2DFrame();
	void ClearScreen(LinearColor color);
	void DrawCircle();

	void DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY, const std::wstring& fontName = L"Verdana");

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

	void PrintListAdapters(DirectVersionName dVersionName, Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory, UINT deviceId);

	int width;
	int height;

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	bool imguiEnabled = true;

	Microsoft::WRL::ComPtr<ID3D11Device1> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;


	Microsoft::WRL::ComPtr<ID2D1Factory> pFactory2D = nullptr;
	Microsoft::WRL::ComPtr<IDWriteFactory> pDwriteFactory = nullptr;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> pRenderTarget2D = nullptr;
};


DECLARE_LOG_CATEGORY_EXTERN(GraphicsLog);
DECLARE_LOG_CATEGORY_EXTERN(LogD3D11_1RHI);
DECLARE_LOG_CATEGORY_EXTERN(LogD2D_RHI);