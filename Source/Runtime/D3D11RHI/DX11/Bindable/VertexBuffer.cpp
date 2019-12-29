#include "VertexBuffer.h"
#include <Runtime/D3D11RHI/DX11/Bindable/BindableCodex.h>


namespace Bind
{
	VertexBuffer::VertexBuffer(DX11RHI& gfx, const DynamicVtx::VertexBuffer& vertexBuffer)
		:
		VertexBuffer(gfx, "?", vertexBuffer)
	{}

	VertexBuffer::VertexBuffer(DX11RHI& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer)
		:
		stride((UINT)vertexBuffer.GetLayout().Size()),
		tag(tag)
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

	void VertexBuffer::Bind(DX11RHI& gfx)
	{
		const UINT offset = 0u;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(DX11RHI& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(gfx, tag, vertexBuffer);
	}

	std::string VertexBuffer::GetUID() const
	{
		return GenerateUID(tag);
	}

	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}

}