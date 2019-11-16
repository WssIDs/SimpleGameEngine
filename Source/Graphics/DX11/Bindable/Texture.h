#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const class Surface& surface);
	virtual void Bind(Graphics& gfx) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
};

