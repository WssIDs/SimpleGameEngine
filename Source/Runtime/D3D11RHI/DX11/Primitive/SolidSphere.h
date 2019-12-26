#pragma once
#include <Runtime/D3D11RHI/DX11/Drawable/Drawable.h>


class SolidSphere : public Drawable
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float deltaSeconds) override;
	void SetPosition(DirectX::XMFLOAT3 position);
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};

