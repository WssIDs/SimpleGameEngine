#pragma once
#include <Runtime/Core/Core.h>
#include <Runtime/D3D11RHI/DX11/Bindable/ConstantBuffers.h>
#include <Runtime/Engine/Test/Buffer.h>
#include <Runtime/Engine/Test/Primitive.h>
#include "Runtime/Engine/Test/NewMesh.h"

class PointLight : public WObject
{
public:
	PointLight();
	PointLight(const std::string& name);
	PointLight(const std::string& name, float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw() const;
	void Bind(DX11RHI& gfx) const;

	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

	DirectX::XMFLOAT3 GetPosition() const;

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
	Vector Location;
	PointLightConstantBuffer LightData;
	std::shared_ptr<PixelBuffer> LightBuffer;

	mutable std::shared_ptr<NewMesh> Mesh;
	mutable std::string Name;
};