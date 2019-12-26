#include "Blender.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCodex.h>


namespace Bind
{
	Blender::Blender(Graphics& gfx, bool blending, std::optional<float> factor)
		:
		blending(blending)
	{
		if (factor)
		{
			factors.emplace();
			factors->fill(*factor);
		}

		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
		auto& brt = blendDesc.RenderTarget[0];
		if (blending)
		{
			brt.BlendEnable = TRUE;

			if (factor)
			{
				brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
				brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
			}
			else
			{
				brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
				brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			}
		}

		GetDevice(gfx)->CreateBlendState(&blendDesc, &pBlender);
	}

	void Blender::Bind(Graphics& gfx)
	{
		const float* data = factors ? factors->data() : nullptr;
		GetContext(gfx)->OMSetBlendState(pBlender.Get(), data, 0xFFFFFFFF);
	}

	void Blender::SetFactor(float factor)
	{
		assert(factors);
		return factors->fill(factor);
	}

	float Blender::GetFactor() const
	{
		assert(factors);
		return factors->front();
	}

	std::shared_ptr<Blender> Blender::Resolve(Graphics& gfx, bool blending, std::optional<float> factor)
	{
		return Codex::Resolve<Blender>(gfx, blending, factor);
	}

	std::string Blender::GenerateUID(bool blending, std::optional<float> factor)
	{
		using namespace std::string_literals;
		return typeid(Blender).name() + "#"s + (blending ? "b"s : "n"s) + (factor ? "#f"s + std::to_string(*factor) : "");
	}

	std::string Blender::GetUID() const
	{
		return GenerateUID(blending, factors ? factors->front() : std::optional<float>{});
	}
}
