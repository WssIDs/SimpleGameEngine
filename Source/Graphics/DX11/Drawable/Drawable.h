#pragma once
#include "Graphics/DX11/Graphics.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics& gfx) const;
	virtual void Update(float deltaSeconds) = 0;
	virtual ~Drawable() = default;
protected:
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> iBuf);
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;
	const class IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};

