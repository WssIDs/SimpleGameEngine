#pragma once
#include "Bindable.h"
#include <string>



namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::string& path);
		virtual void Bind(Graphics& gfx) override;
		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const override;

	private:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}
