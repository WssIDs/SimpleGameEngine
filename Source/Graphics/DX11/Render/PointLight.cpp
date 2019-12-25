#include "PointLight.h"
#include "Imgui\imgui.h"
#include "..\Graphics.h"
#include "..\..\Test\TestNewSphere.h"

PointLight::PointLight()
	:
	constantBuffer(Graphics::GetGraphics())
{
	Location = Vector(-35.0f, 160.0f, -50.0f);

	cube = std::make_shared<TestNewCube>();
	cube->SetLocation(Location);
	//SetName("Default0");

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(PointLightConstantBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbbd, nullptr, &pPointLightBuffer);

	Reset();
}

PointLight::PointLight(const std::string& name)
	:
	constantBuffer(Graphics::GetGraphics())
{
	Location = Vector(-35.0f, 160.0f, -50.0f);
	cube = std::make_shared<TestNewCube>();
	cube->SetLocation(Location);
	SetName(name);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(PointLightConstantBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbbd, nullptr, &pPointLightBuffer);

	Reset();
}

PointLight::PointLight(const std::string& name, float radius /*= 0.5f*/)
	:
	constantBuffer(Graphics::GetGraphics())
{
	Location = Vector(-35.0f, 160.0f, -50.0f);
	cube = std::make_shared<TestNewCube>();
	cube->SetLocation(Location);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(PointLightConstantBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbbd, nullptr, &pPointLightBuffer);

	Reset();
}

void PointLight::SpawnControlWindow()
{
	if (ImGui::Begin(GetName().c_str()))
	{
		ImGui::Text("Location");
		ImGui::SliderFloat("X", &Location.x, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("Y", &Location.y, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("Z", &Location.z, -1000.0f, 1000.0f, "%.1f");

		ImGui::Text("Rotation");
		ImGui::SliderAngle("Pitch", &cube->Rotation.Pitch, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Yaw", &cube->Rotation.Yaw, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Roll", &cube->Rotation.Roll, -180.0f, 180.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &LightData.diffuseIntensity, FLT_MIN, 50.0f, "%.2f", 1);
		ImGui::ColorEdit3("Diffuse Color", &LightData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &LightData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &LightData.attConst, 0.05f, 1000.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &LightData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &LightData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset()
{
	Location = Vector(-35.0f, 160.0f, -50.0f);

	LightData = {
		{Location.x, Location.y, Location.z},
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
	//mesh.SetPosition(pcbData.pos);
	//mesh.Draw(Graphics::GetGraphics());

	cube->SetLocation(Location);
	cube->Draw();
}

void PointLight::Bind(Graphics& gfx) const
{

	//constantBuffer.Update(Graphics::GetGraphics(), pcbDataCopy);
	//constantBuffer.Bind(Graphics::GetGraphics());

}

void PointLight::Tick(double deltaTime)
{
	cube->Update(deltaTime);
}

void PointLight::Render(double deltaTime)
{
	WObject::Render(deltaTime);
	//Bind(Graphics::GetGraphics());

	LightData.pos = DirectX::XMFLOAT3(Location.x, Location.y, Location.z);

	auto pcbDataCopy = LightData;
	const auto pos = DirectX::XMLoadFloat3(&LightData.pos);
	DirectX::XMStoreFloat3(&pcbDataCopy.pos, DirectX::XMVector3Transform(pos, Graphics::GetGraphics().GetCamera()));
	Graphics::GetGraphics().GetDeviceContext3D()->UpdateSubresource(pPointLightBuffer.Get(), 0, nullptr, &pcbDataCopy, 0, 0);
	Graphics::GetGraphics().GetDeviceContext3D()->PSSetConstantBuffers(0, 1, pPointLightBuffer.GetAddressOf());

	Draw();
	SpawnControlWindow();
}

DirectX::XMFLOAT3 PointLight::GetPosition() const
{
	return LightData.pos;
}
