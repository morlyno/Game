#pragma once

#include "ConstantBuffers.h"
#include "../Drawable/Drawable.h"

class ColorCBuf : public Bindable
{
public:
	ColorCBuf( Graphics& gfx,const Drawable& parent );
	void Bind( Graphics& gfx ) noexcept override;
private:
	const Drawable& parent;
	PixelConstantBuffer<DirectX::XMFLOAT4> pcbuf;
};