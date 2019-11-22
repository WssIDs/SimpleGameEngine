#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot /*0u*/)
	:
	Parent(parent)
{
	if( !pVertexConstantBuffer)
	{
		pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	const auto modelView = Parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};

	pVertexConstantBuffer->Update(gfx, tf);
	pVertexConstantBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVertexConstantBuffer;
