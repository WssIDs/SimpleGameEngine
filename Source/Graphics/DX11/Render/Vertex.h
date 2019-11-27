#pragma once
#include <vector>
#include <DirectXMath.h>
#include <type_traits>
#include "Color.h"
#include "Graphics/DX11/Graphics.h"

namespace DynamicVtx
{
	enum class ElementType : int
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
			using Type = ::RGBAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Color";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset);
			size_t GetOffsetAfter() const;
			size_t GetOffset() const;
			size_t Size() const;
			static constexpr size_t SizeOf(ElementType type);
			ElementType GetType() const;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const;

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

		const Element& ResolveByIndex(size_t i) const;
		VertexLayout& Append(ElementType type);
		size_t Size() const;
		size_t GetElementCount() const;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;

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
			return *reinterpret_cast<typename VertexLayout::ElemType<Type>::Type*>(pAttribute);
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
		Vertex(char* pData, const VertexLayout& layout);

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
		ConstantVertex(const Vertex& v);

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
		VertexBuffer(VertexLayout layout);

		const char* GetData() const;
		const VertexLayout& GetLayout() const;
		size_t Size() const;
		size_t SizeBytes() const;

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back();
		Vertex Front();
		Vertex operator[](size_t i);
		ConstantVertex Back() const;
		ConstantVertex Front() const;
		ConstantVertex operator[](size_t i) const;

	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};

}
