#pragma once
#include "ConstantBuffers.h"
#include "Graphics/DX11/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
	virtual void Bind(Graphics& gfx) override;

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> m_VertexConstantBuffer;
	const Drawable& m_Parent;
};

