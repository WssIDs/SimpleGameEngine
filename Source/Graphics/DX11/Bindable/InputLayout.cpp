#include "InputLayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
	GetDevice(gfx)->CreateInputLayout(layout.data(),
		(UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_pInputLayout);
}

void InputLayout::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}
