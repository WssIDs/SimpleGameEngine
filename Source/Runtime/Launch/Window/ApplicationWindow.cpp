#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>
#include <algorithm>
#include <ThirdParty/Imgui/imgui.h>
#include <thread>
#include "../LaunchEngineLoop.h"

DEFINE_LOG_CATEGORY(ApplicationWindowLog);

namespace dx = DirectX;

ImguiManager imgui;


FEngineLoop	GEngineLoop;

ApplicationWindow::ApplicationWindow(int width, int height, const std::string& name, const std::string& commandLine)
	:
	Window(width, height, name, commandLine)
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Create");

	EngineInput = std::make_shared<InputSystem>();

	if (EngineInput != nullptr)
	{
		EngineInput->BindAction("EngineExit", EInputEvent::IE_Pressed, this, &ApplicationWindow::EngineExit);
		EngineInput->BindAction("ChangeFullSceenMode", EInputEvent::IE_Pressed, this, &ApplicationWindow::ChangeFullScreenMode);

		WindowKeyMessageHandler::Get()->SetEngineInputSystem(EngineInput.get());
	}

	level = new Level();

	if (level != nullptr)
	{
		level->Load();
	}

	Graphics::Get().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 5000.0f));

	EngineInit();
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
	FTimer::Get()->Reset();

	FTimer::Get()->ResetAccumulatedTime();		// stores the time accumulated by the rendered
	FTimer::Get()->ResetNumberLoops();			// the number of completed loops while updating the game

	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		// let the timer tick
		FTimer::Get()->Tick();

		if (!IsPaused())
		{
			// calculate frame per seconds
			FTimer::Get()->CalculateFramePerSeconds();
			
			// accumulate the elapsed time since the last frame
			FTimer::Get()->AccumulateTime(true);
			
			// now update the game logic with fixed deltaTime as often as possible
			FTimer::Get()->ResetNumberLoops();
			while (FTimer::Get()->GetAccumulatedTime() >= FTimer::Get()->GetDefaultDeltaTime() && FTimer::Get()->GetNumberLoops() < FTimer::Get()->GetMaxSkipFrames())
			{
				Update(FTimer::Get()->GetDefaultDeltaTime());
				FTimer::Get()->AccumulateTime(false);
				FTimer::Get()->GenerateNumberLoops();
			}

			Render(FTimer::Get()->GetAccumulatedTime() / FTimer::Get()->GetDefaultDeltaTime());
		}

		EngineTick();
	}
}

void ApplicationWindow::Update(double deltaTime)
{
	WindowKeyMessageHandler::Get()->Tick(deltaTime);

	/*while (const auto e = keyboardInput.ReadKey())
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
	}*/

	if (level != nullptr)
	{
		level->Tick(deltaTime);
	}
}

void ApplicationWindow::Render(double farseer)
{
	Graphics::Get().BeginFrame(0.07, 0.0f, 0.12f); // StartFrame

	// DRAW/LOGICS

	if (level != nullptr)
	{
		level->Render(farseer);
	}

	Graphics::Get().EndFrame(); // EndFrame
}

void ApplicationWindow::OnResize()
{
	Window::OnResize();
	Graphics::Get().OnResize();
}

void ApplicationWindow::OnPosChange()
{
	bool fullscreen = Graphics::Get().GetFullScreenState();

	if (fullscreen != (bool)Graphics::Get().IsCurrentInFullScreen())
	{
		SetPause(true);
		FTimer::Get()->Stop();
		Graphics::Get().OnResize();
		FTimer::Get()->Start();
		SetPause(false);
	}
}

void ApplicationWindow::EngineExit()
{
	CloseWindow();
}

void ApplicationWindow::ChangeFullScreenMode()
{
	ToggleBordlessFullScreenMode();
}
