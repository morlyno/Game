#pragma once

#include "DrawableMemberData.h"

class SolidSphere : public DrawableMemberData<SolidSphere>
{
public:
	SolidSphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
	bool SpawnControlWindow() noexcept override;
};