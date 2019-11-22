#include "PixelShader.h"
#include <string>
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& gfx, const TSTRING& path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	std::wstring wpath(path.begin(), path.end());
	D3DReadFileToBlob(wpath.c_str(), &pBlob);
	GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
