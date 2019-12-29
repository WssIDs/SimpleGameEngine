#include "TransformConstantBuffer.h"


namespace Bind
{
	TransformConstantBuffer::TransformConstantBuffer(DX11RHI& gfx, const Drawable& parent, UINT slot /*0u*/)
		:
		Parent(parent)
	{
		if (!pVertexConstantBuffer)
		{
			pVertexConstantBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformConstantBuffer::Bind(DX11RHI& gfx)
	{
		UpdateBind(gfx, GetTransforms(gfx));
	}

	void TransformConstantBuffer::UpdateBind(DX11RHI& gfx, const Transforms& transforms)
	{
		pVertexConstantBuffer->Update(gfx, transforms);
		pVertexConstantBuffer->Bind(gfx);
	}

	Bind::TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransforms(DX11RHI& gfx)
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