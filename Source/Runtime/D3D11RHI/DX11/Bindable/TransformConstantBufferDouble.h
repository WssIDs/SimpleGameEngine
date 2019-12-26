#pragma once
#include "TransformConstantBuffer.h"

namespace Bind
{
	class TransformConstantBufferDouble : public TransformConstantBuffer
	{
	public:
		TransformConstantBufferDouble(Graphics& gfx, const Drawable& parent, UINT slotV = 0u, UINT slotP = 0u);
		void Bind(Graphics& gfx) override;

	protected:
		void UpdateBind(Graphics& gfx, const Transforms& transforms);
	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPixelConstantBuffer;
	};
}

