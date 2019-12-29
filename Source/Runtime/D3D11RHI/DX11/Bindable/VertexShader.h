#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <string>
#include <memory>


namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(DX11RHI& gfx, const std::string& path);
		virtual void Bind(DX11RHI& gfx) override;
		ID3DBlob* GetByteCode() const;

		static std::shared_ptr<VertexShader> Resolve(DX11RHI& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const override;

	private:
		std::string path;
		Microsoft::WRL::ComPtr<ID3DBlob> pByteCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}
