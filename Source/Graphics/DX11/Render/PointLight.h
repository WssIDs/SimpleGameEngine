#pragma once
#include "Graphics/DX11/Graphics.h"
#include "Graphics/DX11/Bindable/ConstantBuffers.h"
#include "../Primitive/SolidSphere.h"




class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw(Graphics& gfx) const;
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const;
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
	mutable PixelConstantBuffer<PointLightConstantBuffer> constantBuffer;
};

