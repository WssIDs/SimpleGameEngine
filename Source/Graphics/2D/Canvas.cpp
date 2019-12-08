#include "Canvas.h"
#include "..\DX11\Graphics.h"

void Canvas::Render(double deltaTime)
{
	WObject::Render(deltaTime);

	Graphics::GetGraphics().DrawText(L"Добро пожаловать в Direct 2D", 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f);
	Graphics::GetGraphics().DrawText(L"FPS: " + std::to_wstring(Graphics::GetGraphics().GetFPS()), 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 25.0f);
	Graphics::GetGraphics().DrawText(L"FrameTime: " + std::to_wstring(Graphics::GetGraphics().GetFrameTime()) + L" ms", 10.0f, LinearColor(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 40.0f);

	Graphics::GetGraphics().DrawFillRect(10.0f, 80.0f, 140.0f, 200.f, LinearColor(0.0f, 0.5f, 1.0f, 1.0f));
}

void Canvas::Tick(double deltaTime)
{
	WObject::Tick(deltaTime);
}
