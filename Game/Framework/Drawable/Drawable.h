#pragma once

#include "../Graphics.h"
#include "../Bindable/Bindable.h"
//#include "../Bindable/IndexBuffer.h"
#include <memory>
#include <DirectXMath.h>

class Bindable;

class Drawable
{
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable() = default;
	void Draw( Graphics& gfx ) const noexcept( !IS_DEBUG );
	virtual void Update( float dt ) noexcept = 0;
	void AddBind( std::unique_ptr<Bindable> addbind ) noexcept;
	void AddIndexBuffer( std::unique_ptr<class IndexBuffer> indexBuffer ) noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};