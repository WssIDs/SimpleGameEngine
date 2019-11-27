#include "Drawable.h"
#include "Graphics/DX11/Bindable/IndexBuffer.h"
#include <cassert>

using namespace Bind;

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& bind : binds)
	{
		bind->Bind(gfx);
	}
	//for (auto& sbind : GetStaticBinds())
	//{
	//	sbind->Bind(gfx);
	//}
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

//void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf)
//{
//	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
//	pIndexBuffer = iBuf.get();
//	binds.push_back(std::move(iBuf));
//}
