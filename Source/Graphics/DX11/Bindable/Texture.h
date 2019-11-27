#pragma once
#include "Bindable.h"
#include <string>

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const class Surface& surface, unsigned int slot = 0);
		Texture(Graphics& gfx, const std::string& fileName, unsigned int slot = 0);
		virtual void Bind(Graphics& gfx) override;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}