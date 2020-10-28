#pragma once

#include "DrawableMemberData.h"

class Cube : public DrawableMemberData
{
public:
	Cube( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index,bool test = false );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
	//bool SpawnControlWindow() noexcept override;
private:
	bool Rotate_Roll = false;
	bool Rotate_Pitch = false;
	bool Rotate_Yaw = false;

	bool test;
};