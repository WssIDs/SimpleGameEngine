#include "Drawable.h"
#include "Graphics/DX11/Bindable/IndexBuffer.h"
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& bind : m_binds)
	{
		bind->Bind(gfx);
	}
	for (auto& sbind : GetStaticBinds())
	{
		sbind->Bind(gfx);
	}
	gfx.DrawIndexed(m_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	m_binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf)
{
	m_pIndexBuffer = iBuf.get();
	m_binds.push_back(std::move(iBuf));
}
