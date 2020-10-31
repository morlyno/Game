#pragma once

#include "Bindable.h"
#include "../ErrorHandle/Macros/GraphicsThrowMacros.h"
#include "../VertexLayout.h"
#include <memory>
#include "../BindableCodex.h"
#include <functional>

namespace Bind
{

	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer( Graphics& gfx,const VertexData& vd,const std::string& tag );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<VertexBuffer> Resolve( Graphics& gfx,const VertexData& vd,std::function<void()> func,const std::string& tag ) noexcept;
		static std::string GenerateKey( const std::string& tag ) noexcept;
		std::string GetKey() const noexcept override;
	private:
		const UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		std::string tag;
	};

}