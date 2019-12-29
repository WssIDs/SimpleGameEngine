#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>


namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(DX11RHI& gfx);
		void Bind(DX11RHI& gfx) override;

		static std::shared_ptr<Sampler> Resolve(DX11RHI& gfx);
		static std::string GenerateUID();
		std::string GetUID() const override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}