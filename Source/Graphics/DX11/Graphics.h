#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Graphics
{
	friend class Bindable;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	// init DX11_0
	void InitDX11(HWND hWnd);
	// init DX11_1
	void InitDX11_1(HWND hWnd);

	// Clearbuffers and set a new TragetView 
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
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	bool imguiEnabled = true;

	Microsoft::WRL::ComPtr<ID3D11Device1> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
};

