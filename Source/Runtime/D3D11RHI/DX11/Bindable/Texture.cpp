#include "Texture.h"
#include <Runtime/D3D11RHI/DX11/Render/Surface.h>

#include <DirectXTex/Loader/DDSTextureLoader.h>
//#include "DirectXTex.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCodex.h>
#include "DirectXTex.h"
#include "boost\archive\binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"

#pragma comment(lib, "dxguid.lib")

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(DX11RHI& gfx, const std::string& fileName, UINT slot)
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
			//DirectX::TexMetadata info;
			//auto image = std::make_shared<DirectX::ScratchImage>();
			//DirectX::LoadFromTGAFile(std::wstring(path.begin(), path.end()).c_str(), nullptr, *image);
			//DirectX::LoadFromDDSFile(std::wstring(path.begin(), path.end()).c_str(), DirectX::DDS_FLAGS_NONE, &info, *image);

			//if (image->GetImages() != nullptr)
			//{
			//	image = Convert(image);
			//}

			//bAlpha = !image->IsAlphaAllOpaque();
			//pixels = image->GetPixels();

			//sizePixels = sizeof(pixels);
			//if (pixels != nullptr)
			//{
			//	for (int y = 0; y < info.height; ++y)
			//	{
			//		for (int x = 0; x < info.width; ++x)
			//		{
			//			auto red = (unsigned int)pixels[x + 0 + y];
			//			auto green = (unsigned int)pixels[x + 1 + y];
			//			auto blue = (unsigned int)pixels[x + 2 + y];

			//			if (!bAlpha)
			//			{
			//				Colors.push_back(RGBAColor(red, green, blue));
			//			}
			//			else
			//			{
			//				auto alpha = (unsigned int)pixels[x + 2 + y];
			//				Colors.push_back(RGBAColor(red, green, blue, alpha));
			//			}
			//		}
			//	}
			//}

			//DirectX::ScratchImage mipchain;
			//DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipchain);
			//DirectX::CreateShaderResourceView(GetDevice(gfx), mipchain.GetImages(), mipchain.GetImageCount(), mipchain.GetMetadata(), &pTextureView);


		Microsoft::WRL::ComPtr<ID3D11Resource> pD3D11Resource = nullptr;
		DirectX::DDS_ALPHA_MODE alphaMode = DirectX::DDS_ALPHA_MODE_UNKNOWN;

		DirectX::CreateDDSTextureFromFile(GetDevice(gfx),
			String::ConvertToWideChar(fileName).c_str(),
			&pD3D11Resource,
			&pTextureView);

		if (pD3D11Resource != nullptr)
		{
			pD3D11Resource->QueryInterface(IID_ID3D11Texture2D, (void**)&pTexture);

			D3D11_TEXTURE2D_DESC td;
			pTexture->GetDesc(&td);

			if (td.Format == DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM)
			{
				bAlpha = false;
			}
			else if (td.Format == DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM)
			{
				bAlpha = true;
			}
		}
	}

	Texture::Texture()
	{
		////
	}

	void Texture::Bind(DX11RHI& gfx)
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}

	std::shared_ptr<Texture> Texture::Resolve(DX11RHI& gfx, const std::string& path, UINT slot)
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

	void Texture::Serialize()
	{
		std::ofstream ofs(Path::GetFileNameWithoutExtension(path) + ".asset", std::ios::out | std::ios::binary);
		boost::archive::binary_oarchive oa(ofs, boost::archive::no_header);
		oa << (*this);
	}

	void Texture::Deserialize(const std::string& filename)
	{
		name = Path::GetFileNameWithoutExtension(filename);
		std::ifstream ifs(filename, std::ios::in | std::ios::binary);
		boost::archive::binary_iarchive ia(ifs, boost::archive::no_header);
		ia >> (*this);
	}

	std::vector<RGBAColor> Texture::GetColors() const
	{
		return Colors;
	}

	bool Texture::HasAlpha() const
	{
		return bAlpha;
	}

	std::shared_ptr<DirectX::ScratchImage> Texture::Convert(std::shared_ptr<DirectX::ScratchImage> scratch)
	{
		auto decompressed = std::make_shared<DirectX::ScratchImage>();
		DirectX::Decompress(scratch->GetImages(), scratch->GetImageCount(), scratch->GetMetadata(), DXGI_FORMAT_UNKNOWN, *decompressed);

		if (decompressed->GetImages()->format != DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			auto converted = std::make_shared<DirectX::ScratchImage>();
			DirectX::Convert(
				*decompressed->GetImages(),
				DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
				DirectX::TEX_FILTER_DEFAULT,
				DirectX::TEX_THRESHOLD_DEFAULT,
				*converted
			);

			return converted;
		}

		return decompressed;
	}

}