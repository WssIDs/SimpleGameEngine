#include "Topology.h"


namespace Bind
{
	Topology::Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type)
		:
		Type(type)
	{}

	void Topology::Bind(Graphics& gfx)
	{
		GetContext(gfx)->IASetPrimitiveTopology(Type);
	}
}