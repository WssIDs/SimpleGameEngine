#pragma once
#include "Graphics/DX11/Graphics.h"
#include "TestNewCube.h"
#include "Material.h"
#include "Primitive.h"

class TestNewSphere
{
public:
	TestNewSphere();
	void Update(float DeltaTime);
	void Draw();

	Transforms constantBufferPerObj;

	SphereData sphereData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferPerObject;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBufferPerFrame;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> VertexLayout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RastrState;

	std::shared_ptr<Material> MeshMaterial;

	Position position;
};

