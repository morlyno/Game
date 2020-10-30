#include "PixelShader.h"
#include <typeinfo>
#include "../BindableCodex.h"

using namespace Bind;

PixelShader::PixelShader( Graphics& gfx,const std::string& path )
	:
	path( path )
{
	INFOMAN( gfx );

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO( D3DReadFileToBlob( (L"Framework/Shader/ShaderByteCodes/" + std::wstring{ path.begin(),path.end() }).c_str(),&pBlob ) );
	GFX_THROW_INFO( GetDevice( gfx )->CreatePixelShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pPixelShader ) );
}

void PixelShader::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->PSSetShader( pPixelShader.Get(),nullptr,0u );
}

std::shared_ptr<PixelShader> Bind::PixelShader::Resolve( Graphics& gfx,const std::string& path ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( path ) ) )
	{
		return std::dynamic_pointer_cast<PixelShader>(bind);
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<PixelShader>( gfx,path ) );
	return std::dynamic_pointer_cast<PixelShader>(bind);
}

std::string Bind::PixelShader::GenerateKey( const std::string& path ) noexcept
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + path;
}

std::string Bind::PixelShader::GetKey() const noexcept
{
	return GenerateKey( path );
}
