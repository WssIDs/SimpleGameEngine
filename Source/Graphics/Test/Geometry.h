#pragma once
#include <DirectXMath.h>
#include "Graphics/DX11/Math/WGMath.h"
#include <vector>
#include "../DX11/Render/Color.h"

struct MainVertex
{
	MainVertex(float inX, float inY, float inZ, float inRed = 1.0f, float inGreen = 1.0f, float inBlue = 1.0f, float inAlpha = 1.0f)
		:
		Position(inX, inY, inZ),
		Color(inRed, inGreen, inBlue, inAlpha)
	{}

	MainVertex(DirectX::XMFLOAT3 inPosition, LinearColor inColor = LinearColor::White)
		:
		Position(inPosition),
		Color(inColor)
	{}

	DirectX::XMFLOAT3 Position;
	LinearColor Color;
};

struct MeshData
{
	MeshData() {};

	MeshData(std::vector<MainVertex> inVertices, std::vector<unsigned int> inIndices)
		:
		Vertices(inVertices),
		Indices(inIndices)
	{
	}

	std::vector<MainVertex> Vertices;
	std::vector<unsigned int> Indices;
};

class Geometry
{
public:

	static MeshData MakeSphere(int latDiv, int longDiv)
	{
		//assert(latDiv >= 3);
		//assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);

		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<MainVertex> vertices;

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationX(lattitudeAngle * iLat)
			);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				DirectX::XMFLOAT3 calculatedPos;
				auto v = DirectX::XMVector3Transform(
					latBase,
					DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				DirectX::XMStoreFloat3(&calculatedPos, v);
				vertices.emplace_back(calculatedPos);
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned int)vertices.size();
		{
			DirectX::XMFLOAT3 northPos;
			DirectX::XMStoreFloat3(&northPos, base);
			vertices.emplace_back(northPos);
		}

		const auto iSouthPole = (unsigned int)vertices.size();
		{
			DirectX::XMFLOAT3 southPos;
			DirectX::XMStoreFloat3(&southPos, DirectX::XMVectorNegate(base));
			vertices.emplace_back(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](unsigned int iLat, unsigned int iLong)
		{
			return iLat * longDiv + iLong;
		};

		std::vector<unsigned int> indices;

		for (unsigned int iLat = 0; iLat < (unsigned int)latDiv - 2; iLat++)
		{
			for (unsigned int iLong = 0; iLong < (unsigned int)longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (unsigned int iLong = 0; iLong < (unsigned int)longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		//SphereData data(vertices, indices);

		return MeshData(vertices, indices);
	}

	static MeshData MakeCube()
	{
		// Front Face
		std::vector<MainVertex> vertices = {
			// Front Face
			{-1.0f, -1.0f, -1.0f/*, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f*/},
			{-1.0f,  1.0f, -1.0f/*, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f*/},
			{1.0f,  1.0f, -1.0f/*, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f*/},
			{1.0f, -1.0f, -1.0f/*, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f*/},

			// Back Face
			{-1.0f, -1.0f, 1.0f/*, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f*/},
			{1.0f, -1.0f, 1.0f/*, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f*/},
			{1.0f,  1.0f, 1.0f/*, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f*/},
			{-1.0f,  1.0f, 1.0f/*, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f*/},

			// Top Face
			{-1.0f, 1.0f, -1.0f/*, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f*/},
			{-1.0f, 1.0f,  1.0f/*, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f*/},
			{1.0f, 1.0f,  1.0f/*, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f*/},
			{1.0f, 1.0f, -1.0f/*, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f*/},

			// Bottom Face
			{-1.0f, -1.0f, -1.0f/*, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f*/},
			{1.0f, -1.0f, -1.0f/*, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f*/},
			{1.0f, -1.0f,  1.0f/*, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f*/},
			{-1.0f, -1.0f,  1.0f/*, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f*/},

			// Left Face
			{-1.0f, -1.0f,  1.0f/*, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f*/},
			{-1.0f,  1.0f,  1.0f/*, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f*/},
			{-1.0f,  1.0f, -1.0f/*, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f*/},
			{-1.0f, -1.0f, -1.0f/*, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f*/},

			// Right Face
			{1.0f, -1.0f, -1.0f/*, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f*/},
			{1.0f,  1.0f, -1.0f/*, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f*/},
			{1.0f,  1.0f,  1.0f/*, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f*/},
			{1.0f, -1.0f,  1.0f/*, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f*/},
		};

		// Front Face
		std::vector<unsigned int> indices =
		{
			// Front Face
			0,  1,  2,
			0,  2,  3,

			// Back Face
			4,  5,  6,
			4,  6,  7,

			// Top Face
			8,  9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		return  MeshData(vertices, indices);
	}
};