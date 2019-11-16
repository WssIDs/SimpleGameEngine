#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
	:
	m_Parent(parent)
{
	if( !m_VertexConstantBuffer)
	{
		m_VertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	m_VertexConstantBuffer->Update(gfx,
		DirectX::XMMatrixTranspose(
			m_Parent.GetTransformXM() *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	);

	m_VertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformConstantBuffer::m_VertexConstantBuffer;
