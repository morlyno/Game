#include "Texture.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"

using namespace Bind;

Texture::Texture( Graphics& gfx,const Surface& s )
{
	INFOMAN( gfx );

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = s.GetWidht();
	td.Height = s.GetWidht();
	td.MipLevels = 1u;
	td.ArraySize = 1u;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1u;
	td.SampleDesc.Quality = 0u;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0u;
	td.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = s.GetBufferPointer();
	srd.SysMemPitch = s.GetWidht() * sizeof( Surface::Color );
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &td,&srd,&pTexture ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1u;
	srvd.Texture2D.MostDetailedMip = 0u;

	GFX_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView(
		pTexture.Get(),
		&srvd,
		&pTextureView
	) );
}

void Texture::Bind( Graphics& gfx ) noexcept
{
	GetContext( gfx )->PSSetShaderResources( 0u,1u,pTextureView.GetAddressOf() );
}
