#include "Camera.h"
#include "Imgui\imgui.h"
#include "..\..\DX11\Math\WGMath.h"
#include <algorithm>

namespace dx = DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const
{
	//return dx::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
	//	dx::XMMatrixRotationRollPitchYaw(-pitch, -yaw, 0.0f);

	const dx::XMVECTOR forwardBaseVector = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply camera ti a base vector
	const auto lookVector = dx::XMVector3Transform(forwardBaseVector,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);

	const auto camPosition = dx::XMLoadFloat3(&pos);
	const auto camTarget = dx::XMVectorAdd(camPosition, lookVector);
	//
	return dx::XMMatrixLookAtLH(camPosition, camTarget, dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::SpawnControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X",&pos.x,-80.0f,80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -800.0f, 800.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -800.0f, 800.0f, "%.1f");
		ImGui::Text("Orientation");
		//ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, 0.995f * -90.0f, 0.995f * 90.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset()
{
	pos = { 0.0f, 85.0f, -200.0f };
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}

void Camera::Rotate(float dx, float dy)
{
	const auto angle = yaw + dx * rotationspeed;
	yaw = WGMath::WrapAngle(angle);
	pitch = std::clamp(pitch + dy * rotationspeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation)
{
	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
		dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));

	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}

constexpr float Camera::rotationspeed;

constexpr float Camera::travelSpeed;
