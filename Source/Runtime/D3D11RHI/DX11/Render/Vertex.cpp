#include "Vertex.h"


namespace DynamicVtx
{
	// VertexLayout
	const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const
	{
		return elements[i];
	}

	VertexLayout& VertexLayout::Append(ElementType type)
	{
		elements.emplace_back(type, Size());
		return *this;
	}

	size_t VertexLayout::Size() const
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}

	size_t VertexLayout::GetElementCount() const
	{
		return elements.size();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());
		for (const auto& e : elements)
		{
			desc.push_back(e.GetDesc());
		}
		return desc;
	}

	std::string VertexLayout::GetCode() const
	{
		std::string code;
		for (const auto& e : elements)
		{
			code += e.GetCode();
		}
		return code;
	}

	// VertexLayout::Element
	VertexLayout::Element::Element(ElementType type, size_t offset)
		:
		type(type),
		offset(offset)
	{}

	size_t VertexLayout::Element::GetOffsetAfter() const
	{
		return offset + Size();
	}

	size_t VertexLayout::Element::GetOffset() const
	{
		return offset;
	}

	size_t VertexLayout::Element::Size() const
	{
		return SizeOf(type);
	}

	constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
	{
		switch (type)
		{
		case ElementType::Position2D:
			return sizeof(ElemType<ElementType::Position2D>::Type);
		case ElementType::Position3D:
			return sizeof(ElemType<ElementType::Position3D>::Type);
		case ElementType::Texture2D:
			return sizeof(ElemType<ElementType::Texture2D>::Type);
		case ElementType::Normal:
			return sizeof(ElemType<ElementType::Normal>::Type);
		case ElementType::Tangent:
			return sizeof(ElemType<ElementType::Tangent>::Type);
		case ElementType::Bitangent:
			return sizeof(ElemType<ElementType::Bitangent>::Type);
		case ElementType::Float3Color:
			return sizeof(ElemType<ElementType::Float3Color>::Type);
		case ElementType::Float4Color:
			return sizeof(ElemType<ElementType::Float4Color>::Type);
		case ElementType::RGBAColor:
			return sizeof(ElemType<ElementType::RGBAColor>::Type);
		}
		assert("Invalid element type" && false);
		return 0u;
	}

	ElementType VertexLayout::Element::GetType() const
	{
		return type;
	}

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
	{
		switch (type)
		{
		case ElementType::Position2D:
			return GenerateDesc<ElementType::Position2D>(GetOffset());
		case ElementType::Position3D:
			return GenerateDesc<ElementType::Position3D>(GetOffset());
		case ElementType::Texture2D:
			return GenerateDesc<ElementType::Texture2D>(GetOffset());
		case ElementType::Normal:
			return GenerateDesc<ElementType::Normal>(GetOffset());
		case ElementType::Tangent:
			return GenerateDesc<ElementType::Tangent>(GetOffset());
		case ElementType::Bitangent:
			return GenerateDesc<ElementType::Bitangent>(GetOffset());
		case ElementType::Float3Color:
			return GenerateDesc<ElementType::Float3Color>(GetOffset());
		case ElementType::Float4Color:
			return GenerateDesc<ElementType::Float4Color>(GetOffset());
		case ElementType::RGBAColor:
			return GenerateDesc<ElementType::RGBAColor>(GetOffset());
		}
		assert("Invalid element type" && false);
		return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
	}



	const char* VertexLayout::Element::GetCode() const
	{
		switch (type)
		{
		case ElementType::Position2D:
			return ElemType<ElementType::Position2D>::code;
		case ElementType::Position3D:
			return ElemType<ElementType::Position3D>::code;
		case ElementType::Texture2D:
			return ElemType<ElementType::Texture2D>::code;
		case ElementType::Normal:
			return ElemType<ElementType::Normal>::code;
		case ElementType::Tangent:
			return ElemType<ElementType::Tangent>::code;
		case ElementType::Bitangent:
			return ElemType<ElementType::Bitangent>::code;
		case ElementType::Float3Color:
			return ElemType<ElementType::Float3Color>::code;
		case ElementType::Float4Color:
			return ElemType<ElementType::Float4Color>::code;
		case ElementType::RGBAColor:
			return ElemType<ElementType::RGBAColor>::code;
		}

		assert("Invalid element type" && false);
		return "Invalid";
	}

	// Vertex
	Vertex::Vertex(char* pData, const VertexLayout& layout)
		:
		pData(pData),
		layout(layout)
	{
		assert(pData != nullptr);
	}

	// ConstantVertex
	ConstantVertex::ConstantVertex(const Vertex& v)
		:
		vertex(v)
	{}

	// VertexBuffer
	VertexBuffer::VertexBuffer(VertexLayout layout, size_t size)
		:
		layout(std::move(layout))
	{
		Resize(size);
	}

	const char* VertexBuffer::GetData() const
	{
		return buffer.data();
	}

	const VertexLayout& VertexBuffer::GetLayout() const
	{
		return layout;
	}

	void VertexBuffer::Resize(size_t newSize)
	{
		const auto size = Size();
		if(size < newSize)
		{
			buffer.resize(buffer.size() + layout.Size() * (newSize - size));
		}
	}

	size_t VertexBuffer::Size() const
	{
		return buffer.size() / layout.Size();
	}

	size_t VertexBuffer::SizeBytes() const
	{
		return buffer.size();
	}


	Vertex VertexBuffer::Back()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}


	Vertex VertexBuffer::Front()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(),layout };
	}

	Vertex VertexBuffer::operator[](size_t i)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}

	ConstantVertex VertexBuffer::Back() const
	{
		return const_cast<VertexBuffer*>(this)->Back();
	}

	ConstantVertex VertexBuffer::Front() const
	{
		return const_cast<VertexBuffer*>(this)->Front();
	}

	ConstantVertex VertexBuffer::operator[](size_t i) const
	{
		return const_cast<VertexBuffer&>(*this)[i];
	}
}