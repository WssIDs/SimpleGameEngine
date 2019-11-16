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

class ApplicationWindow: public Window
{
public:
	ApplicationWindow(int width, int height,const wchar_t* name);

	~ApplicationWindow();

	int Update();

	virtual void onUpdate() override;

private:
	Timer timer;
	Camera camera;
	float speedFactor = 1.0f;
	std::vector<std::unique_ptr<class Drawable>> primitives;
	static constexpr size_t nPrimitives = 180;
};

