#pragma once
#include <Graphics/Engine/Core.h>
#include <memory>
#include "Material.h"
#include "Geometry.h"
#include "Primitive.h"

//struct CubeVertex
//{
//	CubeVertex() {}
//	CubeVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
//		:
//		pos(x, y, z),
//		texCoord(u, v),
//		normal(nx, ny, nz)
//	{}
//
//	DirectX::XMFLOAT3 pos;
//	DirectX::XMFLOAT2 texCoord;
//	DirectX::XMFLOAT3 normal;
//};



//struct Light
//{
//	Light()
//	{
//		ZeroMemory(this, sizeof(Light));
//	}
//
//	DirectX::XMFLOAT3 Direction;
//	float Padding1;
//	DirectX::XMFLOAT3 Position;
//	float Range;
//	DirectX::XMFLOAT3 Attenuation;
//	float Padding2;
//	DirectX::XMFLOAT4 Ambient;
//	DirectX::XMFLOAT4 Diffuse;
//};
//
//struct cbPerFrame
//{
//	Light  light;
//};

//struct Position
//{
//	Position() {};
//	Position(float inX, float inY, float inZ)
//		:
//		x(inX), y(inY), z(inZ)
//	{}
//
//	float x;
//	float y;
//	float z;
//};

class NewCube : public Primitive
{
public:
	NewCube();

	virtual void InitMaterial() override;
	virtual void Update(double DeltaTime) override;
	virtual void Draw() override;
};

