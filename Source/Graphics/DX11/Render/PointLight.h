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
	void Bind(Graphics& gfx) const;
private:
	struct PointLightContantBuffer
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 m_pos = { 0.0f, 0.0f, 0.0f };
	mutable SolidSphere m_mesh;
	mutable PixelConstantBuffer<PointLightContantBuffer> m_contantBuffer;
};

