#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

// Test Imported mesh
#include "Graphics/Tests/AssetTest.h"

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>

#include "Graphics/DX11/GDIPlusManager.h"
#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"

namespace dx = DirectX;

GDIPlusManager gdipm;
ImguiManager imgui;


ApplicationWindow::ApplicationWindow(int width, int height,const TSTRING name)
	:Window(width,height,name),
	light(Gfx())
{
	S_LOG(TEXT("Application Window"), TEXT("Create"));


	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	
	//Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

ApplicationWindow::~ApplicationWindow()
{
	S_LOG(TEXT("Application Window"), TEXT("Destroy"));
}

int ApplicationWindow::Update()
{
	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		onUpdate();
	}
}

void ApplicationWindow::onUpdate()
{
	Window::onUpdate();

	auto deltaSeconds = timer.Mark() * speedFactor;

	Gfx().BeginFrame(0.07,0.0f,0.12f); // StartFrame

	// DRAW/LOGICS

	Gfx().SetCamera(camera.GetMatrix());
	light.Bind(Gfx(), camera.GetMatrix());


	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	model.Draw(Gfx(), transform);

	light.Draw(Gfx());


	// render imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowModelWindow();

	Gfx().EndFrame(); // EndFrame
}

void ApplicationWindow::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}
