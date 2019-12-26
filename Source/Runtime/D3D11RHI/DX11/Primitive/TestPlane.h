#pragma once
#include <Runtime/D3D11RHI/DX11/Drawable/Drawable.h>

class TestPlane : public Drawable
{
public:
	TestPlane(Graphics& gfx, float size);
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(float roll, float pitch, float yaw);
	DirectX::XMMATRIX GetTransformXM() const override;
	void SpawnControlWindow(Graphics& gfx);
private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.18f;
		float specularPower = 18.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1] = { 0.0f };
	} pmc;
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};

