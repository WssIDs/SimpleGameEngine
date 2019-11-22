#pragma once
#include "Bindable.h"
#include <vector>



class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode);
	void Bind(Graphics& gfx) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

