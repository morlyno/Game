#pragma once

#include "Drawable/SolidSphere.h"
#include "Bindable/ConstantBuffers.h"
#include "Bindable/PixelShader.h"

class PointLight
{
	struct PointLightConstants
	{
		alignas(16) DirectX::XMFLOAT3 pos;
	};
public:
	PointLight( Graphics& gfx,float x,float y,float z );
	void Draw( Graphics& gfx ) noexcept( !IS_DEBUG );
	void Bind( Graphics& gfx ) noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
private:
	PointLightConstants PLConst;
	SolidSphere mesh;
	PixelConstantBuffer<PointLightConstants> pcb;
};