#pragma once
#include "Bindable.h"
#include <vector>
#include "..\Render\Vertex.h"


namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer);
		VertexBuffer(Graphics& gfx, const DynamicVtx::VertexBuffer& vertexBuffer);
		void Bind(Graphics& gfx) override;

		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag, const DynamicVtx::VertexBuffer& vertexBuffer);
		
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

