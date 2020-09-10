#pragma once

#include "DrawableBase.h"
#include "DrawableMemberData.h"

class Triangle : public DrawableMemberData<Triangle>
{
public:
	Triangle( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	DirectX::XMFLOAT4 GetColorXM() const noexcept override;
	std::string GetType() const noexcept override;
};