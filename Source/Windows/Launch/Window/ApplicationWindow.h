#pragma once
#include "Window.h"
//#include "Graphics/DX12/GraphicsEngine.h"
//#include "Graphics/DX12/SwapChain.h"
//#include "Graphics/DX12/CommandQueue.h"
#include "Windows/Timer/Timer.h"
#include <memory>
#include "Graphics/GUI/ImguiManager.h"
#include "Graphics/Engine/Actors/Camera.h"
#include <Graphics\DX11\Render\PointLight.h>
#include <set>
#include "Graphics/DX11/Primitive/Mesh.h"

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const TSTRING name);

	~ApplicationWindow();

	int Update();
	virtual void onUpdate() override;

	void ShowImguiDemoWindow();
	void ShowRawInputWindow();
private:
	int x = 0, y = 0;
	bool showDemoWindow = false;
	Timer timer;
	Camera camera;
	PointLight light;
	float speedFactor = 1.0f;

	Model model{ Gfx(), R"(..\..\..\Content\Models\Girl.fbx)" };
};

