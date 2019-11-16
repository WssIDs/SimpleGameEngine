#include "Topology.h"

Topology::Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type)
	:
	m_Type(type)
{}

void Topology::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetPrimitiveTopology(m_Type);
}
