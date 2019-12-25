#pragma once
#include "Graphics/DX11/Graphics.h"
#include "Graphics/DX11/Bindable/ConstantBuffers.h"
#include "../../Engine/Core.h"
#include "../../Test/NewCube.h"
#include "../../Test/PixelBuffer.h"

class NewSphere;


class PointLight : public WObject
{
public:
	PointLight();
	PointLight(const std::string& name);
	PointLight(const std::string& name, float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw() const;
	void Bind(Graphics& gfx) const;

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

	mutable std::shared_ptr<NewSphere> sphere;
	mutable std::shared_ptr<NewCube> cube;
	mutable std::string name;
};

