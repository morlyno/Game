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
		Tangent,
		Bitangent,
		Count
	};
	template<ElementType> struct Map;
	template<> struct Map<ElementType::Position2D>
	{
		using sysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* lable = "Position2D";
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
	template<> struct Map<ElementType::Tangent>
	{
		using sysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* lable = "Tangent";
	};
	template<> struct Map<ElementType::Bitangent>
	{
		using sysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* lable = "Bitangent";
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
			case Tangent:
				return sizeof( Map<Tangent>::sysType );
			case Bitangent:
				return sizeof( Map<Bitangent>::sysType );
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
	const Element& QuerraElements( const ElementType& et ) const noexcept(!IS_DEBUG)
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
	auto GetDesc() const noexcept(!IS_DEBUG)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
		ied.reserve( elements.size() );
		for ( const auto& e : elements )
		{
			ied.push_back( D3D11_INPUT_ELEMENT_DESC{} );
			switch ( e.GetType() )
			{
			case Position2D:
				ied.back() = { Map<Position2D>::lable,0u,Map<Position2D>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			case Position3D:
				ied.back() = { Map<Position3D>::lable,0u,Map<Position3D>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			case Normal:
				ied.back() = { Map<Normal>::lable,0u,Map<Normal>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			case Texture2D:
				ied.back() = { Map<Texture2D>::lable,0u,Map<Texture2D>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			case Tangent:
				ied.back() = { Map<Tangent>::lable,0u,Map<Tangent>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			case Bitangent:
				ied.back() = { Map<Bitangent>::lable,0u,Map<Bitangent>::format,0u,(UINT)e.GetOffset(),D3D11_INPUT_PER_VERTEX_DATA,0u };
				break;
			default:
				assert( false && "Invalid Tempalte Argument" );
				break;
			}
		}
		return std::move( ied );
	}
	auto GetLayoutString() const noexcept(!IS_DEBUG)
	{
		std::string str;
		for ( const auto& e : elements )
		{
			switch ( e.GetType() )
			{
			case Position2D:
				str += Map<Position2D>::lable;
				break;
			case Position3D:
				str += Map<Position3D>::lable;
				break;
			case Normal:
				str += Map<Normal>::lable;
				break;
			case Texture2D:
				str += Map<Texture2D>::lable;
				break;
			case Tangent:
				str += Map<Tangent>::lable;
				break;
			case Bitangent:
				str += Map<Bitangent>::lable;
				break;
			default:
				assert( false && "Invalid Tempalte Argument" );
				break;
			}
		}
		return std::move( str );
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
			const auto e = vl.QuerraElements( ET );
			auto data = pData + e.GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<ET>::sysType*>(data);
		}
	private:
		const VertexLayout& vl;
		char* pData = nullptr;
	};
	class ConstVertex
	{
	public:
		ConstVertex( const VertexLayout& vl,const char* pData ) noexcept
			:
			vl( vl ),
			pData( pData )
		{}
		template<VertexLayout::ElementType ET>
		const auto& Get() const noexcept(!IS_DEBUG)
		{
			const auto e = vl.QuerraElements( ET );
			auto data = pData + e.GetOffset();
			return *reinterpret_cast<const typename VertexLayout::Map<ET>::sysType*>(data);
		}
	private:
		const VertexLayout& vl;
		const char* pData = nullptr;
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
	template<typename First,typename... Args>
	void Emplace_Back( First&& first,Args&&... args )
	{
		Emplace_Back( std::forward<First>( first ) );
		Emplace_Back( std::forward<Args>( args )... );
	}
	size_t ByteSize() const noexcept
	{
		return buffer.size();
	}
	size_t Size() const noexcept
	{
		return buffer.size() / vl.LayoutSize();
	}
	size_t LayoutSize() const noexcept
	{
		return vl.LayoutSize();
	}
	const char* data() const noexcept
	{
		return buffer.data();
	}
	const auto GetDesc() const noexcept
	{
		return std::move( vl.GetDesc() );
	}
	const VertexLayout& GetLayout() const noexcept
	{
		return vl;
	}
	Vertex operator[]( size_t index ) noexcept(!IS_DEBUG)
	{
		assert( (LayoutSize() * index) < buffer.size() );
		assert( (LayoutSize() * (index + 1u)) <= buffer.size() );
		return Vertex( vl,buffer.data() + (LayoutSize() * index) );
	}
	ConstVertex operator[]( size_t index ) const noexcept(!IS_DEBUG)
	{
		assert( (LayoutSize() * index) < buffer.size() );
		assert( (LayoutSize() * (index + 1u)) <= buffer.size() );
		return ConstVertex( vl,buffer.data() + (LayoutSize() * index) );
	}
	Vertex back() noexcept
	{
		return Vertex( vl,(buffer.end() - LayoutSize())._Ptr );
	}
	Vertex front() noexcept
	{
		return Vertex( vl,buffer.data() );
	}
	ConstVertex back() const noexcept
	{
		return ConstVertex( vl,(buffer.end() - LayoutSize())._Ptr );
	}
	ConstVertex front() const noexcept
	{
		return ConstVertex( vl,buffer.data() );
	}
	ConstVertex const_back() const noexcept
	{
		return back();
	}
	ConstVertex const_front() const noexcept
	{
		return front();
	}
	void Resize( size_t new_size ) noexcept
	{
		buffer.resize( new_size * vl.LayoutSize() );
	}
	void Reserve( size_t new_capacity ) noexcept
	{
		buffer.reserve( new_capacity * vl.LayoutSize() );
	}
private:
	VertexLayout vl;
	std::vector<char> buffer;
};