#include "ColorCBuf.h"

ColorCBuf::ColorCBuf( Graphics& gfx,const Drawable& parent )
	:
	parent( parent ),
	pcbuf( gfx  )
{
}

void ColorCBuf::Bind( Graphics& gfx ) noexcept
{
	pcbuf.Update(
		gfx,
		parent.GetColorXM()
	);
	pcbuf.Bind( gfx );
}
