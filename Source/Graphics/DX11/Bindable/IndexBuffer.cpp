#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
	: m_count ( (UINT)indices.size())
{
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(m_count * sizeof(unsigned short)); // !!! Very important
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	isd.SysMemPitch = 0;
	isd.SysMemSlicePitch = 0;
	
	GetDevice(gfx)->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
}

void IndexBuffer::Bind(Graphics& gfx)
{
	// bind index buffer
	GetContext(gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const
{
	return m_count;
}
