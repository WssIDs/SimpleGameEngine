#pragma once
#include "Bindable.h"
#include <vector>
#include "..\Render\Vertex.h"


namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx, const DynamicVtx::VertexBuffer& vertexBuffer);
		void Bind(Graphics& gfx) override;

	protected:
		const UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}

