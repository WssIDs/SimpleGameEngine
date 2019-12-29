#pragma once
#include "TransformConstantBuffer.h"

namespace Bind
{
	class TransformConstantBufferDouble : public TransformConstantBuffer
	{
	public:
		TransformConstantBufferDouble(DX11RHI& gfx, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u);
		void Bind(DX11RHI& gfx) override;

	protected:
		void UpdateBind(DX11RHI& gfx, const Transforms& transforms);
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPixelConstantBuffer;
	};
}

