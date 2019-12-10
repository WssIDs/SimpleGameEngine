#pragma once
#include "Window.h"
#include "Graphics/GUI/ImguiManager.h"

#include <memory>
#include <set>

#include "Graphics/DX11/Primitive/TestPlane.h"
#include "Graphics/Engine/Core/Level/Level.h"
#include "Graphics/Test/TestInputSystem.h"

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const std::string& name, const std::string& commandLine = "");

	~ApplicationWindow();

	int Run();
	
	void Update(double deltaTime);
	// generating output
	virtual void Render(double farseer);	// renders the game world

	void ShowImguiDemoWindow();
	void ShowRawInputWindow();

	void OnResize() override;
	void OnPosChange() override;

private:
	double deltaTime;
	int maxSkipFrames;

	int x = 0, y = 0;
	bool showDemoWindow = false;
	float speedFactor = 1.0f;

	Level* level;
};


DECLARE_LOG_CATEGORY_EXTERN(ApplicationWindowLog);