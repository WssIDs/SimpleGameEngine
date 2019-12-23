#pragma once
#include <Graphics/Engine/Core.h>
#include <d3d11.h>
#include <wrl/client.h>


class TestTexture
{
public:

	TestTexture(const std::string& TexturePath);
	
	bool IsLoaded();

	unsigned int Width;
	unsigned int Height;

	unsigned int MipLevels;
	unsigned int ArraySize;

	DXGI_FORMAT Format;

	// The number of multisamples per pixel
	unsigned int CountMultisamples;
	//The image quality level.The higher the quality, the lower the performance.
	unsigned int QualityLevel;

	D3D11_USAGE Usage;

	unsigned int BindFlags;
	unsigned int CPUAccessFlags;
	unsigned int MiscFlags;

	bool bAlpha;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;

private:

	bool bLoaded = false;
};

