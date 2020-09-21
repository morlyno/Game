#pragma once

#include "DrawableMemberData.h"

class Sphere : public DrawableMemberData<Sphere>
{
public:
	Sphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
	bool SpawnControlWindow() noexcept override;
};