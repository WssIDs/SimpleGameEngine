#pragma once
#include "Window.h"
#include "Windows/Timer/Timer.h"

#include "Graphics/GUI/ImguiManager.h"
#include "Graphics/Engine/Actors/Camera.h"
#include <Graphics\DX11\Render\PointLight.h>

#include <memory>
#include <set>

#include "Graphics/DX11/Primitive/Mesh.h"
#include "Graphics/DX11/Primitive/TestPlane.h"

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const std::string& name);

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

	//Model model{ Gfx(), BASE_MODELS_DIR + "Wall.fbx" };
	Model girl{ Gfx(), BASE_MODELS_DIR + "Girl.fbx" };
	//TestPlane plane {Gfx(), 50.0f };
};


DECLARE_LOG_CATEGORY_EXTERN(ApplicationWindowLog);