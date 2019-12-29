#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <string>



namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(DX11RHI& gfx, const std::string& path);
		virtual void Bind(DX11RHI& gfx) override;
		static std::shared_ptr<PixelShader> Resolve(DX11RHI& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const override;

	private:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}
