#pragma once
#include <vector>
#include <DirectXMath.h>


template<class T>
class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		:
		m_vertices( std::move(verts_in)),
		m_indices( std::move(indices_in))
	{
		assert(m_vertices.size() > 2);
		assert(m_indices.size() % 3 == 0);
	}

	void Transform( DirectX::FXMMATRIX matrix)
	{
		for (auto& vertex : m_vertices )
		{
			const DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&vertex.pos);
			DirectX::XMStoreFloat3(&vertex.pos, DirectX::XMVector3Transform(position, matrix));
		}
	}

public:
	std::vector<T> m_vertices;
	std::vector<unsigned short> m_indices;
};