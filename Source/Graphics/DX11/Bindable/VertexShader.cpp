#include "VertexShader.h"
#include <string>
#include <d3dcompiler.h>
#include "BindableCodex.h"

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
	{
		D3DReadFileToBlob(std::wstring(path.begin(), path.end()).c_str(), &pByteCode);
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

	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<VertexShader>(gfx, path);
	}

	std::string VertexShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}

	std::string VertexShader::GetUID() const
	{
		return GenerateUID(path);
	}

}
