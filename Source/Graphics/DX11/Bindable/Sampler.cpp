#include "Sampler.h"
#include "BindableCodex.h"


namespace Bind
{
	Sampler::Sampler(Graphics& gfx)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler);
	}

	void Sampler::Bind(Graphics& gfx)
	{
		GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
	}

	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx)
	{
		return Codex::Resolve<Sampler>(gfx);
	}

	std::string Sampler::GenerateUID()
	{
		return typeid(Sampler).name();
	}

	std::string Sampler::GetUID() const
	{
		return GenerateUID();
	}

}