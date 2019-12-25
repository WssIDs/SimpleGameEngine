#include "Camera.h"
#include "Imgui\imgui.h"
#include "..\..\DX11\Math\WGMath.h"
#include <algorithm>
#include "..\..\Test\InputSystem.h"

DEFINE_LOG_CATEGORY(CameraLog)

namespace dx = DirectX;

Camera::Camera()
{
	CreatePlayerInputComponent();
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const
{
	//return dx::XMMatrixTranslation(-pos.x, -pos.y, -pos.z) *
	//	dx::XMMatrixRotationRollPitchYaw(-pitch, -yaw, 0.0f);

	const dx::XMVECTOR forwardBaseVector = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply camera ti a base vector
	const auto lookVector = dx::XMVector3Transform(forwardBaseVector,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	const auto camPosition = dx::XMLoadFloat3(&pos);
	const auto camTarget = dx::XMVectorAdd(camPosition, lookVector);

	const dx::XMVECTOR upVector = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//
	return dx::XMMatrixLookAtLH(camPosition, camTarget, upVector);
}

void Camera::SpawnControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Look speed", &step, 0.0f, 50.0f, "%.1f");
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
	pos = { 19.4f, 160.0f, -36.5f };
	pitch = 0.1f;
	yaw = -0.8f;
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

void Camera::Tick(double deltaTime)
{
	WObject::Tick(deltaTime);
}

void Camera::Render(double deltaTime)
{
	WObject::Render(deltaTime);
	Graphics::GetGraphics().SetCamera(GetMatrix());
	
	SpawnControlWindow();
}

void Camera::Test()
{
	if (!IsBlockInput())
	{
		WGE_LOG(CameraLog, LogVerbosity::Default, "TestAction");
	}
}

void Camera::Turn(float turn)
{
	if (!IsBlockInput())
	{
		//WGE_LOG(CameraLog, LogVerbosity::Default, "Turn = %f", turn);
		Rotate(turn * step, 0.0f);
	}
}

void Camera::LookUp(float lookup)
{
	if (!IsBlockInput())
	{
		//WGE_LOG(CameraLog, LogVerbosity::Default, "LookUp = %f", lookup);
		Rotate(0.0f, lookup * step);
	}
}

void Camera::MoveForward(float forward)
{
	if (!IsBlockInput())
	{
		//WGE_LOG(CameraLog, LogVerbosity::Default, "forward = %lf", forward);
		Translate({ 0.0f, 0.0f, (float)(forward) * 0.01f });
	}
}

void Camera::MoveRight(float right)
{
	if (!IsBlockInput())
	{
		//WGE_LOG(CameraLog, LogVerbosity::Default, "right = %lf", right);
		Translate({ (float)(right) * 0.01f , 0.0f, 0.0f });
	}
}

void Camera::MoveUp(float up)
{
	if (!IsBlockInput())
	{
		//WGE_LOG(CameraLog, LogVerbosity::Default, "up = %lf", up);
		Translate({ 0.0f, (float)(up) * 0.01f , 0.0f });
	}
}

void Camera::ToggleCamera()
{
	WGE_LOG(CameraLog, LogVerbosity::Default, "ToggleCamera");
	ToggleInput();
}

void Camera::SetupPlayerInputComponent(InputSystem* InputComponent)
{
	InputComponent->BindAxis("MoveCameraForward", this, &Camera::MoveForward);
	//TestInputSystem::Get().BindAxis("MoveCameraBackward", this, &Camera::MoveForward);
	//TestInputSystem::Get().BindAxis("MoveCameraLeft", this, &Camera::MoveRight);
	InputComponent->BindAxis("MoveCameraRight", this, &Camera::MoveRight);
	InputComponent->BindAxis("MoveCameraUp", this, &Camera::MoveUp);
	//TestInputSystem::Get().BindAxis("MoveCameraDown", this, &Camera::MoveUp);
	InputComponent->BindAxis("Turn", this, &Camera::Turn);
	InputComponent->BindAxis("LookUp", this, &Camera::LookUp);

	InputComponent->BindAction("TestAction", EInputEvent::IE_Pressed, this, &Camera::Test);

	InputComponent->BindAction("ToggleCameraInput", EInputEvent::IE_Pressed, this, &Camera::ToggleCamera);
}
