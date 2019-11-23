#include "VertexShader.h"
#include <string>
#include <d3dcompiler.h>

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const TSTRING& path)
	{
		std::wstring wpath(path.begin(), path.end());
		D3DReadFileToBlob(wpath.c_str(), &pByteCode);
		GetDevice(gfx)->CreateVertexShader(pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(), nullptr, &pVertexShader);
	}

	void VertexShader::Bind(Graphics& gfx)
	{
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetByteCode() const
	{
		return pByteCode.Get();
	}
}
