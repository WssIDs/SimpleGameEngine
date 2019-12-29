#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <array>
#include <optional>

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(DX11RHI& gfx, bool blending, std::optional<float> factor = {});
		void Bind(DX11RHI& gfx) override;
		void SetFactor(float factor);
		float GetFactor() const;
		static std::shared_ptr<Blender> Resolve(DX11RHI& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);

		std::string GetUID() const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}

