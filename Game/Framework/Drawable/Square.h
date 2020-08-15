#pragma once

#include "Drawable.h"

class Square : public Drawable
{
public:
	Square( Graphics& gfx,float x,float y,float dx,float dy,float angle,float dangle );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float x;
	float y;
	
	float tx = 0;
	float ty = 0;

	float dx;
	float dy;

	float angle;

	float dangle;
	
	bool MoveRight = true;
};