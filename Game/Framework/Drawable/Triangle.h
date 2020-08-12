#pragma once

#include "Drawable.h"

class Square : public Drawable
{
public:
	Square( Graphics& gfx,float x,float y,float dx );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float x;
	float y;
	
	float dx;
	bool MoveRight = true;
};