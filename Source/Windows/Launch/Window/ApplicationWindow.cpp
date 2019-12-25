#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>

#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"
#include "Graphics/DX11/Bindable/Sampler.h"
#include <thread>
#include "WindowKeyMessageHandler.h"
#include "../LaunchEngineLoop.h"
#include "Graphics/Test/NewCube.h"

DEFINE_LOG_CATEGORY(ApplicationWindowLog);

namespace dx = DirectX;

ImguiManager imgui;


FEngineLoop	GEngineLoop;

ApplicationWindow::ApplicationWindow(int width, int height, const std::string& name, const std::string& commandLine)
	:
	Window(width, height, name, commandLine),
	deltaTime(1.0 /(double)240),
	maxSkipFrames(10)
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Create");

	level = new Level();

	if (level != nullptr)
	{
		level->Load();
	}

	Graphics::GetGraphics().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 5000.0f));

	EngineInit();

	//Cube = std::make_shared<TestNewCube>();
	Sphere = std::make_shared<NewSphere>();
}

ApplicationWindow::~ApplicationWindow()
{
	delete level;
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Destroy");
}

int ApplicationWindow::EngineInit()
{
	int errorLever = GEngineLoop.Init();
	return errorLever;
}

void ApplicationWindow::EngineTick()
{
	GEngineLoop.Tick();
}

int ApplicationWindow::Run()
{
	EngineTick();

	// reset (start) the timer
	Graphics::GetGraphics().GetTimer()->reset();

	double accumulatedTime = 0.0;		// stores the time accumulated by the rendered
	int nLoops = 0;						// the number of completed loops while updating the game

	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		// let the timer tick
		Graphics::GetGraphics().GetTimer()->tick();

		if (!IsPaused())
		{
			// calculate fps
			Graphics::GetGraphics().CalculateFrameStats();
			
			// accumulate the elapsed time since the last frame
			accumulatedTime += Graphics::GetGraphics().GetTimer()->getDeltaTime();

			// now update the game logic with fixed deltaTime as often as possible
			nLoops = 0;
			while (accumulatedTime >= deltaTime && nLoops < maxSkipFrames)
			{
				Update(deltaTime);
				//Update(deltaTime);
				accumulatedTime -= deltaTime;
				nLoops++;

				//WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "delta = %lf, accumulatedTime = %lf, nLoops = %d", deltaTime, accumulatedTime, nLoops);
			}

			Render(accumulatedTime / deltaTime);
		}
	}
}

void ApplicationWindow::Update(double deltaTime)
{
	WindowKeyMessageHandler::Get()->Tick(deltaTime);

	//Sphere->Update((float)deltaTime);

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

		if (keyboardInput.KeyIsPressed(VK_PRIOR))
		{
			Graphics::GetGraphics().changeResolution(true);
		}

		if (keyboardInput.KeyIsPressed(VK_NEXT))
		{
			Graphics::GetGraphics().changeResolution(false);
		}

		if (keyboardInput.KeyIsPressed(VK_F2))
		{
			showDemoWindow = !showDemoWindow;
		}

		if (keyboardInput.KeyIsPressed(VK_MENU) && keyboardInput.KeyIsPressed(VK_F4))
		{
			CloseWindow();
		}

		if (keyboardInput.KeyIsPressed(VK_MENU) && keyboardInput.KeyIsPressed(VK_RETURN))
		{
			ToggleBordlessFullScreenMode();
		}
	}

	if (level != nullptr)
	{
		level->Tick(deltaTime);
	}
}

void ApplicationWindow::Render(double farseer)
{
	Graphics::GetGraphics().BeginFrame(0.07, 0.0f, 0.12f); // StartFrame

	// DRAW/LOGICS

	if (level != nullptr)
	{
		level->Render(farseer);
	}

	//Sphere->Draw();

	Graphics::GetGraphics().EndFrame(); // EndFrame
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

void ApplicationWindow::OnResize()
{
	Window::OnResize();
	Graphics::GetGraphics().OnResize();
}

void ApplicationWindow::OnPosChange()
{
	bool fullscreen = Graphics::GetGraphics().GetFullScreenState();

	if (fullscreen != (bool)Graphics::GetGraphics().IsCurrentInFullScreen())
	{
		SetPause(true);
		Graphics::GetGraphics().GetTimer()->stop();
		Graphics::GetGraphics().OnResize();
		Graphics::GetGraphics().GetTimer()->start();
		SetPause(false);
	}
}
