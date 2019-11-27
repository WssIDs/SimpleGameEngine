#include "Texture.h"
#include "Graphics/DX11/Render/Surface.h"

#include "DDSTextureLoader.h"
#include "DirectXTex.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const class Surface& surface, unsigned int slot)
		:
		slot(slot)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = surface.GetWidth();
		textureDesc.Height = surface.GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = surface.GetBufferPtr();
		sd.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);
		wrl::ComPtr<ID3D11Texture2D> pTexture;

		GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture);


		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);
	}

	Texture::Texture(Graphics& gfx, const std::string& fileName, unsigned int slot)
		:
		slot(slot)
	{
		//DirectX::TexMetadata info;
		auto image = std::make_unique<DirectX::ScratchImage>();
		const std::wstring wfile = std::wstring(fileName.begin(), fileName.end());
		DirectX::LoadFromTGAFile(wfile.c_str(), nullptr, *image);
		DirectX::CreateShaderResourceView(GetDevice(gfx), image->GetImages(), image->GetImageCount(), image->GetMetadata(), &pTextureView);
	}

	void Texture::Bind(Graphics& gfx)
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}
}