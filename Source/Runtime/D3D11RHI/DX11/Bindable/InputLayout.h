#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <Runtime/D3D11RHI/DX11/Render/Vertex.h>



namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(DX11RHI& gfx, DynamicVtx::VertexLayout layout, ID3DBlob* pVertexShaderByteCode);
		void Bind(DX11RHI& gfx) override;
		static std::shared_ptr<InputLayout> Resolve(DX11RHI& gfx, const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode);
		static std::string GenerateUID(const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode = nullptr);
		std::string GetUID() const override;

	protected:
		DynamicVtx::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}