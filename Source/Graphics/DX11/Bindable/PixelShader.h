#pragma once
#include "Bindable.h"
#include <string>
#include "Windows\Launch\Window\STypes.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const TSTRING& path);
	virtual void Bind(Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};

