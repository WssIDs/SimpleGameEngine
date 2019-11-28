#include "TransformConstantBufferDouble.h"

namespace Bind
{
	TransformConstantBufferDouble::TransformConstantBufferDouble(Graphics& gfx, const Drawable& parent, UINT slotV /*= 0u*/, UINT slotP /*= 0u*/)
		:
		TransformConstantBuffer(gfx,parent,slotV)
	{
		if( !pPixelConstantBuffer )
		{
			pPixelConstantBuffer = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, slotP);
		}
	}

	void TransformConstantBufferDouble::Bind(Graphics& gfx)
	{
		const auto transforms = GetTransforms(gfx);
		TransformConstantBuffer::UpdateBind(gfx, transforms);
		UpdateBind(gfx, transforms);
	}

	void TransformConstantBufferDouble::UpdateBind(Graphics& gfx, const Transforms& transforms)
	{
		pPixelConstantBuffer->Update(gfx, transforms);
		pPixelConstantBuffer->Bind(gfx);
	}

	std::unique_ptr<PixelConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBufferDouble::pPixelConstantBuffer;
}