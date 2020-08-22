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
};