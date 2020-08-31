#pragma once

#include "Bindable.h"
#include "../Drawable/Drawable.h"
#include "ConstantBuffers.h"

class TransformCBuf : public Bindable
{
public:
	TransformCBuf( Graphics& gfx,const Drawable& parent );
	void Bind( Graphics& gfx ) noexcept override;
private:
	const Drawable& parent;
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	PixelConstantBuffer<DirectX::XMFLOAT4> pcbuf;
};