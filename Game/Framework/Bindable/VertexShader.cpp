#include "VertexShader.h"
#include <typeinfo>
#include "../BindableCodex.h"

using namespace Bind;

VertexShader::VertexShader( Graphics& gfx,const std::string& path )
	:
	path( path )
{
	INFOMAN( gfx );

	GFX_THROW_INFO( D3DReadFileToBlob( (L"Framework/Shader/ShaderByteCodes/" + std::wstring{ path.begin(),path.end() }).c_str(),&pBytecodeBlob ) );
	GFX_THROW_INFO( GetDevice( gfx )->CreateVertexShader( pBytecodeBlob->GetBufferPointer(),pBytecodeBlob->GetBufferSize(),nullptr,&pVertexShader ) );
}

void VertexShader::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->VSSetShader( pVertexShader.Get(),nullptr,0u );
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}

std::shared_ptr<VertexShader> Bind::VertexShader::Resolve( Graphics& gfx,const std::string& path ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( path ) ) )
	{
		return std::dynamic_pointer_cast<VertexShader>( bind );
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<VertexShader>( gfx,path ) );
	return std::dynamic_pointer_cast<VertexShader>(bind);
}

std::string Bind::VertexShader::GenerateKey( const std::string& path ) noexcept
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + path;
}

std::string Bind::VertexShader::GetKey() const noexcept
{
	return GenerateKey( path );
}
