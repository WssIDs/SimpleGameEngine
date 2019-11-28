#pragma once
#include "..\Drawable\Drawable.h"



class TestCube : public Drawable
{
public:
	TestCube(Graphics& gfx, float size);
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(float roll, float pitch, float yaw);
	DirectX::XMMATRIX GetTransformXM() const override;
	void SpawnControlWindow(Graphics& gfx);
private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1];
	} pmc;
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};

