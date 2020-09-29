#pragma once

#include <vector>
#include <utility>
#include <DirectXMath.h>

class VertexLayout
{
public:
	enum ElementType
	{
		Position2D,
		Position3D,
		Normal,
		Texture2D,
		Count
	};
	template<ElementType> struct Map;
	template<> struct Map<ElementType::Position2D>
	{
		using sysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* lable = "Position";
	};
	template<> struct Map<ElementType::Position3D>
	{
		using sysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* lable = "Position";
	};
	template<> struct Map<ElementType::Normal>
	{
		using sysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* lable = "Normal";
	};
	template<> struct Map<ElementType::Texture2D>
	{
		using sysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* lable = "TexCoord";
	};
	class Element
	{
	public:
		Element( const ElementType& ET,size_t offset ) noexcept
			:
			offset( offset ),
			ET( ET )
		{}
		size_t GetOffsetNext() const noexcept(!IS_DEBUG)
		{
			return Size() + offset;
		}
		size_t Size() const noexcept(!IS_DEBUG)
		{
			return SizeOf( ET );
		}
		size_t GetOffset() const noexcept
		{
			return offset;
		}
		static constexpr size_t SizeOf( ElementType et ) noexcept(!IS_DEBUG)
		{
			switch ( et )
			{
			case Position2D:
				return sizeof( Map<Position2D>::sysType );
			case Position3D:
				return sizeof( Map<Position3D>::sysType );
			case Normal:
				return sizeof( Map<Normal>::sysType );
			case Texture2D:
				return sizeof( Map<Texture2D>::sysType );
			default:
				assert( false && "Invalid Template Type" );
				return -1;
			}
		}
		const ElementType& GetType() const noexcept
		{
			return ET;
		}
	private:
		ElementType ET;
		size_t offset;
	};
public:
	VertexLayout& Add( const ElementType& ET ) noexcept(!IS_DEBUG)
	{
		elements.emplace_back( ET,GetOffset() );
		size += Element::SizeOf( ET );
		return *this;
	}
	size_t LayoutSize() const noexcept
	{
		return size;
	}
	const Element& QuerraElements( ElementType et ) const noexcept(!IS_DEBUG)
	{
		for ( const auto& e : elements )
		{
			if ( e.GetType() == et )
			{
				return e;
			}
		}
		assert( false && "Invalid Template Argument" );
		return elements[0];
	}
	const Element& ByIndex( size_t index ) const noexcept
	{
		return elements[index];
	}
private:
	size_t GetOffset() noexcept(!IS_DEBUG)
	{
		return elements.empty() ? 0u : elements.back().GetOffsetNext();
	}
private:
	size_t size = 0;
	std::vector<Element> elements;
};

class VertexData
{
public:
	class Vertex
	{
	public:
		Vertex( const VertexLayout& vl,char* pData ) noexcept
			:
			vl( vl ),
			pData( pData )
		{}
		template<VertexLayout::ElementType ET>
		auto& Get() const noexcept(!IS_DEBUG)
		{
			auto e = vl.QuerraElements( ET );
			auto data = pData + e.GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<ET>::sysType*>(data);
		}
	private:
		const VertexLayout& vl;
		char* pData = nullptr;
	};
public:
	VertexData( VertexLayout&& vl ) noexcept
		:
		vl( std::move( vl ) )
	{}
	template<typename T>
	void Emplace_Back( T&& val )
	{
		buffer.reserve( buffer.size() + sizeof( T ) );
		for ( size_t i = 0; i < sizeof( T ); ++i )
		{
			buffer.emplace_back( *(reinterpret_cast<char*>(&val) + i) );
		}
	}
	template<typename T,typename... Args>
	void Emplace_Back( T&& first,Args&&... args )
	{
		Emplace_Back( std::forward<T>( first ) );
		Emplace_Back( std::forward<Args>( args )... );
	}
	size_t Size() const noexcept
	{
		return buffer.size();
	}
	size_t LayoutSize() const noexcept
	{
		return vl.LayoutSize();
	}
	const char* data() const noexcept
	{
		return buffer.data();
	}
	Vertex operator[]( size_t index ) noexcept(!IS_DEBUG)
	{
		assert( (LayoutSize() * index) < buffer.size() );
		assert( (LayoutSize() * (index + 1u)) <= buffer.size() );
		return Vertex( vl,buffer.data() + (LayoutSize() * index) );
	}
private:
	VertexLayout vl;
	std::vector<char> buffer;
};