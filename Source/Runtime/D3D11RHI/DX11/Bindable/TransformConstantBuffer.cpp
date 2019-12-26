#include "TransformConstantBuffer.h"


namespace Bind
{
	TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot /*0u*/)
		:
		Parent(parent)
	{
		if (!pVertexConstantBuffer)
		{
			pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformConstantBuffer::Bind(Graphics& gfx)
	{
		UpdateBind(gfx, GetTransforms(gfx));
	}

	void TransformConstantBuffer::UpdateBind(Graphics& gfx, const Transforms& transforms)
	{
		pVertexConstantBuffer->Update(gfx, transforms);
		pVertexConstantBuffer->Bind(gfx);
	}

	Bind::TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransforms(Graphics& gfx)
	{
		const auto modelView = Parent.GetTransformXM() * gfx.GetCamera();
		return	{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(
				modelView *
				gfx.GetProjection()
			)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVertexConstantBuffer;
}