#pragma once
#include "Bindable.h"
#include <string>
#include "Windows\Launch\Window\STypes.h"



class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const TSTRING& path);
	virtual void Bind(Graphics& gfx) override;
	ID3DBlob* GetByteCode() const;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCode;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
};

