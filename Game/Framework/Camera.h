#pragma once

#include "Graphics.h"

class Camera
{
public:
	void ShowControlWindow() noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
private:
	void Reset() noexcept;
private:
	float x = 0.0f;
	float y = 0.0f;
	float z = 20.0f;

	float look_x = 0;
	float look_y = 0;
	float look_z = 0;
	float r = 20.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	bool rotation_camera = true;
};