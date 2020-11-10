#pragma once
#include "DrawableMemberData.h"

class Sheet : public DrawableMemberData
{
public:
	Sheet( Graphics& gfx,float x,float y,float z,float width,float height,float depth,int index );
	DirectX::XMMATRIX GetTransformXM() const noexcept ;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
};