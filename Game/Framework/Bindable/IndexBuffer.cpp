#include "IndexBuffer.h"
#include <typeinfo>

using namespace Bind;

IndexBuffer::IndexBuffer( Graphics& gfx,std::vector<unsigned short> indices,const std::string& tag )
	:
	count( (UINT)indices.size() ),
	tag( tag )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC bdi = {};
	bdi.ByteWidth = UINT( count * sizeof( unsigned short ) );
	bdi.StructureByteStride = sizeof( unsigned short );
	bdi.Usage = D3D11_USAGE_DEFAULT;
	bdi.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdi.CPUAccessFlags = 0u;
	bdi.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA sdi = {};
	sdi.pSysMem = indices.data();

	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &bdi,&sdi,&pIndexBuffer ) );
}

void IndexBuffer::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->IASetIndexBuffer( pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u );
}

UINT IndexBuffer::GetCount() const
{
	return count;
}

std::string Bind::IndexBuffer::GenerateKey( const std::string& tag ) noexcept
{
	using namespace std::string_literals;
	return typeid(IndexBuffer).name() + "#"s + tag;
}

std::string Bind::IndexBuffer::GetKey() const noexcept
{
	return GenerateKey( tag );
}
