#include "TestNewSphere.h"
#include "TestNewCube.h"

TestNewSphere::TestNewSphere()
{
	SetMaterial("Sphere.fx");

	InitMaterial();

	InputElement.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	MeshData = Geometry::MakeSphere(12, 24);

	SetScale3D(Vector(1.0f, 1.0f, 1.0f));
	Init();
}

void TestNewSphere::InitMaterial()
{
	Primitive::InitMaterial();
}

void TestNewSphere::Update(double DeltaTime)
{
	Primitive::Update(DeltaTime);
}

void TestNewSphere::Draw()
{
	Primitive::Draw();
}
