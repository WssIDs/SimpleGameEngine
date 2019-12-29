#include "Bindable.h"


namespace Bind
{
	ID3D11DeviceContext* Bindable::GetContext(DX11RHI& gfx)
	{
		return gfx.pDeviceContext3D.Get();
	}

	ID3D11Device* Bindable::GetDevice(DX11RHI& gfx)
	{
		return gfx.pDevice3D.Get();
	}
}