#pragma once
#include "ConstantBuffers.h"
#include "Graphics/DX11/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	virtual void Bind(Graphics& gfx) override;

private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> m_VertexConstantBuffer;
	const Drawable& m_Parent;
};

