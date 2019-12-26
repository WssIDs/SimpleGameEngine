#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <Runtime/D3D11RHI/DX11/Render/Vertex.h>



namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx, DynamicVtx::VertexLayout layout, ID3DBlob* pVertexShaderByteCode);
		void Bind(Graphics& gfx) override;
		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode);
		static std::string GenerateUID(const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode = nullptr);
		std::string GetUID() const override;

	protected:
		DynamicVtx::VertexLayout layout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}