#pragma once
#include "Graphics/DX11/Graphics.h"
#include <vector>

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics& gfx) = 0;
		virtual ~Bindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx);
		static ID3D11Device* GetDevice(Graphics& gfx);
	};
}