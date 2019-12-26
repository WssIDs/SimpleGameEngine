#pragma once
#include "Window.h"
#include <Runtime/GUI/ImguiManager.h>
#include <Runtime/Engine/Level/Level.h>

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const std::string& name, const std::string& commandLine = "");

	~ApplicationWindow();

	int EngineInit();
	void EngineTick();
	//void EngineExit();

	int Run();
	
	void Update(double deltaTime);
	// generating output
	virtual void Render(double farseer);	// renders the game world

	void OnResize() override;
	void OnPosChange() override;

private:
	double deltaTime;
	int maxSkipFrames;

	//int x = 0, y = 0;
	bool showDemoWindow = false;
	float speedFactor = 1.0f;

	Level* level;
};


DECLARE_LOG_CATEGORY_EXTERN(ApplicationWindowLog);