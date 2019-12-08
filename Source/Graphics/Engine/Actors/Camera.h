#pragma once
#include "Graphics/DX11/Graphics.h"
#include "../Core.h"



class Camera : public WObject
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const;
	// Imgui window
	void SpawnControlWindow();
	void Reset();
	void Rotate(float dx, float dy);
	void Translate(DirectX::XMFLOAT3 translation);


	virtual void Tick(double deltaTime) override;


	virtual void Render(double deltaTime) override;

private:
	DirectX::XMFLOAT3 pos;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationspeed = 0.004f;
};