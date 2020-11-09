#pragma once
#include "Drawable.h"

class Model : public Drawable
{
public:
	Model( Graphics& gfx,const std::string& path,float x,float y,float z );
	void Update( float dt ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	float GetSpecularPower() const noexcept;
	float GetSpecularIntesity() const noexcept;
	bool GetNormalMap() const noexcept;
	bool SpawnControlWindow() noexcept;
	std::string GetType() const noexcept;
private:
	float scale_width = 1.0f;;
	float scale_height = 1.0f;
	float scale_depth = 1.0f;
	float x;
	float y;
	float z;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
};