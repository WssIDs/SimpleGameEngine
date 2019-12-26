#include "NewSphere.h"

NewSphere::NewSphere()
{
	SetMaterial("Sphere.fx");

	InitMaterial();

	InputElement.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	InputElement.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 12, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	MeshData = Geometry::MakeSphere(12, 24);

	SetScale3D(Vector(1.0f, 1.0f, 1.0f));
	Init();
}

void NewSphere::InitMaterial()
{
	Primitive::InitMaterial();
}

void NewSphere::Update(double DeltaTime)
{
	Primitive::Update(DeltaTime);
}

void NewSphere::Draw()
{
	Primitive::Draw();
}
