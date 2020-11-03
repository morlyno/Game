#pragma once

#include "Graphics.h"
#include "Inputs/Mouse.h"
#include "Inputs/Keyboard.h"

class Camera
{
public:
	Camera() noexcept;
	void ShowControlWindow() noexcept;
	void Inputs( const Mouse::Event& mouse,const Keyboard& kbd ) noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
private:
	void Reset() noexcept;
	void WrapAngles() noexcept;
private:
	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTOR eyeDir;
	float d_pitch;
	float d_yaw;
};