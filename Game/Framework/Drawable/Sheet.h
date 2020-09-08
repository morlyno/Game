#pragma once

#include "DrawableBase.h"
#include "DrawableMemberData.h"

class Sheet : public DrawableMemberData<Sheet>
{
public:
	Sheet( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float scale_width,float scale_height,float scale_depth,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	DirectX::XMFLOAT4 GetColorXM() const noexcept override;
	std::string GetType() const noexcept override;
};