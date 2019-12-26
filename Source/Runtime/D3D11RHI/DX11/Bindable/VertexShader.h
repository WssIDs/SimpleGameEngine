#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <string>
#include <memory>


namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::string& path);
		virtual void Bind(Graphics& gfx) override;
		ID3DBlob* GetByteCode() const;

		static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& path);
		static std::string GenerateUID(const std::string& path);
		std::string GetUID() const override;

	private:
		std::string path;
		Microsoft::WRL::ComPtr<ID3DBlob> pByteCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}
