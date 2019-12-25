#include "TestTexture.h"
#include "..\Loader\DDSTextureLoader.h"
#include "..\DX11\Graphics.h"
#include "..\Helpers\StringHelper.h"

TestTexture::TestTexture(const std::string& TexturePath)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pD3D11Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pD3D11ShaderResourceView = nullptr;

	auto Device = Graphics::GetGraphics().GetDevice3D().Get();

	DirectX::DDS_ALPHA_MODE alphaMode = DirectX::DDS_ALPHA_MODE_UNKNOWN;

	DirectX::CreateDDSTextureFromFile(Device,
		Graphics::GetGraphics().GetDeviceContext3D().Get(),
		String::ConvertToWideChar(TexturePath).c_str(),
		&pD3D11Resource,
		&pD3D11ShaderResourceView,
		0,
		&alphaMode);

	if (pD3D11Resource.Get() != nullptr)
	{
		pD3D11Resource->QueryInterface(IID_ID3D11Texture2D, (void**)&pTexture);

		D3D11_TEXTURE2D_DESC td;
		pTexture->GetDesc(&td);
		//pD3D11ShaderResourceView->

		if (td.Format == DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM)
		{
			bAlpha = false;
		}
		else if (td.Format == DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM)
		{
			bAlpha = true;
		}

		Width = td.Width;
		Height = td.Height;

		MipLevels = td.MipLevels;
		ArraySize = td.ArraySize;
		Format = td.Format;
		CountMultisamples = td.SampleDesc.Count;
		QualityLevel = td.SampleDesc.Quality;
		Usage = td.Usage;

		BindFlags = td.BindFlags;
		CPUAccessFlags = td.CPUAccessFlags;
		MiscFlags = td.MiscFlags;

		auto shaderViewDesc = D3D11_SHADER_RESOURCE_VIEW_DESC{};
		shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderViewDesc.Format = td.Format;
		shaderViewDesc.Texture2D.MostDetailedMip = 0;
		shaderViewDesc.Texture2D.MipLevels = MipLevels;

		if (pTexture != nullptr)
		{
			Device->CreateShaderResourceView(pTexture.Get(), &shaderViewDesc, &ShaderResourceView);

			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

			Device->CreateSamplerState(&sampDesc, &SamplerState);

			bLoaded = true;
		}
	}
}

bool TestTexture::IsLoaded()
{
	return bLoaded;
}
