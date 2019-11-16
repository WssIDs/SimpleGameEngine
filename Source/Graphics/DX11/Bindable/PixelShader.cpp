#include "PixelShader.h"
#include <string>
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	D3DReadFileToBlob(path.c_str(), &pBlob);
	GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
}

void PixelShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
