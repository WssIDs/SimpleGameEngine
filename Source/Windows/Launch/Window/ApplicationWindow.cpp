#include "ApplicationWindow.h"
#include <iomanip>
#include <memory>

#include "Graphics/DX11/Primitive/Box.h"
#include "Graphics/DX11/Primitive/Cylinder.h"
#include "Graphics/DX11/Primitive/Pyramid.h"
#include "Graphics/DX11/Primitive/SkinnedBox.h"

// Test Imported mesh
#include "Graphics/Tests/AssetTest.h"

#include "Graphics/DX11/Math/WGMath.h"
#include <algorithm>

#include "Graphics/DX11/GDIPlusManager.h"
#include "Graphics/DX11/Render/Surface.h"
#include "Imgui/imgui.h"

namespace dx = DirectX;

GDIPlusManager gdipm;
ImguiManager imgui;


ApplicationWindow::ApplicationWindow(int width, int height,const TSTRING name)
	:Window(width,height,name),
	light(Gfx())
{
	S_LOG(TEXT("Application Window"), TEXT("Create"));

	class Factory
	{
	public:
		Factory(Graphics& gfx)
			: m_gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 material = { cdist(rng), cdist(rng), cdist(rng) };
			const DirectX::XMFLOAT3 scale = { 0.005f, 0.005f, 0.005f };
			
			switch (sdist(rng))
			{
			case 0:
				return std::make_unique<Box>(m_gfx,
					rng, adist, ddist,
					odist, rdist, bdist, material
					);
			case 1:
				return std::make_unique<Cylinder>(m_gfx,
					rng, adist, ddist, odist,
					rdist, bdist, tdist
					);
			case 2:
				return std::make_unique<Pyramid>(m_gfx,
					rng, adist, ddist, odist,
					rdist, tdist
					);
			case 3:
				return std::make_unique<SkinnedBox>(m_gfx,
					rng, adist, ddist, odist,
					rdist
					);
			case 4:
				return std::make_unique<AssetTest>(m_gfx,
					rng, adist, ddist,
					odist, rdist, material, scale
					);
			default:
				assert(false && "impossible drawable option in factory");
				return {};
			}
		}

		~Factory()
		{
		}

	private:
		Graphics& m_gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0, 4 };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3, 30 };
	};

	primitives.reserve(nPrimitives);
	std::generate_n(std::back_inserter(primitives), nPrimitives, Factory{ Gfx() });

	Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	
	// init box pointers for editing instance parameters
	for (auto& pd : primitives)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}
	
	//Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

ApplicationWindow::~ApplicationWindow()
{
	S_LOG(TEXT("Application Window"), TEXT("Destroy"));
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

	Gfx().BeginFrame(0.07,0.0f,0.12f); // StartFrame

	// DRAW/LOGICS

	Gfx().SetCamera(camera.GetMatrix());
	light.Bind(Gfx(), camera.GetMatrix());

	// render primitive
	for (auto& primitive : primitives )
	{
		primitive->Update(  keyboardInput.KeyIsPressed(VK_SPACE) ? 0.0f : deltaSeconds);
		primitive->Draw(Gfx());
	}

	light.Draw(Gfx());


	// render imgui windows
	SpawnSimulationWindow();
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	SpawnBoxManagerWindow();
	SpawnBoxWindows();

	Gfx().EndFrame(); // EndFrame
}

void ApplicationWindow::SpawnSimulationWindow()
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text(u8"Нажмите пробел, чтобы остановить сцену");
	}
	ImGui::End();
}

void ApplicationWindow::SpawnBoxManagerWindow()
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (unsigned int i = 0; i < boxes.size(); i++)
			{
				const bool selected = *comboBoxIndex == i;
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

void ApplicationWindow::SpawnBoxWindows()
{
	for (auto id = boxControlIds.begin(); id != boxControlIds.end();)
	{
		if(!boxes[*id]->SpawnControlWindow(*id,Gfx()))
		{
			id = boxControlIds.erase(id);
		}
		else
		{
			++id;
		}
	}
}
