#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

#include "Graphics/DX11/Primitive/Box.h"
#include "Graphics/DX11/Primitive/Melon.h"
#include "Graphics/DX11/Primitive/Sheet.h"
#include "Graphics/DX11/Primitive/Pyramid.h"
#include "Graphics/DX11/Primitive/SkinnedBox.h"

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>


#include "Graphics/DX11/GDIPlusManager.h"
#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"

namespace dx = DirectX;

GDIPlusManager gdipm;
ImguiManager imgui;

ApplicationWindow::ApplicationWindow(int width, int height,const wchar_t* name)
	:Window(width,height,name)
{
	S_LOG(L"Application Window", L"Create");


	class Factory
	{
	public:
		Factory(Graphics& gfx)
			: m_gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					m_gfx, rng, adist, ddist,
					odist, rdist
					);
			case 1:
				return std::make_unique<Box>(
					m_gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2:
				return std::make_unique<Melon>(
					m_gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			case 3:
				return std::make_unique<Sheet>(
					m_gfx, rng, adist, ddist,
					odist, rdist
					);
			case 4:
				return std::make_unique<SkinnedBox>(
					m_gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}

		~Factory()
		{
		}

	private:
		Graphics& m_gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,4 };
	};

	primitives.reserve(nPrimitives);
	std::generate_n(std::back_inserter(primitives), nPrimitives, Factory{ Gfx() });

	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

ApplicationWindow::~ApplicationWindow()
{
	S_LOG(L"Application Window", L"Destroy");
}

int ApplicationWindow::Update()
{
	while (true)
	{
		if (const auto ecode = ProcessMessages())
		{
			return *ecode;
		}

		onUpdate();
	}
}

void ApplicationWindow::onUpdate()
{
	Window::onUpdate();

	auto deltaSeconds = timer.Mark() * speedFactor;

	//if(keyboardInput.KeyIsPressed(VK_SPACE))
	//{
	//	Gfx().DisableImgui();
	//}
	//else
	//{
	//	Gfx().EnableImgui();
	//}

	Gfx().BeginFrame(0.07,0.0f,0.12f); // StartFrame

	// DRAW/LOGICS

	Gfx().SetCamera(camera.GetMatrix());

	for (auto& primitive : primitives )
	{
		primitive->Update(  keyboardInput.KeyIsPressed(VK_SPACE) ? 0.0f : deltaSeconds);
		primitive->Draw(Gfx());
	}

	//if(Gfx().IsImguiEnabled())
	//{
	//	bool test = Gfx().IsImguiEnabled();
	//	ImGui::ShowDemoWindow(&test);
	//}
	
	static char buffer[1024];

	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 15.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text(u8"Нажмите пробел, чтобы остановить сцену");
	}
	ImGui::End();

	camera.SpawnControlWindow();

	Gfx().EndFrame(); // EndFrame
}
