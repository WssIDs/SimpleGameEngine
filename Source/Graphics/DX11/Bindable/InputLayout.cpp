#include "InputLayout.h"
#include "BindableCodex.h"


namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, DynamicVtx::VertexLayout layout_in, ID3DBlob* pVertexShaderByteCode)
		:
		layout(std::move(layout_in))
	{

		const auto d3dLayout = layout.GetD3DLayout();

		GetDevice(gfx)->CreateInputLayout(d3dLayout.data(),
			(UINT)d3dLayout.size(),
			pVertexShaderByteCode->GetBufferPointer(),
			pVertexShaderByteCode->GetBufferSize(),
			&pInputLayout);
	}

	void InputLayout::Bind(Graphics& gfx)
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx, const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode)
	{
		return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderByteCode);
	}

	std::string InputLayout::GenerateUID(const DynamicVtx::VertexLayout& layout, ID3DBlob* pVertexShaderByteCode /*= nullptr*/)
	{
		using namespace std::string_literals;
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}

	std::string InputLayout::GetUID() const
	{
		return GenerateUID(layout);
	}

}