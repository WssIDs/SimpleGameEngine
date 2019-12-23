#pragma once
#include "Bindable.h"
#include <string>
#include <boost\serialization\vector.hpp>
#include "boost\serialization\access.hpp"
#include <boost\serialization\string.hpp>
#include "..\Render\Color.h"
#include "boost\serialization\array_wrapper.hpp"
#include <ThirdParty\DirectXTex\include\DirectXTex.h>

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
		Texture();
		Texture(Graphics& gfx, const std::string& fileName, UINT slot = 0);
		virtual void Bind(Graphics& gfx) override;
		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const override;

		void Serialize();

		void Deserialize(const std::string& filename);

		std::vector<RGBAColor> GetColors() const;

		bool HasAlpha() const;

	private:
		std::shared_ptr<DirectX::ScratchImage> Convert(std::shared_ptr<DirectX::ScratchImage> scratch);
	private:
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & name;
			ar & bAlpha;
			ar & path;
			ar & Colors;
		}

		std::vector<RGBAColor> Colors;

		unsigned int slot;
	protected:
		bool bAlpha = false;
		std::string path;
		std::string name;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}