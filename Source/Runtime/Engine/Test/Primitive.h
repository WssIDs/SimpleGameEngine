#pragma once
#include <Runtime/Core/Core.h>
#include <string>
#include "Geometry.h"
#include "Material.h"
#include "Buffer.h"

struct Vector
{
	Vector()
	{}

	Vector(float inX, float inY, float inZ)
		:
		x(inX),
		y(inY),
		z(inZ)
	{}

	static Vector IdentityVector()
	{
		return Vector(1.0f, 1.0f, 1.0f);
	}

	static Vector ZeroVector()
	{
		return Vector(0.0f, 0.0f, 0.0f);
	}

	friend Vector operator+(const Vector& leftVector, const Vector& rightVector)
	{
		Vector result = Vector(leftVector.x + rightVector.x, leftVector.y + rightVector.y, leftVector.z + rightVector.z);
		return result;
	}

	float x;
	float y;
	float z;
};

struct Rotator
{
	Rotator()
	{}

	Rotator(float inPitch, float inYaw, float inRoll)
		:
		Pitch(inPitch),
		Roll(inRoll),
		Yaw(inYaw)
	{}

	static Rotator IdentityRotator()
	{
		return Rotator(1.0f, 1.0f, 1.0f);
	}

	static Rotator ZeroRotator()
	{
		Vector(0.0f, 0.0f, 0.0f);
	}

	float Pitch;
	float Roll;
	float Yaw;
};


struct ColorConstant
{
	DirectX::XMFLOAT3 Color = { 1.0f,1.0f,1.0f };
	float padding;
};

struct ConstantTransform
{
	//DirectX::XMMATRIX ModelWorld;
	DirectX::XMMATRIX ModelView;
	DirectX::XMMATRIX ModelViewProj;
};

class Primitive
{
public:
	Primitive();

	void SetName(std::string Name);
	virtual void InitMaterial();
	// set shader Name
	void SetMaterial(std::string Name);
	void Init();

	virtual void Update(double DeltaTime);
	virtual void Draw();

	void SetScale3D(Vector inScale);
	void SetLocation(Vector newLocation);
	void SetRotation(Rotator newRotation);
	Vector GetLocation() const;
	Rotator GetRotation() const;
	Vector GetScale3D() const;

	MeshData MeshData;

	ColorConstant ConstantBufferColor;
	std::shared_ptr<PixelBuffer> ColorConstantBuffer;
	std::shared_ptr<VertexBuffer> TransformConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> VertexLayout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RastrState;

	std::shared_ptr<Material> MeshMaterial;

	Rotator Rotation;
	Vector Location;
	Vector Scale;

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputElement;

private:
	std::string Name = "Primitive";
	std::string ShaderName;
	ConstantTransform ConstantBufferTransformPerObj;

	//DirectX::XMMATRIX ModelWorld;
	DirectX::XMMATRIX ModelView;
	DirectX::XMMATRIX ModelViewProj;
};

