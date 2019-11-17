#include "PointLight.h"
#include "Imgui\imgui.h"

PointLight::PointLight(Graphics& gfx, float radius /*= 0.5f*/)
	:
	m_mesh (gfx, radius),
	m_contantBuffer(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pcbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pcbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pcbData.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &m_pcbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &m_pcbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &m_pcbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &m_pcbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &m_pcbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &m_pcbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	m_pcbData = {
		{0.0f, 0.0f, 0.0f},
		{0.05f, 0.05f, 0.05f},
		{1.0f, 1.0f, 1.0f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f	
	};
}

void PointLight::Draw(Graphics& gfx) const
{
	m_mesh.SetPosition(m_pcbData.pos);
	m_mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const
{
	m_contantBuffer.Update(gfx, PointLightContantBuffer{ m_pcbData });
	m_contantBuffer.Bind(gfx);
}
