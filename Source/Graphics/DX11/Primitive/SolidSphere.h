#pragma once
#include "..\Drawable\DrawableBase.h"


class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius);
	void Update(float deltaSeconds) override;
	void SetPosition(DirectX::XMFLOAT3 position);
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};

