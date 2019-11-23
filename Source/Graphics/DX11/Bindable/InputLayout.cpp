#include "InputLayout.h"


namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
	{
		GetDevice(gfx)->CreateInputLayout(layout.data(),
			(UINT)layout.size(),
			pVertexShaderByteCode->GetBufferPointer(),
			pVertexShaderByteCode->GetBufferSize(),
			&pInputLayout);
	}

	void InputLayout::Bind(Graphics& gfx)
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}