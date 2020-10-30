#include "Sampler.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include <typeinfo>
#include "../BindableCodex.h"

using namespace Bind;

Sampler::Sampler( Graphics& gfx )
{
	INFOMAN( gfx );

	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	GFX_THROW_INFO( GetDevice( gfx )->CreateSamplerState( &sd,&pSampler ) );
}

void Sampler::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->PSSetSamplers( 0u,1u,pSampler.GetAddressOf() );
}

std::shared_ptr<Sampler> Bind::Sampler::Resolve( Graphics& gfx ) noexcept
{
	if ( const auto& bind = Bind::Codex::Resolve( GenerateKey() ) )
	{
		return std::dynamic_pointer_cast<Sampler>(bind);
	}
	const auto& bind = Bind::Codex::Store( std::make_shared<Sampler>( gfx ) );
	return std::dynamic_pointer_cast<Sampler>(bind);
}

std::string Bind::Sampler::GenerateKey() noexcept
{
	return typeid(Sampler).name();
}

std::string Bind::Sampler::GetKey() const noexcept
{
	return GenerateKey();
}
