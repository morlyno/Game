#include "TransformCBuf.h"

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
		)
	};
	vcbuf->Update( gfx,tf );
	vcbuf->Bind( gfx );
}
