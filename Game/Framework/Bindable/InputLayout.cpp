#include "InputLayout.h"
#include "../VertexLayout.h"
#include <typeinfo>
#include "../BindableCodex.h"

using namespace Bind;

InputLayout::InputLayout( Graphics& gfx,const VertexLayout& vl,ID3DBlob* pVertexShaderBytecode )
	:
	layout( vl.GetLayoutString() )
{
	INFOMAN( gfx );

	GFX_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		vl.GetDesc().data(),
		(UINT)vl.GetDesc().size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	) );
}

void InputLayout::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->IASetInputLayout( pInputLayout.Get() );
}

std::shared_ptr<InputLayout> Bind::InputLayout::Resolve( Graphics& gfx,const VertexLayout& vl,ID3DBlob* pVertexShaderBytecode ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( vl.GetLayoutString() ) ) )
	{
		return std::dynamic_pointer_cast<InputLayout>(bind);
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<InputLayout>( gfx,vl,pVertexShaderBytecode ) );
	return std::dynamic_pointer_cast<InputLayout>(bind);
}

std::string InputLayout::GenerateKey( const std::string& layoutstr ) noexcept
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + layoutstr;
}

std::string InputLayout::GetKey() const noexcept
{
	return GenerateKey( layout );
}
