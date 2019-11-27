#pragma once
#include "Graphics/DX11/Graphics.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

class Drawable
{
	//template<class T>
	//friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics& gfx) const;
	virtual void Update(float deltaSeconds) {}
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable()
	{
		for (auto& pb : binds)
		{
			if(auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
	void AddBind(std::shared_ptr<Bind::Bindable> bind);
	//void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> iBuf);
private:
	//virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const = 0;
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
};

