#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(DX11RHI& gfx, const std::vector<unsigned short>& indices);
		IndexBuffer(DX11RHI& gfx, std::string tag, const std::vector<unsigned short>& indices);
		void Bind(DX11RHI& gfx) override;
		UINT GetCount() const;
		static std::shared_ptr<IndexBuffer> Resolve(DX11RHI& gfx, const std::string& tag,
			const std::vector<unsigned short>& indices);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string tag;
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
