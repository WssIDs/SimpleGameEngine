#include "PixelShader.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCodex.h>



namespace Bind
{
	PixelShader::PixelShader(DX11RHI& gfx, const std::string& path)
		:
		path(path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob(std::wstring(path.begin(), path.end()).c_str(), &pBlob);
		GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	}

	void PixelShader::Bind(DX11RHI& gfx)
	{
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	std::shared_ptr<PixelShader> PixelShader::Resolve(DX11RHI& gfx, const std::string& path)
	{
		return Codex::Resolve<PixelShader>(gfx, path);
	}

	std::string PixelShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + path;
 	}

	std::string PixelShader::GetUID() const
	{
		return GenerateUID(path);
	}

}