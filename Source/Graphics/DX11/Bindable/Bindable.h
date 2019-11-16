#pragma once
#include "Graphics/DX11/Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx);
	static ID3D11Device* GetDevice(Graphics& gfx);
};

