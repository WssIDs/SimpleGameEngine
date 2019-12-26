#include "Material.h"
#include "boost\archive\binary_oarchive.hpp"
#include "boost\archive\binary_iarchive.hpp"
#include <filesystem>

Material::Material(const std::string& EffectPath)
	:
	EffectPath(EffectPath)
{}

void Material::Load()
{
	std::wstring path = String::ConvertToWideChar(EffectPath);

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	Microsoft::WRL::ComPtr<ID3DBlob> msgError;

	
	HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "VS", "vs_5_0", flags, 0, &VertexShaderBuffer, &msgError);
	if(FAILED(hr))
	{
		const char* errorMsg = (const char*)msgError->GetBufferPointer();
		MessageBox(nullptr, errorMsg, "Shader Compilation Error", MB_RETRYCANCEL);
	}

	hr = D3DCompileFromFile(path.c_str(), nullptr, nullptr, "PS", "ps_5_0", flags, 0, &PixelShaderBuffer, &msgError);
	if (FAILED(hr))
	{
		const char* errorMsg = (const char*)msgError->GetBufferPointer();
		MessageBox(nullptr, errorMsg, "Shader Compilation Error", MB_RETRYCANCEL);
	}

	if (VertexShaderBuffer != nullptr || PixelShaderBuffer != nullptr)
	{
		Graphics::GetGraphics().GetDevice3D()->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, &VertexShader);
		Graphics::GetGraphics().GetDevice3D()->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, &PixelShader);

		if (VertexShader.Get() != nullptr || PixelShader.Get() != nullptr)
		{
			Graphics::GetGraphics().GetDeviceContext3D()->VSSetShader(VertexShader.Get(), nullptr, 0);
			Graphics::GetGraphics().GetDeviceContext3D()->PSSetShader(PixelShader.Get(), nullptr, 0);

			MaterialName = Path::GetFileNameWithoutExtension(EffectPath);

			auto* p1 = reinterpret_cast<unsigned char*>(VertexShaderBuffer->GetBufferPointer());
			auto  n1 = VertexShaderBuffer->GetBufferSize();

			VertexShaderBufferData.reserve(n1);
			std::copy(p1, p1 + n1, std::back_inserter(VertexShaderBufferData));

			auto* p2 = reinterpret_cast<unsigned char*>(PixelShaderBuffer->GetBufferPointer());
			auto  n2 = PixelShaderBuffer->GetBufferSize();

			PixelShaderBufferData.reserve(n2);
			std::copy(p2, p2 + n2, std::back_inserter(PixelShaderBufferData));

			bLoaded = true;
		}
		else
		{
			bLoaded = false;
		}

	}
	else
	{
		bLoaded = false;
	}

	//TexturePaths.push_back(BASE_TEXTURES_DIR + "uv_pattern.dds");
	//TexturePaths.push_back(BASE_TEXTURES_DIR + "main.dds");

	for (auto& texPath : TexturePaths)
	{
		std::shared_ptr<TestTexture> texture = std::make_shared<TestTexture>(texPath);

		if (texture->IsLoaded())
		{
			DiffuseTextures.push_back(texture);
			DiffuseShaderResourceViews.push_back(texture->ShaderResourceView.Get());
			DiffuseSamplers.push_back(texture->SamplerState.Get());
		}
	}
}

bool Material::IsLoaded()
{
	return bLoaded;
}

void Material::AddTexture(const std::string& texturepath)
{
	TexturePaths.push_back(texturepath);
}

void Material::Serialize()
{
	std::ofstream ofs(BASE_TEXTURES_DIR + Path::GetFileNameWithoutExtension(MaterialName) + ".asset", std::ios::out | std::ios::binary);
	boost::archive::binary_oarchive oa(ofs, boost::archive::no_header);
	oa << (*this);
}

void Material::Deserialize(const std::string& filename)
{
	MaterialName = Path::GetFileNameWithoutExtension(BASE_TEXTURES_DIR + filename);
	std::ifstream ifs(BASE_TEXTURES_DIR + filename, std::ios::in | std::ios::binary);
	boost::archive::binary_iarchive ia(ifs, boost::archive::no_header);
	ia >> (*this);
}
