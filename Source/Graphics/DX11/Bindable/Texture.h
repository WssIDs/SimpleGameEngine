#pragma once
#include "Bindable.h"
#include <string>

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const class Surface& surface);
		Texture(Graphics& gfx, const std::string& fileName);
		virtual void Bind(Graphics& gfx) override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}