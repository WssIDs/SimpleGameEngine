#pragma once
#include "Drawable.h"
#include <random>
#include "Graphics/DX11/Bindable/IndexBuffer.h"


template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized()
	{
		return !staticBinds.empty();
	}
	static void AddStaticBind( std::unique_ptr<Bind::Bindable> bind)
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(Bind::IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer( std::unique_ptr<Bind::IndexBuffer> iBuf)
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		this->pIndexBuffer = iBuf.get();
		staticBinds.push_back(std::move(iBuf));
	}
	void SetIndexFromStatic()
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& sbind : staticBinds)
		{
			if (const auto pIndexBuffer = dynamic_cast<Bind::IndexBuffer*>(sbind.get()))
			{
				this->pIndexBuffer = pIndexBuffer;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const override
	{
		return staticBinds;
	}

	static std::vector<std::unique_ptr<Bind::Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bind::Bindable>> DrawableBase<T>::staticBinds;