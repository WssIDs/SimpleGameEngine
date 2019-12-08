#pragma once
#include "Graphics/DX11/Graphics.h"
#include "Graphics/DX11/Bindable/ConstantBuffers.h"
#include "../Primitive/SolidSphere.h"
#include "../../Test/WObject.h"




class PointLight : public WObject
{
public:
	PointLight(float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw() const;
	void Bind(Graphics& gfx) const;

	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

private:
	struct PointLightConstantBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightConstantBuffer pcbData;
	mutable SolidSphere mesh;
	mutable Bind::PixelConstantBuffer<PointLightConstantBuffer> constantBuffer;
};

