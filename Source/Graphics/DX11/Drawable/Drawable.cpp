#include "Drawable.h"
#include "Graphics/DX11/Bindable/IndexBuffer.h"
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& bind : binds)
	{
		bind->Bind(gfx);
	}
	for (auto& sbind : GetStaticBinds())
	{
		sbind->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf)
{
	pIndexBuffer = iBuf.get();
	binds.push_back(std::move(iBuf));
}
