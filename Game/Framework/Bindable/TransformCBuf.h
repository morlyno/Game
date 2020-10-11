#pragma once

#include "Bindable.h"
#include "../Drawable/Drawable.h"
#include "ConstantBuffers.h"

class TransformCBuf : public Bindable
{
	struct Transform
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		alignas(16) DirectX::XMFLOAT3 pos;
	};
public:
	TransformCBuf( Graphics& gfx,const Drawable& parent );
	void Bind( Graphics& gfx ) noexcept override;
protected:
	const Drawable& parent;
	std::unique_ptr<VertexConstantBuffer<Transform>> vcbuf;
};

class ConstBuffDoubleBoy : public TransformCBuf
{
	struct PixelCBuff
	{
		DirectX::XMFLOAT4 matirial;
		float specularIntesity;
		float specularPower;
		float buffer0;
		float buffer1;
	};
public:
	ConstBuffDoubleBoy( Graphics& gfx,const Drawable& parent );
	void Bind( Graphics& gfx ) noexcept override;
private:
	std::unique_ptr<PixelConstantBuffer<PixelCBuff>> pcbuf;
};
