#include "VertexBuffer.h"


namespace Bind
{
	VertexBuffer::VertexBuffer(Graphics& gfx, const DynamicVtx::VertexBuffer& vertexBuffer)
		:
		stride((UINT)vertexBuffer.GetLayout().Size())
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vertexBuffer.SizeBytes());
		bd.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertexBuffer.GetData();

		GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}

	void VertexBuffer::Bind(Graphics& gfx)
	{
		const UINT offset = 0u;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
}