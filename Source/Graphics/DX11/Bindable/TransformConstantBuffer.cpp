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
	const auto modelView = m_Parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};

	m_VertexConstantBuffer->Update(gfx, tf);
	m_VertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_VertexConstantBuffer;
