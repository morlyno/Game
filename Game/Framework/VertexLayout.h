#pragma once

#include <vector>
#include <utility>
#include <DirectXMath.h>

//class Element
//{
//public:
//	Element( unsigned int size )
//		:
//		byteSize( size ),
//		bytes( size )
//	{}
//	template<typename T>
//	void Set( const T& data )
//	{
//		*bytes.data() = *reinterpret_cast<char*>(&data);
//	}
//private:
//	unsigned int byteSize;
//	std::vector<char> bytes;
//};
//
//class VertexLayout
//{
//public:
//	enum ElementType
//	{
//		Position2D,
//		Position3D,
//		Normal,
//		Texture2D
//	};
//	template<ElementType ET>
//	VertexLayout& Add()
//	{
//		assert( false && "Wronge Template Argument")
//		return *this;
//	}
//	template<typename T>
//	void Set( const T& _data )
//	{
//		data[index].Set( data );
//	}
//private:
//	unsigned int nElements = 0;
//	unsigned int index = 0;
//	std::vector<Element> data;
//};
//
//class VertexData
//{
//public:
//	VertexData( VertexLayout&& vl )
//		:
//		vl( std::move( vl ) )
//	{
//		elements.emplace_back( this->vl );
//	}
//private:
//	template<typename T>
//	void Emplace( const T& data )
//	{
//		if ( index == 0 )
//		{
//			elements.emplace_back( vl );
//		}
//		elements[index].Set( data );
//		++index;
//	}
//public:
//	template<typename T>
//	void Emplace_Back( const T& data )
//	{
//		Emplace( data );
//		index = 0;
//	}
//	template<typename T,typename... Args>
//	void Emplace_Back( const T& data,const Args&&... args )
//	{
//		Emplace( data );
//		Emplace_Back( std::forward( args... ) );
//	}
//private:
//	VertexLayout vl;
//	unsigned int index = 0;
//	std::vector<VertexLayout> elements;
//};
//
//template<>
//VertexLayout& VertexLayout::Add<VertexLayout::Position2D>()
//{
//	data.emplace_back( sizeof( float ) * 2 );
//	++nElements;
//	return *this;
//}
//template<>
//VertexLayout& VertexLayout::Add<VertexLayout::Position3D>()
//{
//	data.emplace_back( sizeof( float ) * 3 );
//	++nElements;
//	return *this;
//}
//template<>
//VertexLayout& VertexLayout::Add<VertexLayout::Normal>()
//{
//	data.emplace_back( sizeof( float ) * 3 );
//	++nElements;
//	return *this;
//}
//template<>
//VertexLayout& VertexLayout::Add<VertexLayout::Texture2D>()
//{
//	data.emplace_back( sizeof( float ) * 2 );
//	++nElements;
//	return *this;
//}

#define POSITION2D	DirectX::XMFLOAT2
#define POSITION3D	DirectX::XMFLOAT3
#define NORMAL		DirectX::XMFLOAT3
#define TEXTURE2D	DirectX::XMFLOAT2

//#define P2D_SIZE sizeof((POSITION2D_TYPE))
//#define P3D_SIZE sizeof((POSITION3D_TYPE))
//#define NOR_SIZE sizeof((NORMAL_TYPE))
//#define T2D_SIZE sizeof((TEXTURE2D_TYPE))

enum class ElementType
{
	Position2D,
	Position3D,
	Normal,
	Texture2D
};

class Element
{
public:
	Element( const ElementType& ET,unsigned int offset,unsigned int size )
		:
		offset( offset ),
		size( size ),
		ET( ET )
	{}
	unsigned int GetSize() const
	{
		return size;
	}
	unsigned int GetOffSet() const
	{
		return offset;
	}
	const ElementType& GetType() const
	{
		return ET;
	}
private:
	const ElementType ET;
	unsigned int offset;
	unsigned int size;
};

class VertexLayout
{
public:
	VertexLayout& Add( const ElementType& ET )
	{
		auto bSize = GetSize( ET );
		LayoutByteSize += bSize;
		elements.emplace_back( ET,elements.empty() ? 0u : GetOffSet(),bSize );
		return *this;
	}
	unsigned int GetOffSet() const
	{
		auto& i = elements.back();
		return i.GetOffSet() + i.GetSize();
	}
	unsigned int GetSize( const ElementType& et ) const
	{
		switch ( et )
		{
		case ElementType::Position2D:
			return sizeof( POSITION2D );
			break;
		case ElementType::Position3D:
			return sizeof( POSITION3D );
			break;
		case ElementType::Normal:
			return sizeof( NORMAL );
			break;
		case ElementType::Texture2D:
			return sizeof( TEXTURE2D );
			break;
		default:
			assert( false && "Invalid Element Type" );
			return -1;
			break;
		}
	}
	auto& GetElements() const
	{
		return elements;
	}
	auto GetLayoutByteSize() const
	{
		return LayoutByteSize;
	}
private:
	unsigned int LayoutByteSize = 0u;
	std::vector<Element> elements;
};

class VertexData
{
public:
	VertexData( VertexLayout&& vl )
		:
		vl( std::move( vl ) )
	{}
	template<typename T>
	void Emplace_Back( T&& val )
	{
		for ( size_t i = 0; i < sizeof( T ); ++i )
		{
			data.emplace_back( *(reinterpret_cast<char*>(&val) + i) );
		}
	}
	template<typename T,typename... Args>
	void Emplace_Back( T&& first,Args&&... args )
	{
		Emplace_Back( std::forward<T>( first ) );
		Emplace_Back( std::forward<Args>( args )... );
	}
	auto GetDesc() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied( vl.GetElements().size() );
		for ( const auto& e : vl.GetElements() )
		{
			switch ( e.GetType() )
			{
			case ElementType::Position2D:
				ied.push_back( D3D11_INPUT_ELEMENT_DESC{ "Position2D",0,DXGI_FORMAT_R32G32_FLOAT,0,e.GetOffSet(),D3D11_INPUT_PER_VERTEX_DATA,0 } );
				break;
			case ElementType::Position3D:
				ied.push_back( D3D11_INPUT_ELEMENT_DESC{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,e.GetOffSet(),D3D11_INPUT_PER_VERTEX_DATA,0 } );
				break;
			case ElementType::Normal:
				ied.push_back( D3D11_INPUT_ELEMENT_DESC{ "Normal",0,DXGI_FORMAT_R32G32_FLOAT,0,e.GetOffSet(),D3D11_INPUT_PER_VERTEX_DATA,0 } );
				break;
			case ElementType::Texture2D:
				ied.push_back( D3D11_INPUT_ELEMENT_DESC{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,e.GetOffSet(),D3D11_INPUT_PER_VERTEX_DATA,0 } );
				break;
			default:
				assert( false && "Invalid Element Type" );
				break;
			}
		}
		return std::move( ied );
	}
	auto& GetData() const
	{
		return data;
	}
	auto Data() const
	{
		return data.data();
	}
	auto GetLayout() const
	{
		return vl;
	}
private:
	VertexLayout vl;
	std::vector<char> data;
};
