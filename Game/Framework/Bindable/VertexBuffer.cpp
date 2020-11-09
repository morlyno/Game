#include "VertexBuffer.h"
#include <typeinfo>

using namespace Bind;

Bind::VertexBuffer::VertexBuffer( Graphics& gfx,const VertexData& vd,const std::string& tag )
	:
	stride( (UINT)vd.LayoutSize() ),
	tag( tag )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = (UINT)vd.ByteSize();
	bd.StructureByteStride = (UINT)vd.LayoutSize();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vd.data();
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer(
		&bd,
		&sd,
		&pVertexBuffer
	) );
}

void VertexBuffer::Bind( Graphics& gfx ) noexcept
{
	const UINT offset = 0u;
	GetContext( gfx )->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
}

std::shared_ptr<VertexBuffer> Bind::VertexBuffer::Resolve( Graphics& gfx,const VertexData& vd,std::function<void()> func,const std::string& tag ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( tag ) ) )
	{
		return std::dynamic_pointer_cast<VertexBuffer>(bind);
	}
	if ( func )
	{
		func();
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<VertexBuffer>( gfx,vd,tag ) );
	return std::dynamic_pointer_cast<VertexBuffer>(bind);
}

std::string Bind::VertexBuffer::GenerateKey( const std::string& tag ) noexcept
{
	using namespace std::string_literals;
	return typeid(VertexBuffer).name() + "#"s + tag;
}

std::string Bind::VertexBuffer::GetKey() const noexcept
{
	return GenerateKey( tag );
}
