#pragma once

#include "DrawableBase.h"

class Sheet : public DrawableBase<Sheet>
{
public:
	Sheet( Graphics& gfx,float x,float y,float dx,float dy,float angle,float dangle );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float oreginx;
	float oreginy;

	float tx = 0;
	float ty = 0;

	float dx;
	float dy;

	float angle;
	float dangle;
};