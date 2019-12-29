#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>


namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(DX11RHI& gfx, D3D_PRIMITIVE_TOPOLOGY type);
		virtual void Bind(DX11RHI& gfx) override;
		static std::shared_ptr<Topology> Resolve(DX11RHI& gfx, D3D_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const override;
	protected:
		D3D_PRIMITIVE_TOPOLOGY type;
	};
}

