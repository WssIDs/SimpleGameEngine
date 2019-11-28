#pragma once
#include "Bindable.h"


namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type);
		virtual void Bind(Graphics& gfx) override;
		static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type);
		static std::string GenerateUID(D3D_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const override;
	protected:
		D3D_PRIMITIVE_TOPOLOGY type;
	};
}

