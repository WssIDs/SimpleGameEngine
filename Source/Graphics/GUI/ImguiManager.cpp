#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include <filesystem>

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImFontConfig font_config;
	font_config.OversampleH = 1; //or 2 is the same
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	//init Cyrillic
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x044F, // Cyrillic
		0,
	};

	std::filesystem::path path = std::filesystem::path(R"(..\..\..\Content\Fonts\segoeui.ttf)");

	std::string filename = path.u8string();

	io.Fonts->AddFontFromFileTTF(filename.c_str(), 18.0f, &font_config, ranges);

	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
