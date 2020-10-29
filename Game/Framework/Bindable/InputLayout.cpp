#include "InputLayout.h"
#include "../VertexLayout.h"
#include <typeinfo>

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

std::string InputLayout::GenerateKey( const std::string& layout ) noexcept
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + layout;
}

std::string InputLayout::GetKey() const noexcept
{
	return GenerateKey( layout );
}
