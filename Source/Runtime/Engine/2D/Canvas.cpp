#include "Canvas.h"
#include  <Runtime/Launch/App.h>
//#include "..\DX11\Graphics.h"

Canvas::Canvas()
{
}

void Canvas::Render(double deltaTime)
{
	WObject::Render(deltaTime);

	DrawText(L"����� ���������� � Direct 2D", 10.0f, LinearColor::White, 10.0f, 10.0f);
	ShowFPS();

	DrawFillRect(10.0f, 80.0f, 140.0f, 200.f, LinearColor(0.0f, 0.5f, 1.0f, 1.0f));
}

void Canvas::DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY, const std::wstring& fontName)
{
	DX11RHI::Get().DrawText(text, fontSize, textColor, screenX, screenY, fontName);
}

void Canvas::DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY)
{
	DX11RHI::Get().DrawText(text, fontSize, textColor, screenX, screenY);
}

void Canvas::DrawFillRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color)
{
	DX11RHI::Get().DrawFillRect(screenX, screenY, screenW, screenH, color);
}

void Canvas::ShowFPS()
{
	DrawText(L"FPS: " + std::to_wstring(FApp::GetFramePerSeconds()), 10.0f, LinearColor::White, 10.0f, 25.0f);
	DrawText(L"FrameTime: " + std::to_wstring(FApp::GetMiliSecondsPerFrame()) + L" ms", 10.0f, LinearColor::White, 10.0f, 40.0f);
}

void Canvas::Tick(double deltaTime)
{
	WObject::Tick(deltaTime);
}
