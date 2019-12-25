#pragma once
#include "Graphics/DX11/Graphics.h"
#include "Graphics/DX11/Bindable/ConstantBuffers.h"
#include "../../Engine/Core.h"
#include "../../Test/TestNewCube.h"

class TestNewSphere;


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

	Microsoft::WRL::ComPtr<ID3D11Buffer> pPointLightBuffer;
	
	PointLightConstantBuffer LightData;
	mutable std::shared_ptr<TestNewSphere> sphere;
	mutable std::shared_ptr<TestNewCube> cube;
	mutable std::string name;
	mutable Bind::PixelConstantBuffer<PointLightConstantBuffer> constantBuffer;
};

