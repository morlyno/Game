#pragma once

#include "Graphics.h"

class Camera
{
public:
	void ShowControlWindow() noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
private:
	void Reset() noexcept;
private:
	float x = 0.0f;
	float y = 0.0f;
	float z = 20.0f;

	union
	{
		struct
		{
			float look_x;
			float look_y;
			float look_z;
		};
		float look_xyz[3] = { 0.0f };
	};
	float r = 20.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	bool rotation_camera = true;
};