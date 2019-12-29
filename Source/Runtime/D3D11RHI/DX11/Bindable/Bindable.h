#pragma once
#include <Runtime/Core/Core.h>
#include <vector>
#include <string>

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(DX11RHI& gfx) = 0;
		virtual std::string GetUID() const
		{
			assert(false);
			return "";
		}
		virtual ~Bindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext(DX11RHI& gfx);
		static ID3D11Device* GetDevice(DX11RHI& gfx);
	};
}