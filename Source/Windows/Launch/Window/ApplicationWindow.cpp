#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>

#include "Graphics/DX11/GDIPlusManager.h"
#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"
#include "Graphics/DX11/Bindable/Sampler.h"
#include <thread>
#include <iosfwd>
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

DEFINE_LOG_CATEGORY(ApplicationWindowLog);

namespace dx = DirectX;

GDIPlusManager gdipm;
ImguiManager imgui;


ApplicationWindow::ApplicationWindow(int width, int height, const std::string& name, const std::string& commandLine)
	:
	Window(width, height, name, commandLine),
	fps(0),
	mspf(0.0),
	deltaTime(1.0 /(double)240),
	maxSkipFrames(10),
	light(Gfx(), 5.0f)
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Create");
	
	//model.SetRootTransform(dx::XMMatrixTranslation(-60.0f, 50.0f, 0.0f));
	//plane.SetPosition({ 60.f,50.0f,0.0f });

	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 500.0f));

	test.PrintAll();

	test.Change();

	test.PrintAll();
	// create and open a character archive for output
	std::ofstream ofs("filename");

	//save
	{
		boost::archive::text_oarchive oa(ofs);
		oa << test;
	}

	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "---------------------------------");

	TestObject newTest;

	//load
	{
		std::ifstream ifs("filename");
		boost::archive::text_iarchive ia(ifs);
		ia >> newTest;
	}

	newTest.PrintAll();

}

ApplicationWindow::~ApplicationWindow()
{
	WGE_LOG(ApplicationWindowLog, LogVerbosity::Default, "Destroy");
}

void ApplicationWindow::CalculateFrameStats()
{
	static int nFrames;				    // number of frames seen
	static double elapsedTime;		    // time since last call
	nFrames++;

	// compute average statistics over one second
	if ((timer.getTotalTime() - elapsedTime) >= 1.0)
	{
		// set fps and mspf
		fps = nFrames;
		mspf = 1000.0 / (double)fps;

		// reset
		nFrames = 0;
		elapsedTime += 1.0;
	}
}

int ApplicationWindow::Run()
{
	// reset (start) the timer
	timer.reset();

	double accumulatedTime = 0.0;		// stores the time accumulated by the rendered
	int nLoops = 0;						// the number of completed loops while updating the game

	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		// let the timer tick
		timer.tick();

		if (!IsPaused())
		{
			// calculate fps
			CalculateFrameStats();
			
			// accumulate the elapsed time since the last frame
			accumulatedTime += timer.getDeltaTime();

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
			Gfx().changeResolution(true);
		}

		if (keyboardInput.KeyIsPressed(VK_NEXT))
		{
			Gfx().changeResolution(false);
		}

		if (keyboardInput.KeyIsPressed(VK_F2))
		{
			showDemoWindow = !showDemoWindow;
		}

		if (keyboardInput.KeyIsPressed(VK_ESCAPE))
		{
			CloseWindow();
		}
	}




	while (const auto delta = mouseInput.ReadRawDelta())
	{
		if (!IsCursorEnabled())
		{
			camera.Rotate((float)delta->x, (float)delta->y);
		}
	}



	if (IsCursorEnabled())
	{
		while (!mouseInput.IsEmpty())
		{
			const auto mEvent = mouseInput.Read();
			float step = 3.0f;

			if (mEvent->GetType() == MouseInput::Event::Type::WheelUp)
			{
				camera.Translate({ 0.0f, 0.0f, (float)(deltaTime * step) });
			}
			if (mEvent->GetType() == MouseInput::Event::Type::WheelDown)
			{
				camera.Translate({ 0.0f, 0.0f, (float)(-deltaTime * step) });
			}
		}
	}


	if (!IsCursorEnabled())
	{
		double step = 3.0f;

		if (keyboardInput.KeyIsPressed('W'))
		{
			camera.Translate({ 0.0f, 0.0f, (float)(deltaTime * step) });
		}
		if (keyboardInput.KeyIsPressed('A'))
		{
			camera.Translate({ (float)(-deltaTime * step), 0.0f, 0.0f });
		}
		if (keyboardInput.KeyIsPressed('S'))
		{
			camera.Translate({ 0.0f, 0.0f, (float)(-deltaTime * step) });
		}
		if (keyboardInput.KeyIsPressed('D'))
		{
			camera.Translate({ (float)(deltaTime * step), 0.0f, 0.0f });
		}
		if (keyboardInput.KeyIsPressed('R'))
		{
			camera.Translate({ 0.0f, (float)(deltaTime * step), 0.0f });
		}
		if (keyboardInput.KeyIsPressed('F'))
		{
			camera.Translate({ 0.0f, (float)(-deltaTime * step), 0.0f });
		}
	}
}

void ApplicationWindow::Render(double farseer)
{
	Gfx().BeginFrame(0.07, 0.0f, 0.12f); // StartFrame

	// DRAW/LOGICS

	Gfx().DrawText(L"Добро пожаловать в Direct 2D", 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f);
	Gfx().DrawText(L"FPS: " + std::to_wstring(fps), 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 25.0f);
	Gfx().DrawText(L"FrameTime: " + std::to_wstring(mspf) + L" ms", 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 40.0f);

	Gfx().SetCamera(camera.GetMatrix());
	light.Bind(Gfx(), camera.GetMatrix());


	//model.Draw(Gfx());
	//plane.Draw(Gfx());
	//girl.Draw(Gfx());
	light.Draw(Gfx());

	
	// render imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	//model.ShowWindow("Wall");
	//girl.ShowWindow(Gfx(), "Girl");
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

void ApplicationWindow::OnResize()
{
	Gfx().OnResize();
}

void ApplicationWindow::OnPosChange()
{
	bool fullscreen = Gfx().GetFullScreenState();

	if (fullscreen != (bool)Gfx().IsCurrentInFullScreen())
	{
		SetPause(true);
		timer.stop();
		Gfx().OnResize();
		timer.start();
		SetPause(false);
	}
}
