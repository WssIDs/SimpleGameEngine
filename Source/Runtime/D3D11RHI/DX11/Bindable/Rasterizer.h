#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <array>

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(DX11RHI& gfx, bool twoSided);
		void Bind(DX11RHI& gfx) override;
		static std::shared_ptr<Rasterizer> Resolve(DX11RHI& gfx, bool twoSided);
		static std::string GenerateUID(bool twoSided);
		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		bool twoSided;
	};
}

