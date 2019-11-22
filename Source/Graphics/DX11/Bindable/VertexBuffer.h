#pragma once
#include "Bindable.h"
#include <vector>
#include "..\Render\Vertex.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& verticles)
		: stride (sizeof(V))
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

	VertexBuffer(Graphics& gfx, const VL::VertexBuffer& vbuf)
		:
		stride((UINT)vbuf.GetLayout().Size())
	{

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};

		sd.pSysMem = vbuf.GetData();

		GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}

	void Bind(Graphics& gfx) override;

protected:
	const UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

