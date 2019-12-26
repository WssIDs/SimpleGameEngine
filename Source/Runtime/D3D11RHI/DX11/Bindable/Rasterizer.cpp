#include "Rasterizer.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCodex.h>

namespace Bind
{
	Rasterizer::Rasterizer(Graphics& gfx, bool twoSided)
		:
		twoSided(twoSided)
	{
		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;

		GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer);
	}

	void Rasterizer::Bind(Graphics& gfx)
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, bool twoSided)
	{
		return Codex::Resolve<Rasterizer>(gfx, twoSided);
	}

	std::string Rasterizer::GenerateUID(bool twoSided)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s");
	}

	std::string Rasterizer::GetUID() const
	{
		return GenerateUID(twoSided);
	}
}
