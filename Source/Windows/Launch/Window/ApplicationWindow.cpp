#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

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
	S_LOG(TEXT("Application Window"), Verbosity::Default, TEXT("Create"));


	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 500.0f));
	//Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

ApplicationWindow::~ApplicationWindow()
{
	S_LOG(TEXT("Application Window"), Verbosity::Default, TEXT("Destroy"));
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


	model.Draw(Gfx());

	light.Draw(Gfx());

	while (const auto e = keyboardInput.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (IsCursorEnabled())
			{
				DisableCursor();
				mouseInput.EnableRaw();
			}
			else
			{
				EnableCursor();
				mouseInput.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
		}
	}

	if(!IsCursorEnabled())
	{
		float step = 3.0f;

		if(keyboardInput.KeyIsPressed('W'))
		{
			camera.Translate({ 0.0f, 0.0f, deltaSeconds * step });
		}
		if (keyboardInput.KeyIsPressed('A'))
		{
			camera.Translate({ -deltaSeconds * step, 0.0f, 0.0f });
		}
		if (keyboardInput.KeyIsPressed('S'))
		{
			camera.Translate({ 0.0f, 0.0f, -deltaSeconds * step });
		}
		if (keyboardInput.KeyIsPressed('D'))
		{
			camera.Translate({ deltaSeconds * step, 0.0f, 0.0f });
		}
		if (keyboardInput.KeyIsPressed('R'))
		{
			camera.Translate({ 0.0f, deltaSeconds * step, 0.0f });
		}
		if (keyboardInput.KeyIsPressed('F'))
		{
			camera.Translate({ 0.0f, -deltaSeconds * step, 0.0f });
		}
	}

	while (const auto delta = mouseInput.ReadRawDelta())
	{
		if(!IsCursorEnabled())
		{
			camera.Rotate((float)delta->x, (float)delta->y);
		}
	}

	// render imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	model.ShowWindow();
	//ShowRawInputWindow();

	Gfx().EndFrame(); // EndFrame
}

void ApplicationWindow::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

void ApplicationWindow::ShowRawInputWindow()
{
	while (const auto d = mouseInput.ReadRawDelta())
	{
		x += d->x;
		y += d->y;
	}
	if (ImGui::Begin("Raw Input"))
	{
		ImGui::Text("Tally: (%d,%d)", x, y);
		ImGui::Text("Cursor: %s", IsCursorEnabled() ? "enabled" : "disabled");
	}
	ImGui::End();
}
