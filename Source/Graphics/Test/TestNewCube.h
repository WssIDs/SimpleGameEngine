#pragma once
#include <Graphics/Engine/Core.h>
#include "Graphics/DX11/Graphics.h"
#include <memory>

class Material;

struct CubeVertex
{
	CubeVertex() {}
	CubeVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		:
		pos(x, y, z),
		texCoord(u, v),
		normal(nx, ny, nz)
	{}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

struct Transforms
{
	DirectX::XMMATRIX ModelView;
	DirectX::XMMATRIX ModelViewProj;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	DirectX::XMFLOAT3 Direction;
	float Padding1;
	DirectX::XMFLOAT3 Position;
	float Range;
	DirectX::XMFLOAT3 Attenuation;
	float Padding2;
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
};

struct cbPerFrame
{
	Light  light;
};

struct Position
{
	Position() {};
	Position(float inX, float inY, float inZ)
		:
		x(inX), y(inY), z(inZ)
	{}

	float x;
	float y;
	float z;
};

class TestNewCube
{
public:
	TestNewCube();
	void Update(float DeltaTime);
	void Draw();

	Light light;

	Transforms constantBufferPerObj;
	cbPerFrame constantBufferPerFrame;

	//DirectX::XMMATRIX WorldViewProjection;
	//DirectX::XMMATRIX World;

	DirectX::XMMATRIX Rotation;
	DirectX::XMMATRIX Scale;
	DirectX::XMMATRIX Translation;
	float rot = 0.01f;


	std::vector<CubeVertex> VertexData;
	std::vector<unsigned int> IndicesData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferPerObject;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferPerFrame;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> VertexLayout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RastrState;

	std::shared_ptr<Material> MeshMaterial;

	Position position;
};

