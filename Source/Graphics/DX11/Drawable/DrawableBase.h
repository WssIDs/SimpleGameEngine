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
	static void AddStaticBind( std::unique_ptr<Bindable> bind)
	{
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> iBuf)
	{
		this->pIndexBuffer = iBuf.get();
		staticBinds.push_back(std::move(iBuf));
	}
	void SetIndexFromStatic()
	{
		for (const auto& sbind : staticBinds)
		{
			if (const auto pIndexBuffer = dynamic_cast<IndexBuffer*>(sbind.get()))
			{
				this->pIndexBuffer = pIndexBuffer;
				return;
			}
		}
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override
	{
		return staticBinds;
	}

	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;