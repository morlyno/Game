#include "VertexBuffer.h"
#include <typeinfo>

using namespace Bind;

void VertexBuffer::Bind( Graphics& gfx ) noexcept
{
	const UINT offset = 0u;
	GetContext( gfx )->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
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
