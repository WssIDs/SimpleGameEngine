#pragma once
#include <Runtime/D3D11RHI/DX11/Bindable/Bindable.h>
#include <vector>
#include <Runtime/D3D11RHI/DX11/Render/Vertex.h>


namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(DX11RHI& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer);
		VertexBuffer(DX11RHI& gfx, const DynamicVtx::VertexBuffer& vertexBuffer);
		void Bind(DX11RHI& gfx) override;

		static std::shared_ptr<VertexBuffer> Resolve(DX11RHI& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer);
		
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const override;

	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string tag;
		const UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}

