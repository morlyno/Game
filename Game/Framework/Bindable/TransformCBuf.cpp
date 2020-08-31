#include "TransformCBuf.h"

TransformCBuf::TransformCBuf( Graphics& gfx,const Drawable& parent )
	:
	parent( parent ),
	vcbuf( gfx ),
	pcbuf( gfx )
{
}

void TransformCBuf::Bind( Graphics& gfx ) noexcept
{
	vcbuf.Update(
		gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	);
	pcbuf.Update(
		gfx,
		parent.GetColorXM()
	);
	vcbuf.Bind( gfx );
	pcbuf.Bind( gfx );
}
