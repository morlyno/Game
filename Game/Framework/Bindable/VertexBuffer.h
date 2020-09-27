#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include <vector>
#include "../VertexLayout.h"

class VertexBuffer : public Bindable
{	
public:
	template<typename T>
	VertexBuffer( Graphics& gfx,const std::vector<T>& vertecies )
		:
		stride( sizeof( T ) )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = UINT( vertecies.size() * sizeof( T ) );
		bd.StructureByteStride = sizeof( T );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertecies.data();
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer(
			&bd,
			&sd,
			&pVertexBuffer
		) );
	}
	VertexBuffer( Graphics& gfx,const VertexData& vd )
		:
		stride( vd.GetLayout().GetLayoutByteSize() )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = UINT( vd.GetData().size() );
		bd.StructureByteStride = vd.GetLayout().GetLayoutByteSize();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vd.Data();
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer(
			&bd,
			&sd,
			&pVertexBuffer
		) );
	}
	void Bind( Graphics& gfx ) noexcept override;
private:
	const UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};