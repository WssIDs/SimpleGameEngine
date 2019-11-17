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
		return !m_staticBinds.empty();
	}
	static void AddStaticBind( std::unique_ptr<Bindable> bind)
	{
		m_staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> iBuf)
	{
		m_pIndexBuffer = iBuf.get();
		m_staticBinds.push_back(std::move(iBuf));
	}
	void SetIndexFromStatic()
	{
		for (const auto& sbind : m_staticBinds)
		{
			if (const auto pIndexBuffer = dynamic_cast<IndexBuffer*>(sbind.get()))
			{
				m_pIndexBuffer = pIndexBuffer;
				return;
			}
		}
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override
	{
		return m_staticBinds;
	}

	static std::vector<std::unique_ptr<Bindable>> m_staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::m_staticBinds;