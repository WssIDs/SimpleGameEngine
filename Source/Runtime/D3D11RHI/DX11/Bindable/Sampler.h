#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>


namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx) override;

		static std::shared_ptr<Sampler> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}