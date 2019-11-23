#pragma once
#include "Bindable.h"
#include <vector>
#include "..\Render\Vertex.h"


namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		template<class V>
		VertexBuffer(Graphics& gfx, const std::vector<V>& verticles)
			: stride(sizeof(V))
		{
			D3D11_BUFFER_DESC bd = {};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.ByteWidth = UINT(sizeof(V) * verticles.size());
			bd.StructureByteStride = sizeof(V);

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = verticles.data();

			GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
		}

		VertexBuffer(Graphics& gfx, const DynamicVtx::VertexBuffer& vertexBuffer);
		void Bind(Graphics& gfx) override;

	protected:
		const UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}

