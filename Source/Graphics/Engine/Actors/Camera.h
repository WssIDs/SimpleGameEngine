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

	void MoveForward(float forward);
	void MoveRight(float right);
	void MoveUp(float up);

private:
	float step = 10.f;

	DirectX::XMFLOAT3 pos;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationspeed = 0.004f;
};


DECLARE_LOG_CATEGORY_EXTERN(CameraLog)