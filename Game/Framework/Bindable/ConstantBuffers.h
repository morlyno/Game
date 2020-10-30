#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include <typeinfo>
#include <memory>
#include "../BindableCodex.h"

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
		static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& gfx,const C& consts,UINT slot ) noexcept
		{
			if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( slot ) ) )
			{
				return std::dynamic_pointer_cast<VertexConstantBuffer>(bind);
			}
			const auto& bind = Bind::Codex::Store( std::make_shared<VertexConstantBuffer>( gfx,consts,slot ) );
			return std::dynamic_pointer_cast<VertexConstantBuffer>(bind);
		}
		static std::shared_ptr<VertexConstantBuffer> Resolve( Graphics& gfx,UINT slot ) noexcept
		{
			if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( slot ) ) )
			{
				return std::dynamic_pointer_cast<VertexConstantBuffer>(bind);
			}
			const auto& bind = Bind::Codex::Store( std::make_shared<VertexConstantBuffer>( gfx,slot ) );
			return std::dynamic_pointer_cast<VertexConstantBuffer>(bind);
		}
		static std::string GenerateKey( UINT slot ) noexcept
		{
			using namespace std::string_literals;
			return typeid(VertexConstantBuffer).name() + "#"s + std::to_string( slot );
		}
		std::string GetKey() const noexcept
		{
			return GenerateKey( slot );
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
		static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& gfx,const C& consts,UINT slot ) noexcept
		{
			if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( slot ) ) )
			{
				return std::dynamic_pointer_cast<PixelConstantBuffer>(bind);
			}
			const auto& bind = Bind::Codex::Store( std::make_shared<PixelConstantBuffer>( gfx,consts,slot ) );
			return std::dynamic_pointer_cast<PixelConstantBuffer>(bind);
		}
		static std::shared_ptr<PixelConstantBuffer> Resolve( Graphics& gfx,UINT slot ) noexcept
		{
			if ( const auto& bind = Bind::Codex::Resolve( GenerateKey( slot ) ) )
			{
				return std::dynamic_pointer_cast<PixelConstantBuffer>(bind);
			}
			const auto& bind = Bind::Codex::Store( std::make_shared<PixelConstantBuffer>( gfx,slot ) );
			return std::dynamic_pointer_cast<PixelConstantBuffer>(bind);
		}
		static std::string GenerateKey( UINT slot ) noexcept
		{
			using namespace std::string_literals;
			return typeid(PixelConstantBuffer).name() + "#"s + std::to_string( slot );
		}
		std::string GetKey() const noexcept
		{
			return GenerateKey( slot );
		}
	};

}