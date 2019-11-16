#include "VertexShader.h"
#include <string>
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	D3DReadFileToBlob(path.c_str(), &m_pByteCode);
	GetDevice(gfx)->CreateVertexShader(m_pByteCode->GetBufferPointer(), m_pByteCode->GetBufferSize(), nullptr, &m_pVertexShader);
}

void VertexShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const
{
	return m_pByteCode.Get();
}
