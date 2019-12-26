#pragma once
#include <Runtime/Core/Core.h>
#include <vector>
#include <string>

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics& gfx) = 0;
		virtual std::string GetUID() const
		{
			assert(false);
			return "";
		}
		virtual ~Bindable() = default;

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx);
		static ID3D11Device* GetDevice(Graphics& gfx);
	};
}