#pragma once
#include <Runtime/Core/Core.h>
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
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(DX11RHI& gfx) const;
	virtual void Update(float deltaSeconds) {}
	virtual ~Drawable() = default;

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
protected:
	void AddBind(std::shared_ptr<Bind::Bindable> bind);
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
};

