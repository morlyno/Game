#pragma once

#include "DrawableMemberData.h"

class SolidSphere : public DrawableMemberData
{
public:
	SolidSphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index );
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
};