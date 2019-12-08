#include "PointLight.h"
#include "Imgui\imgui.h"
#include "..\Graphics.h"

PointLight::PointLight(const std::string& name, float radius /*= 0.5f*/)
	:
	name(name),
	mesh (Graphics::GetGraphics(), radius),
	constantBuffer(Graphics::GetGraphics())
{
	Reset();
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin(name.c_str()))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pcbData.pos.x, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("Y", &pcbData.pos.y, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("Z", &pcbData.pos.z, -1000.0f, 1000.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &pcbData.diffuseIntensity, FLT_MIN, 50.0f, "%.2f", 1);
		ImGui::ColorEdit3("Diffuse Color", &pcbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &pcbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &pcbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &pcbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &pcbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	pcbData = {
		{-35.0f, 160.0f, -50.0f},
		{0.2f, 0.2f, 0.2f},
		{1.0f, 1.0f, 1.0f},
		8.0f,
		1.0f,
		0.045f,
		0.0075f	
	};
}

void PointLight::Draw() const
{
	mesh.SetPosition(pcbData.pos);
	mesh.Draw(Graphics::GetGraphics());
}

void PointLight::Bind(Graphics& gfx) const
{
	auto pcbDataCopy = pcbData;
	const auto pos = DirectX::XMLoadFloat3(&pcbData.pos);
	DirectX::XMStoreFloat3(&pcbDataCopy.pos,DirectX::XMVector3Transform( pos, gfx.GetCamera()));
	constantBuffer.Update(Graphics::GetGraphics(),pcbDataCopy);
	constantBuffer.Bind(Graphics::GetGraphics());
}

void PointLight::Tick(double deltaTime)
{
	
}

void PointLight::Render(double deltaTime)
{
	WObject::Render(deltaTime);
	Bind(Graphics::GetGraphics());
	Draw();
	SpawnControlWindow();
}
