#pragma once
#include "Window.h"
#include "Graphics/GUI/ImguiManager.h"
#include "Graphics/Engine/Actors/Camera.h"
#include <Graphics\DX11\Render\PointLight.h>

#include <memory>
#include <set>

#include "Graphics/DX11/Primitive/TestPlane.h"
#include "Graphics/Test/Level.h"

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const std::string& name, const std::string& commandLine = "");

	~ApplicationWindow();

	void CalculateFrameStats();

	int Run();
	
	void Update(double deltaTime);
	// generating output
	virtual void Render(double farseer);	// renders the game world

	void ShowImguiDemoWindow();
	void ShowRawInputWindow();


	void OnResize() override;
	void OnPosChange() override;

private:
	int fps;							// frames per second
	double mspf;					    // milliseconds per frame
	double deltaTime;
	int maxSkipFrames;

	int x = 0, y = 0;
	bool showDemoWindow = false;
	Camera camera;
	PointLight light;
	float speedFactor = 1.0f;

	Level* level;
};


DECLARE_LOG_CATEGORY_EXTERN(ApplicationWindowLog);