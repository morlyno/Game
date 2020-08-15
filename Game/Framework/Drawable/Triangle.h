#pragma once

#include "DrawableBase.h"

class Triangle : public DrawableBase<Triangle>
{
public:
	Triangle( Graphics& gfx,float x,float y,float dx,float dy,float angle,float dangle );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float tx = 0;
	float ty = 0;

	float x;
	float y;

	float dx;
	float dy;

	float angle;

	float dangle;

	bool MoveRight = true;
};