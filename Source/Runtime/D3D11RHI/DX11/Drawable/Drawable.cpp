#include "Drawable.h"
#include <cassert>
#include <Runtime/D3D11RHI/DX11/Bindable/IndexBuffer.h>

using namespace Bind;

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& bind : binds)
	{
		bind->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind)
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back(std::move(bind));
}