#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/ConstantBuffers.h>
#include <Runtime/D3D11RHI/DX11/Drawable/Drawable.h>
#include <DirectXMath.h>

namespace Bind
{
	class TransformConstantBuffer : public Bindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX modelViewProj;
			DirectX::XMMATRIX model;
		};
	public:
		TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
		void Bind(Graphics& gfx) override;

	protected:
		void UpdateBind(Graphics& gfx, const Transforms& transforms);
		Transforms GetTransforms(Graphics& gfx);
	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVertexConstantBuffer;
		const Drawable& Parent;
	};
}