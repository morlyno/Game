#include "TransformCBuf.h"

using namespace Bind;

TransformCBuf::TransformCBuf( Graphics& gfx,const Drawable& parent )
	:
	parent( parent ),
	vcbuf( std::make_unique<VertexConstantBuffer<Transform>>( gfx ) )
{
}

void TransformCBuf::Bind( Graphics& gfx ) noexcept
{
	const auto model = parent.GetTransformXM();
	const Transform tf =
	{
		DirectX::XMMatrixTranspose( model ),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		),
		gfx.GetCameraPos()
	};
	vcbuf->Update( gfx,tf );
	vcbuf->Bind( gfx );
}

ConstBuffDoubleBoy::ConstBuffDoubleBoy( Graphics& gfx,const Drawable& parent )
	:
	TransformCBuf( gfx,parent ),
	pcbuf( std::make_unique<PixelConstantBuffer<PixelCBuff>>( gfx,1u ) )
{}

void ConstBuffDoubleBoy::Bind( Graphics& gfx ) noexcept
{
	TransformCBuf::Bind( gfx );
	const PixelCBuff pt =
	{
		parent.GetColorXM(),
		parent.GetSpecularIntesity(),
		parent.GetSpecularPower(),
		parent.GetNormalMap() ? 1u : 0u,
	};
	pcbuf->Update( gfx,pt );
	pcbuf->Bind( gfx );
}