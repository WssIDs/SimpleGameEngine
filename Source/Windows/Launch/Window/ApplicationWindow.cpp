#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>

#include "Graphics/DX11/GDIPlusManager.h"
#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"
#include "Graphics/DX11/Bindable/Sampler.h"


DEFINE_LOG_CATEGORY(ApplicationWindowLog);

namespace dx = DirectX;

GDIPlusManager gdipm;
ImguiManager imgui;


ApplicationWindow::ApplicationWindow(int width, int height,const std::string& name)
	:Window(width,height,name),
	light(Gfx(), 5.0f)
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Create");
	
	//model.SetRootTransform(dx::XMMatrixTranslation(-60.0f, 50.0f, 0.0f));
	//plane.SetPosition({ 60.f,50.0f,0.0f });

	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 500.0f));
}

ApplicationWindow::~ApplicationWindow()
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Destroy");
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


	//model.Draw(Gfx());
	//plane.Draw(Gfx());
	girl.Draw(Gfx());
	light.Draw(Gfx());

	while (const auto e = keyboardInput.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		if (keyboardInput.KeyIsPressed(VK_SHIFT) && keyboardInput.KeyIsPressed(VK_F1))
		{
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
		}

		if (keyboardInput.KeyIsPressed(VK_F2))
		{	
			showDemoWindow = !showDemoWindow;
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
	//model.ShowWindow("Wall");
	girl.ShowWindow("Girl");
	//plane.SpawnControlWindow(Gfx());
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
