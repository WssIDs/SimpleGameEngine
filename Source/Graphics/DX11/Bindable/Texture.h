#pragma once
#include "Bindable.h"
#include <string>

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		/* Create Texture
		 * bSurfaceLoading = false Loading DDS/TGA
		 * bSurfaceLoading = true - Surface
		 */
		Texture(Graphics& gfx, const std::string& fileName, UINT slot = 0);
		virtual void Bind(Graphics& gfx) override;
		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const override;
	private:
		unsigned int slot;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}