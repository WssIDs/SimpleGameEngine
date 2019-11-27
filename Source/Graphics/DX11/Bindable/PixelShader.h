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

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}
