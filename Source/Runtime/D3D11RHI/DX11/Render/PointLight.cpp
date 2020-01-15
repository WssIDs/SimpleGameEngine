#include "PointLight.h"
#include <Runtime/Engine/Test/NewSphere.h>

PointLight::PointLight()
{
	Location = Vector(-35.0f, 160.0f, -50.0f);
	

	Mesh = std::make_shared<NewMesh>();
	Mesh->SetMesh("sphere.fbx");
	Mesh->SetMaterial("Sphere.fx");
	Mesh->SetLocation(Location);
	Mesh->SetScale3D(Vector(0.1, 0.1, 0.1));
	//Mesh->InitBufferData();
	Mesh->Init();

	//Mesh->SetRotation(Rotation);
	//SetName("Default0");
	LightBuffer = std::make_shared<PixelBuffer>(LightData);

	Reset();
}

PointLight::PointLight(const std::string& name)
{
	Location = Vector(-35.0f, 160.0f, -50.0f);
	Mesh = std::make_shared<NewMesh>();
	Mesh->SetMesh("sphere.fbx");
	Mesh->SetMaterial("Sphere.fx");
	Mesh->SetLocation(Location);
	Mesh->SetScale3D(Vector(0.1, 0.1, 0.1));
	//Mesh->InitBufferData();
	Mesh->Init();
	SetName(name);

	LightBuffer = std::make_shared<PixelBuffer>(LightData);

	Reset();
}

PointLight::PointLight(const std::string& name, float radius /*= 0.5f*/)
{
	Location = Vector(-35.0f, 160.0f, -50.0f);
	Mesh = std::make_shared<NewMesh>();
	Mesh->SetMesh("sphere.fbx");
	Mesh->SetMaterial("Sphere.fx");
	Mesh->SetLocation(Location);
	Mesh->SetScale3D(Vector(0.1, 0.1, 0.1));
	//Mesh->InitBufferData();
	Mesh->Init();

	LightBuffer = std::make_shared<PixelBuffer>(LightData);

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
		ImGui::SliderAngle("Pitch", &Mesh->Rotation.Pitch, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Yaw", &Mesh->Rotation.Yaw, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Roll", &Mesh->Rotation.Roll, -180.0f, 180.0f, "%.1f");

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
	Mesh->SetLocation(Location);
	Mesh->Draw();
}

void PointLight::Bind(DX11RHI& gfx) const
{
	//constantBuffer.Update(Graphics::GetGraphics(), pcbDataCopy);
	//constantBuffer.Bind(Graphics::GetGraphics());
}

void PointLight::Tick(double deltaTime)
{
	Mesh->Update(deltaTime);
}

void PointLight::Render(double deltaTime)
{
	WObject::Render(deltaTime);

	LightData.pos = DirectX::XMFLOAT3(Location.x, Location.y, Location.z);

	auto pcbDataCopy = LightData;
	const auto pos = DirectX::XMLoadFloat3(&LightData.pos);
	DirectX::XMStoreFloat3(&pcbDataCopy.pos, DirectX::XMVector3Transform(pos, DX11RHI::Get().GetCamera()));

	LightBuffer->Update(pcbDataCopy);
	
	Draw();
	SpawnControlWindow();
}

DirectX::XMFLOAT3 PointLight::GetPosition() const
{
	return LightData.pos;
}
