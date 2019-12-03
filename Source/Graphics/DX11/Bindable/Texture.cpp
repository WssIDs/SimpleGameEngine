#include "Texture.h"
#include "Graphics/DX11/Render/Surface.h"

#include <DirectXTex/Loader/DDSTextureLoader.h>
//#include "DirectXTex.h"
#include "BindableCodex.h"
#include "DirectXTex.h"

#pragma comment(lib, "dxguid.lib")

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const std::string& fileName, UINT slot)
		:
		path(fileName),
		slot(slot)
	{
		//if (bSurfaceLoading)
		//{
		//	// load surface
		//	const auto surface = Surface::FromFile(path);

		//	D3D11_TEXTURE2D_DESC textureDesc = {};
		//	textureDesc.Width = surface.GetWidth();
		//	textureDesc.Height = surface.GetHeight();
		//	textureDesc.MipLevels = 1;
		//	textureDesc.ArraySize = 1;
		//	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		//	textureDesc.SampleDesc.Count = 1;
		//	textureDesc.SampleDesc.Quality = 0;
		//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
		//	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//	textureDesc.CPUAccessFlags = 0;
		//	textureDesc.MiscFlags = 0;

		//	D3D11_SUBRESOURCE_DATA sd = {};
		//	sd.pSysMem = surface.GetBufferPtr();
		//	sd.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color);
		//	wrl::ComPtr<ID3D11Texture2D> pTexture;

		//	GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture);


		//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//	srvDesc.Format = textureDesc.Format;
		//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//	srvDesc.Texture2D.MostDetailedMip = 0;
		//	srvDesc.Texture2D.MipLevels = 1;

		//	GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);
		//}
		//else
		//{
			DirectX::TexMetadata info;
			auto image = std::make_unique<DirectX::ScratchImage>();
			//DirectX::LoadFromTGAFile(std::wstring(path.begin(), path.end()).c_str(), nullptr, *image);
			DirectX::LoadFromDDSFile(std::wstring(path.begin(), path.end()).c_str(), DirectX::DDS_FLAGS_NONE, &info, *image);
	
			bAlpha = !image->IsAlphaAllOpaque();

			DirectX::CreateShaderResourceView(GetDevice(gfx), image->GetImages(), image->GetImageCount(), image->GetMetadata(), &pTextureView);
		//}
	}

	void Texture::Bind(Graphics& gfx)
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}

	std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture>(gfx, path, slot);
	}

	std::string Texture::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#"s + std::to_string(slot);
	}

	std::string Texture::GetUID() const
	{
		return GenerateUID(path, slot);
	}

	bool Texture::HasAlpha() const
	{
		return bAlpha;
	}

}