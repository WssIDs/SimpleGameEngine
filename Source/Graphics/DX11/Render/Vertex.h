#pragma once
#include <vector>
#include <DirectXMath.h>
#include <type_traits>
#include "Graphics/DX11/Graphics.h"

namespace VL
{

	struct RGBAColor
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
	};

	enum class ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		RGBAColor,
		Count,
	};


	class VertexLayout
	{
	public:

		template<ElementType> struct ElemType;
		template<> struct ElemType<ElementType::Position2D>
		{
			using Type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};

		template<> struct ElemType<ElementType::Position3D>
		{
			using Type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};

		template<> struct ElemType<ElementType::Texture2D>
		{
			using Type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
		};

		template<> struct ElemType<ElementType::Normal>
		{
			using Type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};

		template<> struct ElemType<ElementType::Float3Color>
		{
			using Type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};

		template<> struct ElemType<ElementType::Float4Color>
		{
			using Type = DirectX::XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};

		template<> struct ElemType<ElementType::RGBAColor>
		{
			using Type = VL::RGBAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				:
				type(type),
				offset(offset)
			{}

			size_t GetOffsetAfter() const
			{
				return offset + Size();
			}

			size_t GetOffset() const
			{
				return offset;
			}

			size_t Size() const
			{
				return SizeOf(type);
			}

			static constexpr size_t SizeOf(ElementType type)
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

			ElementType GetType() const
			{
				return type;
			}

			D3D11_INPUT_ELEMENT_DESC GetDesc() const
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

		private:
			template<ElementType type>
			static D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset)
			{
				return { ElemType<type>::semantic,0,ElemType<type>::dxgiFormat,0,(UINT)offset, D3D11_INPUT_PER_VERTEX_DATA,0 };
			}

		private:
			ElementType type;
			size_t offset;
		};

	public:
		template<ElementType Type>
		const Element& Resolve() const
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}

		const Element& ResolveByIndex(size_t i) const
		{
			return elements[i];
		}

		VertexLayout& Append(ElementType type)
		{
			elements.emplace_back(type, Size());
			return *this;
		}

		size_t Size() const
		{
			return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}

		size_t GetElementCount() const
		{
			return elements.size();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(GetElementCount());
			for (const auto& e : elements)
			{
				desc.push_back(e.GetDesc());
			}
			return desc;
		}

	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		template<ElementType Type>
		auto& Attr()
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename ElemType<Type>::Type*>(pAttribute);
		}
		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			using namespace DirectX;
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.GetType())
			{
			case ElementType::Position2D:
				SetAttribute<ElementType::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::Position3D:
				SetAttribute<ElementType::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::Texture2D:
				SetAttribute<ElementType::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::Normal:
				SetAttribute<ElementType::Normal>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::Float3Color:
				SetAttribute<ElementType::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::Float4Color:
				SetAttribute<ElementType::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case ElementType::RGBAColor:
				SetAttribute<ElementType::RGBAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

protected:
		Vertex(char* pData, const VertexLayout& layout)
			:
			pData(pData),
			layout(layout)
		{
			assert(pData != nullptr);
		}

	private:

		template<typename First, typename ...Rest>
		// enables parameter pack setting of multiple parameters by element index
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		// helper to reduce code duplication in SetAttributeByIndex
		template<ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val)
		{
			using Dest = typename VertexLayout::ElemType<DestLayoutType>::Type;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};


	class ConstantVertex
	{
	public:
		ConstantVertex(const Vertex& v)
			:
			vertex(v)
		{}

		template<ElementType Type>
		const auto& Attr() const
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}

	private:
		Vertex vertex;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout)
			:
			layout(std::move(layout))
		{}

		const char* GetData() const
		{
			return buffer.data();
		}
		const VertexLayout& GetLayout() const
		{
			return layout;
		}

		size_t Size() const
		{
			return buffer.size() / layout.Size();
		}

		size_t SizeBytes() const
		{
			return buffer.size();
		}

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back()
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
		}

		Vertex Front()
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data(),layout };
		}

		Vertex operator[](size_t i)
		{
			assert(i < Size());
			return Vertex{ buffer.data() + layout.Size() * i,layout };
		}

		ConstantVertex Back() const
		{
			return const_cast<VertexBuffer*>(this)->Back();
		}

		ConstantVertex Front() const
		{
			return const_cast<VertexBuffer*>(this)->Front();
		}

		ConstantVertex operator[](size_t i) const
		{
			return const_cast<VertexBuffer&>(*this)[i];
		}

	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};

}
