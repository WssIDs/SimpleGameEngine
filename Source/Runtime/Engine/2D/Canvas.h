#pragma once
#include <Runtime/Core/Core.h>

class Canvas : public WObject
{
public:
	Canvas();

	virtual void Tick(double deltaTime) override;
	virtual void Render(double deltaTime) override;

	void DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY, const std::wstring& fontName);
	void DrawText(const std::wstring& text, const float fontSize, LinearColor textColor, float screenX, float screenY);
	void DrawFillRect(const float screenX, const float screenY, const float screenW, const float screenH, const LinearColor& color);

	void ShowFPS();

};

