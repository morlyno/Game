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
	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::shared_ptr<Bind::Bindable> bind ) noexcept( !IS_DEBUG )
{
	if ( const auto i = dynamic_cast<Bind::IndexBuffer*>( bind.get() ) )
	{
		assert( "pIndexBuffer already set" && pIndexBuffer == nullptr );
		pIndexBuffer = i;
	}
	binds.push_back( std::move( bind ) );
}
