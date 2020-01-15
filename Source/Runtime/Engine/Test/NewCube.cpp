#include "NewCube.h"
#include <Runtime/Core/Helpers/StringHelper.h>
#include "Material.h"
#include "Imgui/imgui.h"

NewCube::NewCube()
{
	SetMaterial("Sphere.fx");
	SetScale3D(Vector(1.0f, 1.0f, 1.0f));
	Init();
}

bool NewCube::Init()
{
	InputElement.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	InputElement.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	MeshData = Geometry::MakeCube();
	InitMaterial();
	InitBufferData();

	return true;
}

void NewCube::InitMaterial()
{
	Primitive::InitMaterial();
}

void NewCube::Update(double DeltaTime)
{
	Primitive::Update(DeltaTime);
}

void NewCube::Draw()
{
	Primitive::Draw();
}