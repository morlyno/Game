#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

void Drawable::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	for ( auto& b : binds )
	{
		b->Bind( gfx );
	}
	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> addbind ) noexcept
{
	binds.push_back( std::move( addbind ) );
}

void Drawable::AddIndexBuffer( std::unique_ptr<IndexBuffer> indexBuffer ) noexcept
{
	pIndexBuffer = indexBuffer.get();
	binds.push_back( std::move( indexBuffer ) );
}
