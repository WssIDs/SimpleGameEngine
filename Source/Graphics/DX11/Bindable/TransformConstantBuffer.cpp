#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot /*0u*/)
	:
	m_Parent(parent)
{
	if( !m_VertexConstantBuffer)
	{
		m_VertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	const auto model = m_Parent.GetTransformXM();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};

	m_VertexConstantBuffer->Update(gfx, tf);
	m_VertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VertexConstantBuffer;
