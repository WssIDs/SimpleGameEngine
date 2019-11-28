#include "PointLight.h"
#include "Imgui\imgui.h"

PointLight::PointLight(Graphics& gfx, float radius /*= 0.5f*/)
	:
	mesh (gfx, radius),
	constantBuffer(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pcbData.pos.x, -150.0f, 150.0f, "%.1f");
		ImGui::SliderFloat("Y", &pcbData.pos.y, -200.0f, 200.0f, "%.1f");
		ImGui::SliderFloat("Z", &pcbData.pos.z, -150.0f, 150.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &pcbData.diffuseIntensity, 0.01f, 50.0f, "%.2f", 1);
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
		{-50.0f, 80.0f, -70.0f},
		{0.05f, 0.05f, 0.05f},
		{1.0f, 1.0f, 1.0f},
		10.0f,
		1.0f,
		0.045f,
		0.0075f	
	};
}

void PointLight::Draw(Graphics& gfx) const
{
	mesh.SetPosition(pcbData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const
{
	auto pcbDataCopy = pcbData;
	const auto pos = DirectX::XMLoadFloat3(&pcbData.pos);
	DirectX::XMStoreFloat3(&pcbDataCopy.pos,DirectX::XMVector3Transform( pos, view));
	constantBuffer.Update(gfx,pcbDataCopy);
	constantBuffer.Bind(gfx);
}
