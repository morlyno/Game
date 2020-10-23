#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"

namespace Bind
{

	template<typename C>
	class ConstantBuffers : public Bindable
	{
	public:
		ConstantBuffers( Graphics& gfx,const C& consts,UINT slot = 0u )
			:
			slot( slot )
		{
			INFOMAN( gfx );

			D3D11_BUFFER_DESC cbd = {};
			cbd.ByteWidth = sizeof( consts );
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;

			GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,&csd,&pConstantBuffer ) );
		}
		ConstantBuffers( Graphics& gfx,UINT slot = 0u )
			:
			slot( slot )
		{
			INFOMAN( gfx );

			D3D11_BUFFER_DESC cbd = {};
			cbd.ByteWidth = sizeof( C );
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.StructureByteStride = 0u;

			GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,nullptr,&pConstantBuffer ) );
		}
		void Update( Graphics& gfx,const C& consts )
		{
			INFOMAN( gfx );

			D3D11_MAPPED_SUBRESOURCE ms;
			GFX_THROW_INFO( GetContext( gfx )->Map(
				pConstantBuffer.Get(),
				0u,
				D3D11_MAP_WRITE_DISCARD,
				0u,
				&ms
			) );
			memcpy( ms.pData,&consts,sizeof( consts ) );
			GetContext( gfx )->Unmap( pConstantBuffer.Get(),0u );
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		UINT slot;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffers<C>
	{
		using ConstantBuffers<C>::pConstantBuffer;
		using Bindable::GetContext;
		using ConstantBuffers<C>::slot;
	public:
		using ConstantBuffers<C>::ConstantBuffers;
		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->VSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() );
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffers<C>
	{
		using ConstantBuffers<C>::pConstantBuffer;
		using Bindable::GetContext;
		using ConstantBuffers<C>::slot;
	public:
		using ConstantBuffers<C>::ConstantBuffers;
		void Bind( Graphics& gfx ) noexcept override
		{
			GetContext( gfx )->PSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() );
		}
	};

}