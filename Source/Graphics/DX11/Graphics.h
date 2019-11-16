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
	DirectX::XMMATRIX m_projection;
	DirectX::XMMATRIX m_camera;

	bool m_imguiEnabled = true;

	Microsoft::WRL::ComPtr<ID3D11Device1> m_pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
};

