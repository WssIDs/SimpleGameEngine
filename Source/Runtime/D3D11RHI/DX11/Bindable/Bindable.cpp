#include "Bindable.h"


namespace Bind
{
	ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx)
	{
		return gfx.pDeviceContext3D.Get();
	}

	ID3D11Device* Bindable::GetDevice(Graphics& gfx)
	{
		return gfx.pDevice3D.Get();
	}
}