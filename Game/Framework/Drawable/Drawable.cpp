#include "Drawable.h"
#include "../Bindable/Bindable.h"
#include "../Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	for ( auto& b : binds )
	{
		b->Bind( gfx );
	}
	for ( auto& sb : GetStaticBinds() )
	{
		sb->Bind( gfx );
	}
	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> addbind ) noexcept( !IS_DEBUG )
{
	assert( "Use AddIndexBuffer for binding an IndexBuffer" && typeid( *addbind ) != typeid( IndexBuffer ) );
	binds.push_back( std::move( addbind ) );
}

void Drawable::AddIndexBuffer( std::unique_ptr<IndexBuffer> indexBuffer ) noexcept( !IS_DEBUG )
{
	assert( "pIndexBuffer already set" && pIndexBuffer == nullptr );
	pIndexBuffer = indexBuffer.get();
	binds.push_back( std::move( indexBuffer ) );
}
