#include "PointLight.h"
#include "Imgui\imgui.h"

PointLight::PointLight(Graphics& gfx, float radius /*= 0.5f*/)
	:
	m_mesh (gfx, radius),
	m_contantBuffer(gfx)
{

}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	m_pos = { 0.0f,0.0f,0.0f };
}

void PointLight::Draw(Graphics& gfx) const
{
	m_mesh.SetPosition(m_pos);
	m_mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const
{
	m_contantBuffer.Update(gfx, PointLightContantBuffer{ m_pos });
	m_contantBuffer.Bind(gfx);
}
