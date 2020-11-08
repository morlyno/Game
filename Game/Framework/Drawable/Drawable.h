#pragma once

#include "../Graphics.h"
#include <memory>
#include <DirectXMath.h>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

class Drawable
{
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable() = default;
	void Draw( Graphics& gfx ) const noexcept( !IS_DEBUG );
	virtual void Update( float dt ) noexcept = 0;
	bool AddBind( std::shared_ptr<Bind::Bindable> bind ) noexcept( !IS_DEBUG );
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual DirectX::XMFLOAT4 GetColorXM() const noexcept = 0;
	virtual float GetSpecularPower() const noexcept = 0;
	virtual float GetSpecularIntesity() const noexcept = 0;
	virtual bool GetNormalMap() const noexcept = 0;
	virtual bool SpawnControlWindow() noexcept = 0;
	virtual std::string GetType() const noexcept = 0;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bind::Bindable>> binds;
};