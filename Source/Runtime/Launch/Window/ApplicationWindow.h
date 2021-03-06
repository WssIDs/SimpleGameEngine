﻿#pragma once
#include "Window.h"
#include <Runtime/GUI/ImguiManager.h>
#include <Runtime/Engine/Level/Level.h>
#include "../../Engine/Test/NewMesh.h"
#include <memory>

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

	void EngineExit();
	void ChangeFullScreenMode();

private:
	std::shared_ptr<InputSystem> EngineInput;
	Level* level;
	std::shared_ptr<NewMesh> Mesh;
};


DECLARE_LOG_CATEGORY_EXTERN(ApplicationWindowLog);