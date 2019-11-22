#pragma once
#include "Window.h"
//#include "Graphics/DX12/GraphicsEngine.h"
//#include "Graphics/DX12/SwapChain.h"
//#include "Graphics/DX12/CommandQueue.h"
#include "Windows/Timer/Timer.h"
#include <memory>
#include "Graphics/DX11/Primitive/Box.h"
#include "Graphics/GUI/ImguiManager.h"
#include "Graphics/Engine/Actors/Camera.h"
#include <Graphics\DX11\Render\PointLight.h>
#include <set>
#include "Graphics/DX11/Primitive/Model.h"

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const TSTRING name);

	~ApplicationWindow();

	int Update();
	virtual void onUpdate() override;

	void ShowModelWindow();

private:
	Timer timer;
	Camera camera;
	PointLight light;
	float speedFactor = 1.0f;

	Model model{ Gfx(), R"(..\..\..\Content\Models\Juliette.fbx)" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

