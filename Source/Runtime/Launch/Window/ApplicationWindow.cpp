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
	Window(width, height, name, commandLine),
	deltaTime(1.0 /(double)240),
	maxSkipFrames(10)
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Create");

	EngineInput = std::make_shared<InputSystem>();

	if (EngineInput != nullptr)
	{
		EngineInput->BindAction("EngineExit", EInputEvent::IE_Pressed, this, &ApplicationWindow::EngineExit);

		WindowKeyMessageHandler::Get()->SetEngineInputSystem(EngineInput.get());
	}

	level = new Level();

	if (level != nullptr)
	{
		level->Load();
	}

	Graphics::Get().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 5000.0f));

	EngineInit();

	//Cube = std::make_shared<TestNewCube>();
	//Sphere = std::make_shared<NewSphere>();
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
	Graphics::Get().GetTimer()->Reset();

	double accumulatedTime = 0.0;		// stores the time accumulated by the rendered
	int nLoops = 0;						// the number of completed loops while updating the game

	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		// let the timer tick
		Graphics::Get().GetTimer()->Tick();

		if (!IsPaused())
		{
			// calculate fps
			Graphics::Get().CalculateFrameStats();
			
			// accumulate the elapsed time since the last frame
			accumulatedTime += Graphics::Get().GetTimer()->GetDeltaTime();

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
		Graphics::Get().GetTimer()->Stop();
		Graphics::Get().OnResize();
		Graphics::Get().GetTimer()->Start();
		SetPause(false);
	}
}

void ApplicationWindow::EngineExit()
{
	CloseWindow();
}
