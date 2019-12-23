#pragma once
#include <DirectXMath.h>
#include "Graphics/DX11/Math/WGMath.h"
#include <vector>

struct SphereVertex
{
	SphereVertex(float inX, float inY, float inZ)
		:
		Position(inX, inY, inZ)
	{}

	SphereVertex(DirectX::XMFLOAT3 pos)
		:
		Position(pos)
	{}

	DirectX::XMFLOAT3 Position;
};

struct SphereData
{
	SphereData() {};

	SphereData(std::vector<DirectX::XMFLOAT3> inVertices, std::vector<unsigned int> inIndices)
		:
		Vertices(inVertices),
		Indices(inIndices)
	{
	}

	std::vector<DirectX::XMFLOAT3> Vertices;
	std::vector<unsigned int> Indices;
};

class Primitive
{
public:

	static SphereData MakeSphere(int latDiv, int longDiv)
	{
		//assert(latDiv >= 3);
		//assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);

		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<DirectX::XMFLOAT3> vertices;

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

		return { vertices, indices };
	}
};